
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#define FB_WIDTH 320
#define FB_HEIGHT 200
#define NUM_COLORS 16


struct gfx_driver {
	int	(*init_video_mode)(void);
	int	(*deinit_video_mode)(void);
	void	(*put_block)(int x1, int y1, int x2, int y2);
	void	(*put_pixels)(int x, int y, int w, unsigned char *p);
	void	(*poll_timer)(void);
	int	(*keypress)(void);
	int	(*get_key)(void);
};

extern unsigned char *framebuffer;

int init_graphics(void);
void deinit_graphics(void);
void do_update(void);
void poll_timer(void);
int get_key(void);
int keypress(void);
void flush_block(int, int, int, int);
void flush_screen(void);

#endif
