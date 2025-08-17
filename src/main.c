#include <stdlib.h>

#include "fe_app.h"

A_Context ctx;

static void quit(void)
{
	A_Quit(&ctx);
}

int main()
{
	atexit(quit);
	if (!A_Init(&ctx, "assets.hfd")) {
		exit(EXIT_FAILURE);
	}
	A_LoadSpriteSheet(&ctx, "sprites/flappy.png", 1, 8);
	A_Run(&ctx);
	return 0;
}
