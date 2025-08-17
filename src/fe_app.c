#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

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

static bool a_initAssets(A_Context* ctx, const char* filepath)
{
	int err;
	zip_t* za;
	if ((za = zip_open(filepath, ZIP_RDONLY, &err)) == NULL) {
		zip_error_t error;
		zip_error_init_with_code(&error, err);
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"unzip %s: %s",
			filepath,
			zip_error_strerror(&error)
		);
		zip_error_fini(&error);
		return false;
	}
	ctx->assets = za;
	return true;
}

bool A_Init(A_Context* ctx, const char* filepath)
{
	// clean ups
	ctx->assets = NULL;
	ctx->window = NULL;
	ctx->renderer = NULL;
	ctx->sprites = NULL;

	// inits
	if (!a_initSDL()) {
		return false;
	}
	if (!a_initAssets(ctx, filepath)) {
		return false;
	}
	if (!a_createWindow(ctx)) {
		return false;
	}

	return true;
}

void A_Quit(A_Context* ctx)
{
	if (ctx->sprites != NULL) {
		for (int i = 0; i < stbds_arrlen(ctx->sprites); i++) {
			if (ctx->sprites[i] != NULL) {
				SDL_DestroyTexture(ctx->sprites[i]->texture);
				free(ctx->sprites[i]);
			}
		}
		stbds_arrfree(ctx->sprites);
		ctx->sprites = NULL;
	}
	if (ctx->assets != NULL) {
		zip_close(ctx->assets);
	}
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

		A_Sprite* sprite = ctx->sprites[0];
		SDL_FRect dst = { .x = 100, .y = 100, .w = sprite->width, .h = sprite->height };
		SDL_FRect src = { .x = 0, .y = 0, .w = sprite->width, .h = sprite->height };
		SDL_RenderTexture(ctx->renderer, sprite->texture, &src, &dst);

		SDL_RenderPresent(ctx->renderer);
	}
}
