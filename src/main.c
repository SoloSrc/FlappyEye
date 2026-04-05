#include <stdlib.h>

#include "fe_app.h"
#include "fe_inp.h"
#include "fe_scn.h"

A_Context ctx;

A_InputActionID moveLeftAction;
A_InputActionID moveRightAction;
A_InputActionID moveUpAction;
A_InputActionID moveDownAction;

const float SPEED = 300.0f;

static void quit(void)
{
	A_Quit(&ctx);
}

static void flappy_update(D_Node* node, float deltaTime)
{
	(void)deltaTime;
	D_VelocityComponent* velCmp = D_GetVelocityComponent(node);
	if (velCmp == NULL) {
		return;
	}

	D_Vector2 inputVec = A_GetInputVector(&ctx,
		moveUpAction, moveDownAction, moveLeftAction, moveRightAction);

	velCmp->x = inputVec.x * SPEED;
	velCmp->y = inputVec.y * SPEED;
}

int main()
{
	atexit(quit);
	if (!A_Init(&ctx, "assets.hfd")) {
		exit(EXIT_FAILURE);
	}
	D_Sprite* flappySprite = A_LoadSpriteSheet(&ctx, "sprites/flappy.png", 1, 8);
	D_Sprite* mountains4Sprite = A_LoadSpriteSheet(&ctx, "background/mountains_4.png", 1, 1);

	moveUpAction = A_CreateInputAction(&ctx);
	A_AssociateInputToAction(&ctx, moveUpAction, A_INPUT_UP_PRESSED);
	moveDownAction = A_CreateInputAction(&ctx);
	A_AssociateInputToAction(&ctx, moveDownAction, A_INPUT_DOWN_PRESSED);
	moveLeftAction = A_CreateInputAction(&ctx);
	A_AssociateInputToAction(&ctx, moveLeftAction, A_INPUT_LEFT_PRESSED);
	moveRightAction = A_CreateInputAction(&ctx);
	A_AssociateInputToAction(&ctx, moveRightAction, A_INPUT_RIGHT_PRESSED);
	
	D_Node* flappy = D_InitNode("flappy");
	D_AddPositionComponent(flappy, -50.0f, -50.0f);
	D_AddVelocityComponent(flappy, 0.0, 0.0);
	A_RegisterUpdateCallback(&ctx, flappy, flappy_update);
	
	D_Node* spriteNode = D_InitNode("sprite");
	D_AddPositionComponent(spriteNode, -80, 85);
	D_AddAnimationComponent(spriteNode, 10);
	D_AddSpriteComponent(spriteNode, flappySprite);
	D_AttachChildNode(flappy, spriteNode);

	D_Node* background = D_InitNode("background");
	D_AddPositionComponent(background, -384.0f, 100.0f);
	D_AddSpriteComponent(background, mountains4Sprite);

	D_Node* level = D_InitNode("level");
	D_AddPositionComponent(level, 0.0f, 0.0f);
	D_AttachChildNode(level, background);
	D_AttachChildNode(level, flappy);

	D_Node *camera = D_InitCameraNode("camera");

	D_Scene* scene = D_InitScene(level, camera);

	A_Run(&ctx, scene);

	D_FreeScene(&scene);

	// TODO: find better place for this
	free(flappySprite);
	free(mountains4Sprite);

	return 0;
}
