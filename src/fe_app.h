// fe_app.h
//
// Header file for anything that is application related. For example,
// initializing SDL, setting up the Window. Running the main loop, etc.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

typedef struct A_Context {
	SDL_Window* window;
	SDL_Renderer* renderer;
	zip_t* assets;
	A_SpriteKV* sprites; // HashMap of loaded sprites, managed by stb_ds
} A_Context;

bool A_Init(A_Context*, const char*);
void A_Quit(A_Context*);
void A_Run(A_Context*, D_Node *node);

D_Sprite* A_LoadSprite(A_Context*, const char*);
D_Sprite* A_LoadSpriteSheet(A_Context* ctx, const char* path, int rows, int cols);

#endif // FE_APP_H
