
#include <stdlib.h>
#include "graphics.h"

unsigned char *framebuffer;

extern struct gfx_driver gfx_sdl;
static struct gfx_driver *gfx = &gfx_sdl;

struct update_block {
	int x1, y1;
	int x2, y2;
};


static struct update_block update = {
	FB_WIDTH, FB_HEIGHT, 0, 0
};


unsigned char palette[NUM_COLORS * 3]= {
	0x00, 0x00, 0x00,
	0x00, 0x80, 0xf0,
	0xf0, 0x50, 0x00,
	0xf0, 0xf0, 0xf0
};

int init_graphics()
{
	if ((framebuffer = malloc(FB_WIDTH * (FB_HEIGHT + 12))) == NULL)
		goto err;


	if (gfx->init_video_mode() < 0) 
		goto err2;

	return 0;

    err2:
	free(framebuffer);
    err:
	return -1;
}

void deinit_graphics()
{
	gfx->deinit_video_mode();
	free(framebuffer);
}

void put_block(int x1, int y1, int x2, int y2)
{
	gfx->put_block(x1, y1, x2, y2);
}

void put_screen()
{
	put_block(0, 0, FB_WIDTH - 1, FB_HEIGHT - 1);
}


void poll_timer()
{
	gfx->poll_timer();
}


int get_key()
{
	return gfx->get_key();
}


int keypress()
{
	return gfx->keypress();
}


void schedule_update(int x1, int y1, int x2, int y2)
{
	if (x1 < update.x1) update.x1 = x1;
	if (y1 < update.y1) update.y1 = y1;
	if (x2 > update.x2) update.x2 = x2;
	if (y2 > update.y2) update.y2 = y2;
}

void do_update()
{
	if (update.x1 <= update.x2 && update.y1 <= update.y2) {
		gfx->put_block(update.x1, update.y1, update.x2, update.y2);
	}
	
	/* reset update block variables */
	update.x1 = FB_WIDTH;
	update.y1 = FB_HEIGHT;
	update.x2 = 0;
	update.y2 = 0;
}


void flush_block(int x1, int y1, int x2, int y2)
{
	int y, w;
	unsigned char *p0;

	schedule_update(x1, y1, x2, y2);

	p0 = &framebuffer[x1 + y1 * FB_WIDTH];
	w = x2 - x1 + 1;

	for (y = y1; y <= y2; y++) {
		gfx->put_pixels(x1, y, w, p0);
		p0 += FB_WIDTH;
	}
}

void flush_screen()
{
	flush_block(0, 0, FB_WIDTH - 1, FB_HEIGHT - 1);
}


void unpack_pixels(int offset, unsigned char pixels)
{
	int i;

	for (i = 0; i < 4; i++) {
		framebuffer[offset + i] = ((pixels & 0xc0) >> 6)/*+pal_offset*/;
		pixels <<= 2;
	}
}

