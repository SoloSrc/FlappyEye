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

static bool a_createWindow(A_Context* ctx)
{
	if (!SDL_CreateWindowAndRenderer("Flappy Eye", 800, 600, 0, &ctx->window, &ctx->renderer)) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Creation of Window and Renderer failed: %s",
			SDL_GetError());
		return false;
	}
	return true;
}

bool A_Init(A_Context* ctx)
{
	if (!a_initSDL()) {
		return false;
	}
	if (!a_createWindow(ctx)) {
		return false;
	}
	return true;
}

void A_Quit(A_Context* ctx)
{
	SDL_DestroyRenderer(ctx->renderer);
	ctx->renderer = NULL;
	SDL_DestroyWindow(ctx->window);
	ctx->window = NULL;
	SDL_Quit();
}

void A_Run(A_Context* ctx)
{
	SDL_Event event;
	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_EVENT_QUIT) {
			break;
		}

		SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(ctx->renderer);
		SDL_RenderPresent(ctx->renderer);
	}
}
