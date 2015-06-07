#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "graphics.h"

void usage(char *name)
{
	printf("Usage: %s <filename>\n", name);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	init_graphics();

	clear_screen();
	if (load_bcg(argv[1], 0) < 0) {
		perror(argv[0]);
	} else {
		show_screen();
		get_input();
	}

	deinit_graphics();	

	exit(EXIT_SUCCESS);
}
