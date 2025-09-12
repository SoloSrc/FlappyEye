// fe_app.h
//
// Header file for anything that is application related. For example,
// initializing SDL, setting up the Window. Running the main loop, etc.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL3/SDL.h>
#include <zip.h>

#include "fe_scn.h"

#ifndef FE_APP_H
#define FE_APP_H

#define STBDS_NO_SHORT_NAMES

typedef struct A_SpriteKV {
	char* key; // Path to the sprite asset
	D_Sprite* value; // Pointer to the loaded sprite
} A_SpriteKV;

typedef struct A_UpdateCallbackKV {
	uintptr_t key;
	D_FrameUpdate value;
} A_UpdateCallbackKV;

typedef struct A_Context {
	SDL_Window* window;
	SDL_Renderer* renderer;
	zip_t* assets;
	A_SpriteKV* sprites; // HashMap of loaded sprites, managed by stb_ds
	A_UpdateCallbackKV* updateCallbacks; // Array of callbacks, managed by stb_ds
} A_Context;

bool A_Init(A_Context* ctx, const char* name);
void A_Quit(A_Context* ctx);
void A_Run(A_Context* ctx, D_Scene *scene);
void A_RegisterUpdateCallback(A_Context *ctx, D_Node* node, D_FrameUpdate callback);

D_Sprite* A_LoadSprite(A_Context* ctx, const char* path);
D_Sprite* A_LoadSpriteSheet(A_Context* ctx, const char* path, int rows, int cols);

#endif // FE_APP_H
