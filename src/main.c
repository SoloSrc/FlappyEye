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
	A_LoadSprite(&ctx, "sprites/flappy.png");
	A_Run(&ctx);
	return 0;
}
