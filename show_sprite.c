#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "graphics.h"

void usage(char *name)
{
	printf("Usage: %s <indfile>\n", name);
}

int main(int argc, char **argv)
{
	int index, height, len;
	FILE *f;

	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((f = fopen(argv[1], "rb")) == NULL) {
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}

	do {
		index = read8(f);
		height = read8(f);
		len = read16l(f);

		printf("%3d (%#02x)  %3d  %5d\n", index, index, height, len);
	} while (index != 255);


/*
	init_graphics();

	clear_screen();
	if (load_bcg(argv[1], 0, 0) < 0) {
		perror(argv[0]);
	} else {
		show_screen();
		get_input();
	}

	deinit_graphics();	
*/

	exit(EXIT_SUCCESS);
}
