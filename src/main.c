#include <stdlib.h>

#include "fe_app.h"
#include "fe_scn.h"

A_Context ctx;

static void quit(void)
{
	A_Quit(&ctx);
}

int main()
{
	atexit(quit);
	if (!A_Init(&ctx, "assets.hfd")) {
		exit(EXIT_FAILURE);
	}
	D_Sprite* sprite = A_LoadSpriteSheet(&ctx, "sprites/flappy.png", 1, 8);
	
	D_Node* root = D_InitNode("flappy");
	D_AddSpriteComponent(root, sprite);
	D_AddPositionComponent(root, -50.0f, -50.0f);

	D_Scene* scene = D_InitScene(root);

	A_Run(&ctx, scene);

	D_FreeScene(&scene);

	return 0;
}
