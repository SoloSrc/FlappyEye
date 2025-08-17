// fe_app.h
//
// Header file for anything that is application related. For example,
// initializing SDL, setting up the Window. Running the main loop, etc.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <zip.h>

#ifndef FE_APP_H
#define FE_APP_H

#define STBDS_NO_SHORT_NAMES

typedef struct A_Sprite {
	SDL_Texture* texture;
	float width;
	float height;
	int frames; // Number of frames in the sprite sheet
	int cols; // Number of columns in the sprite sheet
} A_Sprite;

typedef struct A_Context {
	SDL_Window* window;
	SDL_Renderer* renderer;
	zip_t* assets;
	A_Sprite** sprites; // Array of loaded sprites, managed by stb_ds
} A_Context;

bool A_Init(A_Context*, const char*);
void A_Quit(A_Context*);
void A_Run(A_Context*);

A_Sprite* A_LoadSprite(A_Context*, const char*);
A_Sprite* A_LoadSpriteSheet(A_Context* ctx, const char* path, int rows, int cols);

#endif // FE_APP_H
