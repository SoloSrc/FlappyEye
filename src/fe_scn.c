#include <stdlib.h>

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

D_Node* D_InitNode(void)
{
	D_Node* node = malloc(sizeof(D_Node));
	if (node == NULL) {
		return NULL;
	}
	node->parent = NULL;
	node->children = NULL;
	node->components = NULL;
	return node;
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

void D_AddSpriteComponent(D_Node* node, char* path)
{
	if (node == NULL || path == NULL) {
		return;
	}
	D_SpriteComponent cmp = { .type = S_COMPONENT_TYPE_SPRITE, .path = path};
	D_Component unionCmp = { .sprite = cmp };
	stbds_arrput(node->components, unionCmp);
}
