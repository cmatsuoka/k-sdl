#include <SDL/SDL.h>
#include "graphics.h"

static SDL_Surface *screen;

#define TITLE "Karateka"
#define KEY_QUEUE_SIZE 16

static SDL_Color color[NUM_COLORS];
static Uint32 mapped_color[NUM_COLORS];
static int key_queue[KEY_QUEUE_SIZE];
static int key_queue_start = 0;
static int key_queue_end = 0;
static const int scale = 2;

extern unsigned char palette[NUM_COLORS * 3];

#define key_enqueue(k) do { key_queue[key_queue_end++] =(k); \
	key_queue_end %= KEY_QUEUE_SIZE; } while(0)
#define key_dequeue(k) do {(k) = key_queue[key_queue_start++]; \
	key_queue_start %= KEY_QUEUE_SIZE; } while(0)

static int init_vidmode(void);
static int deinit_vidmode(void);
static void sdl_put_block(int, int, int, int);
static void putpixels(int, int, int, Uint8 *);
static void sdl_timer(void);
static Uint32 timer_function(Uint32);
int sdl_is_keypress(void);
int sdl_get_keypress(void);

struct gfx_driver gfx_sdl = {
	init_vidmode,
	deinit_vidmode,
	sdl_put_block,
	NULL,
	sdl_timer,
	sdl_is_keypress,
	sdl_get_keypress
};


static volatile Uint32 tick_timer = 0;

#define TICK_SECONDS 20

/* ====================================================================*/

/* Some optimized put_pixel routines for the most common cases */

#define _putpixels_scale1(d) static void inline				\
_putpixels_##d##bits_scale1(int x, int y, int w, Uint8 *p) {		\
	Uint##d *s;							\
	if (w == 0) return;						\
	s =(Uint##d *)screen->pixels + x + y * screen->w;		\
	if (SDL_MUSTLOCK(screen)) {					\
		if (SDL_LockSurface(screen) < 0)			\
			return;						\
	}								\
	while(w--) { *s++ = mapped_color[*p++]; }			\
	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);		\
}

#define _putpixels_scale2(d) static void inline				\
_putpixels_##d##bits_scale2(int x, int y, int w, Uint8 *p) {		\
	Uint##d *s, *t;							\
	if (w == 0) return;						\
	x <<= 1; y <<= 1;						\
	s =(Uint##d *)screen->pixels + x + y * screen->w;		\
	t = s + screen->w;						\
	if (SDL_MUSTLOCK(screen)) {					\
		if (SDL_LockSurface(screen) < 0)			\
			return;						\
	}								\
	while(w--) {							\
        	int c = mapped_color[*p];				\
              	*s++ = c; *s++ = c; *t++ = c; *t++ = c; p++;		\
	}								\
	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);		\
}

_putpixels_scale1(8);
_putpixels_scale1(16);
_putpixels_scale1(32);
_putpixels_scale2(8);
_putpixels_scale2(16);
_putpixels_scale2(32);

/* ====================================================================*/

/* Slow, non-optimized put pixel routine */

static void inline _put_pixel(int x, int y, int c)
{
	Uint32 pixel;
	Uint8 *bits, bpp;

	pixel = mapped_color[c];

	bpp = screen->format->BytesPerPixel;
	bits = ((Uint8 *) screen->pixels) + y * screen->pitch + x * bpp;

	/* Set the pixel */
	switch (bpp) {
	case 1:
		*(Uint8 *) (bits) = pixel;
		break;
	case 2:
		*((Uint16 *) (bits)) = (Uint16) pixel;
		break;
	case 3:{
			Uint8 r, g, b;
			r = (pixel >> screen->format->Rshift) & 0xff;
			g = (pixel >> screen->format->Gshift) & 0xff;
			b = (pixel >> screen->format->Bshift) & 0xff;
			*((bits) + screen->format->Rshift / 8) = r;
			*((bits) + screen->format->Gshift / 8) = g;
			*((bits) + screen->format->Bshift / 8) = b;
		}
		break;
	case 4:
		*((Uint32 *) (bits)) = (Uint32) pixel;
		break;
	}
}

static void _putpixels(int x, int y, int w, Uint8 * p)
{
	register int c;
	register int i, j;

	if (w == 0) {
		return;
	}

	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0)
			return;
	}

	switch (scale) {
	case 1:
		while (w--) {
			_put_pixel(x++, y, *p++);
		}
		break;
	case 2:
		x <<= 1;
		y <<= 1;
		while (w--) {
			c = *p++;
			_put_pixel(x, y, c);
			_put_pixel(x++, y + 1, c);
			_put_pixel(x, y, c);
			_put_pixel(x++, y + 1, c);
		}
		break;
	default:
		x *= scale;
		y *= scale;
		while (w--) {
			c = *p++;
			for (i = 0; i < scale; i++) {
				for (j = 0; j < scale; j++)
					_put_pixel(x + i, y + j, c);
			}
			x += scale;
		}
		break;
	}

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
}

