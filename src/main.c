#include <stdlib.h>

#include "fe_app.h"

int main()
{
	atexit(A_Quit);
	if (!A_Init()) {
		exit(EXIT_FAILURE);
	}
	printf("Hello, World\n");
	return 0;
}
