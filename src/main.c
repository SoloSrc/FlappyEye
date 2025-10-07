#include <stdlib.h>

#include "fe_app.h"
#include "fe_inp.h"
#include "fe_scn.h"

A_Context ctx;
A_InputActionID jumpInputAction;

static void quit(void)
{
	A_Quit(&ctx);
}

static void flappy_update(D_Node* node, float deltaTime)
{
	D_VelocityComponent* velCmp = D_GetVelocityComponent(node);
	if (velCmp == NULL) {
		return;
	}
	velCmp->y += -60.0f * deltaTime;

	if (A_IsActionPressed(&ctx, jumpInputAction)) {
		velCmp->y = 100.0f;
	}
}

int main()
{
	atexit(quit);
	if (!A_Init(&ctx, "assets.hfd")) {
		exit(EXIT_FAILURE);
	}
	D_Sprite* sprite = A_LoadSpriteSheet(&ctx, "sprites/flappy.png", 1, 8);
	A_Input spaceInput = A_INPUT_SPACE_PRESSED;
	jumpInputAction = A_CreateInputAction(&ctx);
	A_AssociateInputToAction(&ctx, jumpInputAction, spaceInput);
	
	D_Node* flappy = D_InitNode("flappy");
	D_AddPositionComponent(flappy, -50.0f, -50.0f);
	D_AddVelocityComponent(flappy, 0.0, 0.0);
	A_RegisterUpdateCallback(&ctx, flappy, flappy_update);
	
	D_Node* spriteNode = D_InitNode("sprite");
	D_AddPositionComponent(spriteNode, -80, 85);
	D_AddAnimationComponent(spriteNode, 10);
	D_AddSpriteComponent(spriteNode, sprite);

	D_AttachChildNode(flappy, spriteNode);

	D_Node *camera = D_InitCameraNode("camera");

	D_Scene* scene = D_InitScene(flappy, camera);

	A_Run(&ctx, scene);

	D_FreeScene(&scene);

	return 0;
}
