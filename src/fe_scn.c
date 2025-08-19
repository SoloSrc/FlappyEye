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

void D_InitScene(D_Scene* scene)
{
	scene->root = NULL;
}

void D_FreeScene(D_Scene** scene)
{
	if (scene == NULL || *scene == NULL) {
		return;
	}
	D_FreeNode(&(*scene)->root);
	free(*scene);
	*scene = NULL;
}

void D_InitNode(D_Node* node)
{
	node->parent = NULL;
	node->children = NULL;
	node->components = NULL;
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
