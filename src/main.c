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
	
	D_Node* flappy = D_InitNode("flappy");
	D_AddPositionComponent(flappy, -50.0f, -50.0f);
	
	D_Node* spriteNode = D_InitNode("sprite");
	D_AddPositionComponent(spriteNode, -80, 85);
	D_AddSpriteComponent(spriteNode, sprite);

	D_AttachChildNode(flappy, spriteNode);

	D_Node *camera = D_InitCameraNode("camera");

	D_Scene* scene = D_InitScene(flappy, camera);

	A_Run(&ctx, scene);

	D_FreeScene(&scene);

	return 0;
}
