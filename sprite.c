
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "sprite.h"

#define NUM_SPRITES 13
#define SPRITE_BUFFER_SIZE 3000

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
	"kmi3",
	"kmi4",
	"kmj4",
	"kmi"		/* Broderbund logo */
};

static unsigned char sprite_buffer[SPRITE_BUFFER_SIZE];
static int sprite_end_array[256];
static int sprite_buffer_end = 0;

static unsigned char mask_buffer[SPRITE_BUFFER_SIZE];
static int mask_end_array[256];
static int mask_buffer_end = 0;

int read_sprite(int num)
{
	char filename[FILENAME_SIZE];
	FILE *f;
	int len;
	int i;

	/* Sanity check */
	if (num >= NUM_SPRITES)
		return -1;

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

		len = readmem16l(sprite_buffer + si);
		sprite_end_array[index] = sprite_buffer_end + len;

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
		mask_end_array[index] = mask_buffer_end + len;

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

void blit_sprite(int num, int x, int y)
{
}
