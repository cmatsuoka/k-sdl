
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"

#define NUM_SPRITES 14
#define SPRITE_BUFFER_SIZE 8000

static const char *sprite_filename[NUM_SPRITES] = {
	"ks0",
	"ks1",
	"ks2",
	"ks3",
	"ks4",
	"ksc",
	"ksi0",
	"ksi1",
	"ksi2",
	"ksj2",
	"ksi3",
	"ksi4",
	"ksj4",
	"ksi"		/* Broderbund logo */
};

static const char *mask_filename[NUM_SPRITES] = {
	"km0",
	"km1",
	"km2",
	"km3",
	"km4",
	"kmc",
	"kmi0",
	"kmi1",
	"kmi2",
	"kmj2",
	"kmi3",
	"kmi4",
	"kmj4",
	"kmi"		/* Broderbund logo */
};

static unsigned char sprite_buffer[SPRITE_BUFFER_SIZE];
static int sprite_array[256];
static int sprite_buffer_end;

static unsigned char mask_buffer[SPRITE_BUFFER_SIZE];
static int mask_array[256];
static int mask_buffer_end;


int read_sprite(int num)
{
	char filename[FILENAME_SIZE];
	FILE *f;
	int len;
	int i;

	/* Sanity check */
	if (num >= NUM_SPRITES)
		return -1;

	sprite_buffer_end = 0;
	mask_buffer_end = 0;

	/* Read sprite ind file */

	strncpy(filename, sprite_filename[num], FILENAME_SIZE);
	strncat(filename, ".ind", FILENAME_SIZE);

	if ((f = fopen_wrapper(filename, "rb")) == NULL) {
		return -1;
	}
	fread(sprite_buffer + sprite_buffer_end, 1, 680, f);
	fclose(f);

	for (i = 0; i < 680; i += 4) {
		int si = sprite_buffer_end + i;
		int index = sprite_buffer[si];

		len = readmem16l(sprite_buffer + si + 2);
		sprite_array[index] = sprite_buffer_end + len;

		D_(D_INFO "sprite_array[%d]=%d", index, sprite_array[index]);

		if (index == 255) {
			break;
		}
	}

	/* Read sprite dat file */

	strncpy(filename, sprite_filename[num], FILENAME_SIZE);
	strncat(filename, ".dat", FILENAME_SIZE);

	if ((f = fopen_wrapper(filename, "rb")) == NULL) {
		return -1;
	}

	D_(D_INFO "Read sprite data, %d bytes @ %d", len, sprite_buffer_end);

	fread(sprite_buffer + sprite_buffer_end, 1, len, f);
	fclose(f);

	if (sprite_buffer_end == 0) {
		sprite_buffer_end = len;
	}

	/* Read mask ind file */

	strncpy(filename, mask_filename[num], FILENAME_SIZE);
	strncat(filename, ".ind", FILENAME_SIZE);

	if ((f = fopen_wrapper(filename, "rb")) == NULL) {
		return -1;
	}
	fread(mask_buffer + mask_buffer_end, 1, 680, f);
	fclose(f);

	for (i = 0; i < 680; i += 4) {
		int si = mask_buffer_end + i;
		int index = mask_buffer[si];

		len = readmem16l(mask_buffer + si);
		mask_array[index] = mask_buffer_end + len;

		if (index == 255) {
			break;
		}
	}

	/* Read mask dat file */

	strncpy(filename, mask_filename[num], FILENAME_SIZE);
	strncat(filename, ".dat", FILENAME_SIZE);

	if ((f = fopen_wrapper(filename, "rb")) == NULL) {
		return -1;
	}
	fread(mask_buffer + mask_buffer_end, 1, len, f);
	fclose(f);

	if (mask_buffer_end == 0) {
		mask_buffer_end = len;
	}

	return 0;
}

static int sprite_rle_count;
static int mask_rle_count;
static int sprite_offset;
static int mask_offset;

static void get_next_pixel(unsigned char *s, unsigned char *m)
{
	static unsigned char sprite_rle_val = 0;
	static unsigned char mask_rle_val = 0;

	if (sprite_rle_count == 0) {
		unsigned char c = sprite_buffer[sprite_offset++];
		if (c == 0x7b) {
			sprite_rle_count = sprite_buffer[sprite_offset + 1];
			*s = sprite_rle_val = sprite_buffer[sprite_offset];
			sprite_offset += 2;
		} else {
			*s = c;
		}
	} else {
		*s = sprite_rle_val;
		sprite_rle_count--;
	}

	if (mask_rle_count == 0) {
		unsigned char c = mask_buffer[mask_offset++];
		if (c == 0x7b) {
			mask_rle_count = mask_buffer[mask_offset + 1];
			*m = mask_rle_val = mask_buffer[mask_offset];
			mask_offset += 2;
		} else {
			*m = c;
		}
	} else {
		*m = mask_rle_val;
		mask_rle_count--;
	}
}

void blit_sprite(int num, int x, int y, int pal_offset)
{
	int width, height;
	int offset;
	int i, j;
	unsigned char s, m;

	sprite_rle_count = 0;
	mask_rle_count = 0;

	sprite_offset = sprite_array[num];
	mask_offset = mask_array[num];
	width = sprite_buffer[sprite_offset];
	height = sprite_buffer[sprite_offset + 1];

	D_(D_INFO "sprite_offset=%d mask_offset=%d width=%d height=%d",
				sprite_offset, mask_offset, width, height);

	y -= height;
	offset = y * FB_WIDTH + x;
	sprite_offset += 3;
	mask_offset += 3;

	for (j = 0; j < width; j++) {
		for (i = 0; i < height; i++) {
			get_next_pixel(&s, &m);
			unpack_pixels(offset + j * 4 + i * FB_WIDTH, s, pal_offset);
		}
	}
}

