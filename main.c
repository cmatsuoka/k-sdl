#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "graphics.h"

int main(int argc, char **argv)
{
	init_graphics();

	clear_screen();
	load_bcg("castle.bcg", 0, 0);
	flush_screen();
	do_update();

	deinit_graphics();	

	exit(EXIT_SUCCESS);
}
