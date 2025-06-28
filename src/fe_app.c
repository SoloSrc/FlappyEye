#include <SDL3/SDL.h>

#include "fe_app.h"

static bool a_initSDL(void)
{
	if (!SDL_SetAppMetadata("Flappy Eye", "0.1.0", "com.github.solosrc.flappyeye")) {
		fprintf(stderr, "SDL_SetAppMetadata failed: %s\n", SDL_GetError());
		return false;
	}
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool A_Init()
{
	return a_initSDL();
}

void A_Quit(void)
{
	SDL_Quit();
}
