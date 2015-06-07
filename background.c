
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "graphics.h"

void clear_screen()
{
	memset(framebuffer, 0, FB_WIDTH * FB_HEIGHT);
}

int load_bcg(char *filename, int offset)
{
	FILE *f;
	int len;
	int i;

	if ((f = fopen_wrapper(filename, "rb")) == NULL)
		return -1;

	offset <<= 2;
	len = read16l(f);

	for (i = 0; i < len; i++) {
		int c = fgetc(f);
		unpack_pixels(offset + i * 4, c);
	}

	fclose(f);

	return 0;
}
