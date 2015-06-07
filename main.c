#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "graphics.h"

unsigned char *bytecode;

int main(int argc, char **argv)
{
	if ((bytecode = malloc(BYTECODE_SIZE)) == NULL) {
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}

	init_graphics();

	clear_screen();
	intro();

	deinit_graphics();

	free(bytecode);

	exit(EXIT_SUCCESS);
}