/* ====================================================================*/

static void process_events()
{
	SDL_Event event;
	int key = 0;

	while (SDL_PollEvent(&event) > 0) {
		switch (event.type) {
		case SDL_QUIT:
			deinit_vidmode();
			exit(0);
			break;
		case SDL_KEYDOWN:
			if (isalpha(key)) {
				key_enqueue(key);
			}
			break;
		case SDL_KEYUP:
			break;
		}
	}
}

static int init_vidmode()
{
	int i, mode;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "sdl: can't initialize: %s\n", SDL_GetError());
		return -1;
	}

	mode = SDL_SWSURFACE | SDL_ANYFORMAT;

	if ((screen = SDL_SetVideoMode(320 * scale, FB_HEIGHT * scale,
				       8, mode)) == NULL) {
		fprintf(stderr, "sdl: can't set video mode: %s\n",
			SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);

	SDL_WM_SetCaption(TITLE, TITLE);

	tick_timer = 0;
	SDL_SetTimer(10, timer_function);

	for (i = 0; i < NUM_COLORS; i++) {
		color[i].r = palette[i * 3] << 2;
		color[i].g = palette[i * 3 + 1] << 2;
		color[i].b = palette[i * 3 + 2] << 2;
		mapped_color[i] = SDL_MapRGB(screen->format,
					     color[i].r, color[i].g,
					     color[i].b);
	}
	SDL_SetColors(screen, color, 0, NUM_COLORS);

#define handle_case(d,s) case(d/8): \
	gfx_sdl.put_pixels = _putpixels_##d##bits_scale##s; break;

	gfx_sdl.put_pixels = _putpixels;

	switch (scale) {
	case 1:
		switch (screen->format->BytesPerPixel) {
			handle_case(8, 1);
			handle_case(16, 1);
			handle_case(32, 1);
		}
		break;
	case 2:
		switch (screen->format->BytesPerPixel) {
			handle_case(8, 2);
			handle_case(16, 2);
			handle_case(32, 2);
		}
		break;
	}

	return 0;
}

static int deinit_vidmode()
{
	SDL_Quit();
	return 0;
}

/* put a block onto the screen */
static void sdl_put_block(int x1, int y1, int x2, int y2)
{
	if (x1 >= FB_WIDTH)
		x1 = FB_WIDTH - 1;
	if (y1 >= FB_HEIGHT)
		y1 = FB_HEIGHT - 1;
	if (x2 >= FB_WIDTH)
		x2 = FB_WIDTH - 1;
	if (y2 >= FB_HEIGHT)
		y2 = FB_HEIGHT - 1;

	if (scale > 1) {
		x1 *= scale;
		y1 *= scale;
		x2 = (x2 + 1) * scale - 1;
		y2 = (y2 + 1) * scale - 1;
	}

	SDL_UpdateRect(screen, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

int sdl_is_keypress()
{
	process_events();
	return key_queue_start != key_queue_end;
}

int sdl_get_keypress()
{
	int k;

	while (key_queue_start == key_queue_end)	/* block */
		sdl_timer();
	key_dequeue(k);

	return k;
}

static Uint32 timer_function(Uint32 i)
{
	tick_timer++;
	return i;
}

static void sdl_timer()
{
	static Uint32 m = 0;
	Uint32 dm;

	if (tick_timer < m) {
		m = 0;
	}

	while ((dm = tick_timer - m) < 5) {
		SDL_Delay(5);
	}
	m = tick_timer;

	process_events();
}

#if 0

/* SDL wants its buffer to be filled completely and we generate sound
 * in smaller chunks. So we fill SDL's buffer and keep the remaining
 * sound in the mixer buffer to be used in the next call.
 */
static void fill_audio(void *udata, Uint8 * stream, int len)
{
	Uint32 p = 0;
	static Uint32 n = 0, s = 0;

	/* _D(("(%p, %p, %d)", udata, stream, len)); */
	memcpy(stream, (Uint8 *) buffer + s, p = n);
	for (n = 0, len -= p; len > 0; p += n, len -= n) {
		play_sound();
		n = mix_sound() << 1;
		memcpy(stream + p, buffer, (len < n) ? len : n);
		s = len;
	}
	n -= s;
}

static int sdl_init_sound(SINT16 * b)
{
	SDL_AudioSpec a;

	buffer = b;

	a.freq = 22050;
	a.format = (AUDIO_S16);
	a.channels = 1;
	a.samples = 2048;
	a.callback = fill_audio;
	a.userdata = NULL;

	if (SDL_OpenAudio(&a, NULL) < 0) {
		report("%s\n", SDL_GetError());
		return -1;
	}

	SDL_PauseAudio(0);

	report("SDL sound initialized.\n");

	return 0;
}

static void sdl_close_sound()
{
	SDL_CloseAudio();
}
#endif

