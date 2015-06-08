#include <stdlib.h>
#include "common.h"
#include "graphics.h"


int main(int argc, char **argv)
{
	init_graphics();

	clear_screen();
	intro();

	deinit_graphics();

	exit(EXIT_SUCCESS);
}
