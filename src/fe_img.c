#include <SDL3_image/SDL_image.h>
#include "stb_ds.h"

#include "fe_app.h"

#define ZIP_HAS_FLAG(fs,f) ((fs & f) == f)
#define COPY_BUFFER_SIZE 256

typedef struct a_assetStat {
	uint64_t total;
	uint64_t index;
} a_assetStat;

static bool a_loadAssetStat(A_Context* ctx, const char* path, a_assetStat* stat)
{
	stat->total = 0;
	stat->index = 0;
	zip_stat_t zstat;

	if (zip_stat(ctx->assets, path, ZIP_FL_ENC_UTF_8, &zstat) != 0) {
		zip_error_t* err = zip_get_error(ctx->assets);
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"zip_stat failed for %s: %s",
			path, zip_error_strerror(err));
		zip_error_fini(err);
		return false;
	}

	if (!ZIP_HAS_FLAG(zstat.valid, ZIP_STAT_SIZE) || !ZIP_HAS_FLAG(zstat.valid, ZIP_STAT_INDEX)) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Cannot get the size of asset: %s",
			path
		);
		return false;
	}

	stat->total = zstat.size;
	stat->index = zstat.index;
	return true;
}

static bool a_copyBytesToStream(A_Context* ctx, SDL_IOStream* io, a_assetStat* stat)
{
	uint64_t copiedBytes = 0;
	zip_file_t* zf = zip_fopen_index(ctx->assets, stat->index, ZIP_FL_ENC_UTF_8);
	if (zf == NULL) {
		return 1;
	}
	while (copiedBytes < stat->total) {
		char buffer[COPY_BUFFER_SIZE];
		zip_int64_t rlength = zip_fread(zf, buffer, COPY_BUFFER_SIZE);
		if (rlength < 0) {
			zip_fclose(zf);
			return false;
		}
		zip_int64_t wlength = SDL_WriteIO(io, buffer, (size_t)rlength);
		if (wlength != rlength) {
			zip_fclose(zf);
			return false;
		}
		copiedBytes += wlength;
	}
	zip_fclose(zf);
	return true;
}

A_Sprite A_LoadSprite(A_Context* ctx, const char* path)
{
	// load file statistics
	a_assetStat stat;
	if (!a_loadAssetStat(ctx, path, &stat) != 0) {
		return NULL;
	}

	// create a io stream in memory
	void* buffer = malloc(sizeof(char) * stat.total);
	if (buffer == NULL) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Cannot allocate memory for asset: %s",
			path
		);
		return NULL;
	}
	SDL_IOStream* io = SDL_IOFromMem(buffer, (size_t)stat.total);
	if (io == NULL) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Cannot create IOStream for asset %s. Error: %s",
			path, SDL_GetError()
		);
		free(buffer);
		return NULL;
	}

	// copy bytes from zip to the io stream
	if (!a_copyBytesToStream(ctx, io, &stat)) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Cannot copy bytes from zip to IOStream for asset %s",
			path
		);
		SDL_CloseIO(io);
		free(buffer);
		return NULL;
	}

	// move back to the beginning of the stream
	if (SDL_SeekIO(io, 0, SDL_IO_SEEK_SET) < 0) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Cannot seek to begining (asset %s): %s",
			path, SDL_GetError()
		);
		SDL_CloseIO(io);
		free(buffer);
		return NULL;
	}

	// load the image into a texture
	SDL_Texture* texture = IMG_LoadTexture_IO(ctx->renderer, io, false);
	SDL_CloseIO(io);
	free(buffer);
	if (texture == NULL) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"Cannot load image from IOStream for asset %s: %s",
			path, SDL_GetError()
		);
		return NULL;
	}
	stbds_arrput(ctx->sprites, texture);
	return texture;
}