#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"
#include "script.h"
#include "draw.h"

void draw_figs()
{
	int i;

	for (i = 0; i < fig_index; i++) {
		blit_sprite(fig[i].sprite, fig[i].x, fig[i].y);
	}
}

void draw_fence()
{
}

void erase_wall()
{
	if (stage < 2) {
		memset(framebuffer + 0x21c0 * 4, 0, 1920 * 8);
	} else {
		memset(framebuffer + 0x23a0 * 4, 0, 1680 * 8);
	}
}

void draw_scenario()
{
	int i, j;
	unsigned char p;

	if (attract_mode) {
		if (stage < 2) {
			/* draw sky */
			memset(framebuffer, 1, FB_WIDTH * 40);
		} else {
			//clear_screen();
			if (executing_bytecode) {
				draw_fence();
			}
		}
	}

	if (stage < 2) {
		//if (game_flags != comp_to_uid_1) {
			/* erase mt. fuji */
			memset(framebuffer + 0x0a00 * 4, 1, 3840 * 8);
			memset(framebuffer + 0x1900 * 4, 1, 1360 * 8);
			memset(framebuffer + 0x2120 * 4, 2, 40 * 8);
			memset(framebuffer + 0x2200 * 4, 3, 120 * 8);
		//}

		erase_wall();
		p = 0x99;

		if (game_flags & 0x01) {
			p = 0x66;
		}

		/* draw ground */

		for (i = 0; i < 30; i++) {
			for (j = 0; j < FB_WIDTH; j += 4) {
				unpack_pixels(0x3020 * 4 + i * FB_WIDTH + j, p, 255);
			}
			p ^= 0xff;
		}

		return;
	}

	erase_wall();

	if (stage < 3) {
		/* Inside palace */

		//if (game_flags != comp_to_uid_1) {
			memset(framebuffer + 0x0a00 * 4, 0, 3840 * 8);
			memset(framebuffer + 0x1900 * 4, 2, 880 * 8);
			memset(framebuffer + 0x1c20 * 4, 0, 80 * 8);
		//}

		/* draw orange floor */

		p = 0xaa;

		for (i = 0; i < 30; i++) {
			for (j = 0; j < FB_WIDTH; j += 4) {
				unpack_pixels(0x3020 * 4 + i * FB_WIDTH + j, p, 255);
			}
			p ^= 0xaa;
		}

		return;

	} else {
		/* Dungeon */

		//if (game_flags != comp_to_uid_1) {
			memset(framebuffer, 0, 4560 * 8);
		//}

		p = 0x55;

		/* draw blue floor */

		for (i = 0; i < 30; i++) {
			for (j = 0; j < FB_WIDTH; j += 4) {
				unpack_pixels(0x3020 * 4 + i * FB_WIDTH + j, p, 255);
			}
			p ^= 0x55;
		}
	}
}

void do_scr()
{
	int i;

	fig_index = 0;

wait(2);
	draw_scenario();

	for (i = 0; fig[i].sprite != 255; i++) {
		if ((~fig[i].x & 0x8000) && (fig[i].x & 0x4000)) {
			/*draw_fig(fig[i].sprite, fig[i].x ^ 0x4000, fig[i].y);*/
			D_(D_CRIT "draw_fig %d %d %d\n",
				fig[i].sprite, fig[i].x ^ 0x4000, fig[i].y);
		} else {
			blit_sprite(fig[i].sprite, fig[i].x, fig[i].y);
		}

		fig_index++;
	}

	D_(D_WARN "draw %d figs", i);

	if (!wipe) {
		if (attract_mode) {
			show_screen();
			play_sound(current_tune);
			return;
		}	

		/* copy_fb_area(); */
		play_sound(current_tune);
		return;
	}

	/* blit_something(); */
	play_sound(current_tune);
}

