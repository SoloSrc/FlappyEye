#include <stdlib.h>
#include <string.h>

#include "fe_scn.h"

#include "stb_ds.h"

static void d_removeFromParent(D_Node* parent, D_Node* node)
{
	if (node == NULL || parent == NULL) {
		return;
	}
	for (int i = 0; i < stbds_arrlen(parent->children); i++) {
		if (parent->children[i] == node) {
			stbds_arrdel(parent->children, i);
			break;
		}
	}
}

static void d_freeNodeRecursive(D_Node* node)
{
	if (node == NULL) {
		return;
	}
	stbds_arrfree(node->components);
	for (int i = 0; i < stbds_arrlen(node->children); i++) {
		d_freeNodeRecursive(node->children[i]);
	}
	stbds_arrfree(node->children);
	free(node);
}

static void d_freeScene(D_Scene* scene)
{
	if (scene == NULL) {
		return;
	}
	D_FreeNode(&(scene->root));
	D_FreeNode(&(scene->camera));
	free(scene);
}

D_Scene *D_InitScene(D_Node *root, D_Node* camera)
{
	D_Scene* scene = malloc(sizeof(D_Scene));
	if (scene == NULL) {
		return NULL;
	}
	scene->root = root;
	scene->camera = camera;
	return scene;
}

bool D_ValidateScene(D_Scene *scene)
{
	if (scene == NULL) {
		return false;
	}
	if (scene->root == NULL) {
		return false;
	}
	if (scene->camera == NULL) {
		return false;
	}
	D_Node* camera = scene->camera;
	D_CameraComponent* cameraCmp = NULL;
	for (int i = 0; i < stbds_arrlen(camera->components); i++) {
		D_Component* cmp = &camera->components[i];
		if (cmp->type == D_COMPONENT_TYPE_CAMERA) {
			cameraCmp = &cmp->camera;
			break;
		}
	}
	if (cameraCmp == NULL) {
		return false;
	}
	return true;
}

void D_FreeScene(D_Scene **scene)
{
	if (scene == NULL) {
		return;
	}
	d_freeScene(*scene);
	*scene = NULL;
}

D_Node *D_InitNode(const char *name)
{
	D_Node* node = malloc(sizeof(D_Node));
	if (node == NULL) {
		return NULL;
	}
	size_t nameLen = strlen(name) + 1 /* NULL char */;
	node->name = malloc(sizeof(char) * nameLen);
	if (node->name == NULL) {
		free(node);
		return NULL;
	}
	strncpy_s(node->name, nameLen, name, nameLen);
	node->parent = NULL;
	node->children = NULL;
	node->components = NULL;
	return node;
}

D_Node *D_InitCameraNode(const char *name)
{
	D_Node* camera = D_InitNode(name);
	D_AddPositionComponent(camera, 0, 0);
	D_CameraComponent cameraCmp = { .type = D_COMPONENT_TYPE_CAMERA, };
	D_Component unionCmp = { .camera = cameraCmp };
	stbds_arrput(camera->components, unionCmp);
	return camera;
}

void D_AttachChildNode(D_Node *parent, D_Node *child)
{
	if (parent == NULL || child == NULL) {
		return;
	}
	stbds_arrput(parent->children, child);
}

void D_FreeNode(D_Node** node)
{
	if (node == NULL || *node == NULL) {
		return;
	}
	D_Node* parent = (*node)->parent;
	d_freeNodeRecursive(*node);
	d_removeFromParent(parent, *node);
	*node = NULL;
}

void D_AddPositionComponent(D_Node* node, float x, float y)
{
	if (node == NULL) {
		return;
	}
	D_PositionComponent cmp = { .type = D_COMPONENT_TYPE_POSITION, .x = x, .y = y };
	D_Component unionCmp = { .position = cmp };
	stbds_arrput(node->components, unionCmp);
}

void D_AddSpriteComponent(D_Node* node, D_Sprite* sprite)
{
	if (node == NULL || sprite == NULL) {
		return;
	}
	D_SpriteComponent cmp = { .type = D_COMPONENT_TYPE_SPRITE, .sprite = sprite};
	D_Component unionCmp = { .sprite = cmp };
	stbds_arrput(node->components, unionCmp);
}

void D_AddAnimationComponent(D_Node* node, int framesPerSec)
{
	if (node == NULL || framesPerSec <= 1) {
		return;
	}
	float frameDuration = 1.0f / (float)framesPerSec;
	D_SpriteAnimationComponent cmp = {
		.type = D_COMPONENT_TYPE_SPRITE_ANIMATION,
		.frameDuration = frameDuration, .elapsedTime = 0.0f
	};
	D_Component unionCmp = { .animation = cmp };
	stbds_arrput(node->components, unionCmp);
}

void D_AddVelocityComponent(D_Node* node, float x, float y)
{
	if (node == NULL) {
		return;
	}
	D_VelocityComponent cmp = { .type = D_COMPONENT_TYPE_VELOCITY, .x = x, .y = y };
	D_Component unionCmp = { .velocity = cmp };
	stbds_arrput(node->components, unionCmp);
}

D_VelocityComponent* D_GetVelocityComponent(D_Node* node)
{
	if (node == NULL) {
		return NULL;
	}
	for (int i = 0; i < stbds_arrlen(node->components); i++) {
		D_Component* cmp = &node->components[i];
		if (cmp != NULL && cmp->type == D_COMPONENT_TYPE_VELOCITY) {
			D_VelocityComponent* velCmp = &cmp->velocity;
			return velCmp;
		}
	}
	return NULL;
}
