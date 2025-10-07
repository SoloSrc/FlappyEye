#include <stdint.h>

#include "fe_sys.h"

#include "fe_app.h"
#include "stb_ds.h"

static SDL_FPoint s_calculateWorldPosition(SDL_FPoint base, SDL_FPoint offset)
{
	SDL_FPoint res = {
		.x = base.x + offset.x,
		.y = base.y + offset.y
	};
	return res;
}

static SDL_FPoint s_calculateScreenPosition(A_Context* ctx, D_Node* camera, SDL_FPoint worldPos)
{
	SDL_FPoint cameraPos = { .x = 0.0f, .y = 0.0f };
	for (int i = 0; i < stbds_arrlen(camera->components); i++) {
		D_Component* cmp = &camera->components[i];
		if (cmp->type == D_COMPONENT_TYPE_POSITION) {
			D_PositionComponent* posCmp = &cmp->position;
			cameraPos.x = posCmp->x;
			cameraPos.y = posCmp->y;
			break;
		}
	}
	SDL_Point screenDim;
	SDL_GetRenderOutputSize(ctx->renderer, &screenDim.x, &screenDim.y);

	SDL_FPoint topLeftPos;
	topLeftPos.x = cameraPos.x - (screenDim.x / 2.0f);
	topLeftPos.y = cameraPos.y + (screenDim.y / 2.0f);
	SDL_FPoint screenPos;
	screenPos.x =  worldPos.x - cameraPos.x - topLeftPos.x;
	// in screen position, y is inverted
	screenPos.y = -(worldPos.y - cameraPos.y - topLeftPos.y);

	return screenPos;
}

static SDL_FRect s_calculateSrcRect(D_Sprite* sprite, D_SpriteAnimationComponent* animCmp, float deltaTime)
{
	if (sprite->frames <= 1 || animCmp == NULL) {
		SDL_FRect src = {
			.x = 0.0f,
			.y = 0.0f,
			.w = sprite->width,
			.h = sprite->height
		};
		return src;
	}
	int currFrame = ((int)(animCmp->elapsedTime / animCmp->frameDuration));
	int frame = currFrame % sprite->frames;
	int col = frame % sprite->cols;
	int row = frame / sprite->cols;
	SDL_FRect src = {
		.x = col * sprite->width,
		.y = row * sprite->height,
		.w = sprite->width,
		.h = sprite->height
	};
	animCmp->elapsedTime += deltaTime;
	return src;
}

static void s_renderSprites(A_Context* ctx, D_Scene* scene, D_Node* node, SDL_FPoint parentPos, float deltaTime)
{
	D_Sprite* nodeSprite = NULL;
	D_SpriteAnimationComponent* animCmp = NULL;
	SDL_FPoint localPos = { 0.0f, 0.0f };
	bool hasPos = false;
	for (int i = 0; i < stbds_arrlen(node->components); i++) {
		D_Component* cmp = &node->components[i];

		if (cmp->type == D_COMPONENT_TYPE_SPRITE) {
			D_SpriteComponent* spriteCmp = &cmp->sprite;
			D_Sprite* sprite = spriteCmp->sprite;
			if (sprite != NULL && sprite->texture != NULL) {
				nodeSprite = sprite;
			}
			else {
				SDL_LogError(
					SDL_LOG_CATEGORY_APPLICATION,
					"Sprite not found or texture is NULL for Node: %s",
					node->name
				);
			}
		}

		if (cmp->type == D_COMPONENT_TYPE_SPRITE_ANIMATION) {
			animCmp = &cmp->animation;
		}
		
		if (cmp->type == D_COMPONENT_TYPE_POSITION) {
			D_PositionComponent* posCmp = &cmp->position;
			localPos.x = posCmp->x;
			localPos.y = posCmp->y;
			hasPos = true;
		}
	}
	if (!hasPos) {
		return;
	}
	SDL_FPoint worldPos = s_calculateWorldPosition(parentPos, localPos);

	for (int i = 0; i < stbds_arrlen(node->children); i++) {
		D_Node* child = node->children[i];
		s_renderSprites(ctx, scene, child, worldPos, deltaTime);
	}

	if (nodeSprite == NULL) {
		return;
	}

	SDL_FPoint screenPos = s_calculateScreenPosition(ctx, scene->camera, worldPos);

	// Render sprite
	SDL_FRect dst = {
		.x = screenPos.x,
		.y = screenPos.y,
		.w = nodeSprite->width,
		.h = nodeSprite->height
	};
	SDL_FRect src = s_calculateSrcRect(nodeSprite, animCmp, deltaTime);
	SDL_RenderTexture(ctx->renderer, (SDL_Texture*)nodeSprite->texture, &src, &dst);
}

void S_RenderScene(A_Context* ctx, D_Scene* scene, float deltaTime)
{
	if (ctx == NULL || ctx->renderer == NULL) {
		return;
	}
	if (scene == NULL || scene->root == NULL) {
		return;
	}
	SDL_FPoint root = { .x = 0.0f, .y = 0.0f };
	s_renderSprites(ctx, scene, scene->root, root, deltaTime);
}

static void s_applyVelocity(D_Node* node, float deltaTime)
{
	for (int i = 0; i < stbds_arrlen(node->children); i++) {
		D_Node* child = node->children[i];
		s_applyVelocity(child, deltaTime);
	}
	D_PositionComponent* posCmp = NULL;
	D_VelocityComponent* velCmp = NULL;
	for (int i = 0; i < stbds_arrlen(node->components); i++) {
		D_Component* cmp = &node->components[i];
		if (cmp->type == D_COMPONENT_TYPE_POSITION) {
			posCmp = &cmp->position;
			continue;
		}
		if (cmp->type == D_COMPONENT_TYPE_VELOCITY) {
			velCmp = &cmp->velocity;
			continue;
		}
	}
	if (posCmp == NULL || velCmp == NULL) {
		return;
	}
	posCmp->x += velCmp->x * deltaTime;
	posCmp->y += velCmp->y * deltaTime;
}

void S_ApplyVelocity(A_Context *ctx, D_Scene *scene, float deltaTime)
{
	if (ctx == NULL) {
		return;
	}
	if (scene == NULL || scene->root == NULL) {
		return;
	}
	s_applyVelocity(scene->root, deltaTime);
}

static void s_applyFrameUpdates(A_Context* ctx, D_Node* node, float deltaTime)
{
	for (int i = 0; i < stbds_arrlen(node->children); i++) {
		D_Node* child = node->children[i];
		s_applyFrameUpdates(ctx, child, deltaTime);
	}
	D_FrameUpdate f = stbds_hmget(ctx->updateCallbacks, (uintptr_t)node);
	if (f == NULL) {
		return;
	}
	f(node, deltaTime);
}

void S_ApplyFrameUpdates(A_Context* ctx, D_Scene *scene, float deltaTime)
{
	if (ctx == NULL) {
		return;
	}
	if (scene == NULL || scene->root == NULL) {
		return;
	}
	s_applyFrameUpdates(ctx, scene->root, deltaTime);
}
