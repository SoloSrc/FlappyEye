#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

static bool A_Init()
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

static void A_Quit(void)
{
    SDL_Quit();
}

int main()
{
	atexit(A_Quit);
    if (!A_Init()) {
		exit(EXIT_FAILURE);
    }
    printf("Hello, World\n");
    return 0;
}
