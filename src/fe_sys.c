#include "fe_sys.h"

#include "fe_app.h"
#include "stb_ds.h"

void S_RenderSprites(A_Context* ctx, D_Node* node)
{
	if (node == NULL || ctx == NULL || ctx->renderer == NULL) {
		return;
	}
	for (int i = 0; i < stbds_arrlen(node->children); i++) {
		D_Node* child = node->children[i];
		S_RenderSprites(ctx, child);
	}
	D_Sprite* nodeSprite = NULL;
	SDL_FPoint position = { 0.0f, 0.0f };
	for (int i = 0; i < stbds_arrlen(node->components); i++) {
		D_Component* cmp = &node->components[i];

		if (cmp->type == S_COMPONENT_TYPE_SPRITE) {
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
		
		if (cmp->type == S_COMPONENT_TYPE_POSITION) {
			D_PositionComponent* posCmp = &cmp->position;
			position.x = posCmp->x;
			position.y = posCmp->y;
		}
	}
	SDL_FRect dst = {
		.x = position.x,
		.y = position.y,
		.w = nodeSprite->width,
		.h = nodeSprite->height
	};
	SDL_FRect src = {
		.x = 0,
		.y = 0,
		.w = nodeSprite->width,
		.h = nodeSprite->height
	};
	SDL_RenderTexture(ctx->renderer, (SDL_Texture*)nodeSprite->texture, &src, &dst);
}
