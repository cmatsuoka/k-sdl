
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "graphics.h"

void clear_screen()
{
	memset(framebuffer, 0, FB_WIDTH * FB_HEIGHT);
}

int load_bcg(char *filename, int offset, int pal_offset)
{
	FILE *f;
	int len;
	int i, j;

	if ((f = fopen(filename, "rb")) == NULL)
		return -1;

	len = read16l(f);

	for (i = 0; i < len; i++) {
		int c = fgetc(f);
		for (j = 0; j < 4; j++) {
			framebuffer[offset + i * 4 + j] =
						((c & 0xc0) >> 6) + pal_offset;
			c <<= 2;
		}
	}

	fclose(f);

	return 0;
}
