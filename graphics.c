
#include <stdlib.h>
#include "graphics.h"

static unsigned char *framebuffer;

extern struct gfx_driver gfx_sdl;
static struct gfx_driver *gfx = &gfx_sdl;


unsigned char palette[NUM_COLORS * 3]= {
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x3f,
	0x00, 0x2a, 0x00,
	0x00, 0x2a, 0x2a,
	0x33, 0x00, 0x00,
	0x2f, 0x1c, 0x37,
	0x23, 0x14, 0x00,
	0x2f, 0x2f, 0x2f,
	0x15, 0x15, 0x15,
	0x00, 0x2f, 0x3f,
	0x00, 0x33, 0x15,
	0x15, 0x3f, 0x3f,
	0x3f, 0x27, 0x23,
	0x3f, 0x15, 0x3f,
	0x3b, 0x3b, 0x00,
	0x3f, 0x3f, 0x3f
};

int init_graphics()
{
	if ((framebuffer = malloc(FB_WIDTH * FB_HEIGHT)) == NULL)
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
