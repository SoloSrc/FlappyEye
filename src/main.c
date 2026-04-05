#include <stdlib.h>
#include <time.h>

#include "fe_app.h"
#include "fe_inp.h"
#include "fe_scn.h"

#define NUM_CLOUDS 6
#define CLOUD_Y_MAX  280.0f
#define CLOUD_Y_MIN   20.0f
#define CLOUD_X_MIN -384.0f
#define CLOUD_X_MAX  384.0f

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

static float cloudYCoord()
{
	return ((float)rand() / (float)RAND_MAX) * (CLOUD_Y_MAX - CLOUD_Y_MIN) + CLOUD_Y_MIN;
}

static float cloudXCoord(D_Sprite* sprite)
{
	float randX = ((float)rand() / (float)RAND_MAX) * (CLOUD_X_MAX - CLOUD_X_MIN) + CLOUD_X_MIN;
	return randX + (sprite->width / 2.0f);
}

static void flappyUpdate(D_Node* node, float deltaTime)
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

static void cloudUpdate(D_Node* node, float deltaTime)
{
	(void)node;
	(void)deltaTime;
	D_PositionComponent* pos = D_GetPositionComponent(node);
	if (pos == NULL) {
		return;
	}
	if (pos->x <= 384.0f) {
		return;
	}
	D_SpriteComponent* spriteCmp = D_GetSpriteComponent(node);
	if (spriteCmp == NULL) {
		return;
	}
	pos->x = CLOUD_X_MIN - spriteCmp->sprite->width;
	pos->y = cloudYCoord();
}

int main()
{
	srand(time(NULL));
	atexit(quit);
	if (!A_Init(&ctx, "assets.hfd")) {
		exit(EXIT_FAILURE);
	}

	D_Sprite* flappySprite = A_LoadSpriteSheet(&ctx, "sprites/flappy.png", 1, 8);
	D_Sprite* mountains4Sprite = A_LoadSpriteSheet(&ctx, "background/mountains_4.png", 1, 1);
	D_Sprite* mountains1Sprite = A_LoadSpriteSheet(&ctx, "background/mountains_1.png", 1, 1);
	D_Sprite* mountains2Sprite = A_LoadSpriteSheet(&ctx, "background/mountains_2.png", 1, 1);
	D_Sprite* mountains3Sprite = A_LoadSpriteSheet(&ctx, "background/mountains_3.png", 1, 1);
	D_Sprite* skySprite = A_LoadSpriteSheet(&ctx, "background/sky.png", 1, 1);

	D_Sprite* clouds[NUM_CLOUDS] = {
		A_LoadSpriteSheet(&ctx, "background/cloud_1.png", 1, 1),
		A_LoadSpriteSheet(&ctx, "background/cloud_2.png", 1, 1),
		A_LoadSpriteSheet(&ctx, "background/cloud_3.png", 1, 1),
		A_LoadSpriteSheet(&ctx, "background/cloud_4.png", 1, 1),
		A_LoadSpriteSheet(&ctx, "background/cloud_5.png", 1, 1),
		A_LoadSpriteSheet(&ctx, "background/cloud_6.png", 1, 1),
	};

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
	A_RegisterUpdateCallback(&ctx, flappy, flappyUpdate);
	
	D_Node* spriteNode = D_InitNode("sprite");
	D_AddPositionComponent(spriteNode, -80, 85);
	D_AddAnimationComponent(spriteNode, 10);
	D_AddSpriteComponent(spriteNode, flappySprite);
	D_AttachChildNode(flappy, spriteNode);

	D_Node* background1 = D_InitNode("background1");
	D_AddPositionComponent(background1, -384.0f, 110.0f);
	D_AddSpriteComponent(background1, mountains4Sprite);

	D_Node* background2 = D_InitNode("background2");
	D_AddPositionComponent(background2, -384.0f, -30.0f);
	D_AddSpriteComponent(background2, mountains1Sprite);

	D_Node* background3 = D_InitNode("background3");
	D_AddPositionComponent(background3, -384.0f, -90.0f);
	D_AddSpriteComponent(background3, mountains2Sprite);

	D_Node* background4 = D_InitNode("background4");
	D_AddPositionComponent(background4, -384.0f, -140.0f);
	D_AddSpriteComponent(background4, mountains3Sprite);

	D_Node* skyNode = D_InitNode("sky");
	D_AddPositionComponent(skyNode, -384.0f, 290.0f);
	D_AddSpriteComponent(skyNode, skySprite);

	D_Node* level = D_InitNode("level");
	D_AddPositionComponent(level, 0.0f, 0.0f);
	D_AttachChildNode(level, skyNode);
	D_AttachChildNode(level, background1);
	D_AttachChildNode(level, background2);
	D_AttachChildNode(level, background3);
	D_AttachChildNode(level, background4);

	for (int i = 0; i < NUM_CLOUDS; i++) {
		char name[10];
		sprintf(name, "cloud_%d", i+ 1);
		D_Node* cloud = D_InitNode(name);
		D_Sprite* cloudSprite = clouds[i];
		D_AddPositionComponent(cloud, cloudXCoord(cloudSprite), cloudYCoord());
		D_AddSpriteComponent(cloud, cloudSprite);
		D_AddVelocityComponent(cloud, 50.0f, 0.0f);
		D_AttachChildNode(level, cloud);
		A_RegisterUpdateCallback(&ctx, cloud, cloudUpdate);
	}
	
	D_AttachChildNode(level, flappy);

	D_Node *camera = D_InitCameraNode("camera");

	D_Scene* scene = D_InitScene(level, camera);

	A_Run(&ctx, scene);

	D_FreeScene(&scene);

	// TODO: find better place for this
	free(flappySprite);
	free(mountains1Sprite);
	free(mountains2Sprite);
	free(mountains3Sprite);
	free(mountains4Sprite);
	free(skySprite);
	for (int i = 0; i < NUM_CLOUDS; i++) {
		free(clouds[i]);
	}

	return 0;
}
