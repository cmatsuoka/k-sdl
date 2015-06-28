#include <stdio.h>
#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"
#include "script.h"
#include "draw.h"

static char *text[] = {
	"high atop a craggy cliff|",
	"guarded by an army of",
	"fierce warriors| stands the",
	"fortress of the evil",
	"warlord akuma{ deep in the",
	"darkest dungeon of the",
	"castle| akuma gloats over",
	"his lovely captive| the",
	"princess mariko{",
	" ",
	"you are one trained in the",
	"way of karate} a karateka{",
	"alone and unarmed| you must",
	"defeat akuma and rescue the",
	"beautiful mariko{",
	" ",
	"put fear and self~concern",
	"behind you{  focus your will",
	"on your objective|",
	"accepting death as a",
	"possibility{  this is the way",
	"of the karateka{",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	" ",
	NULL
};

static void broderbund_logo()
{
	read_sprite(13);
	clear_screen();
	blit_sprite(91, 106, 115);
	show_screen();
	wait_nokey(72);
}

static int credits_author()
{
	clear_screen();
	write_text("a game by", 0x1a5b);
	write_text("jordan mechner", 0x1f54);
	show_screen();
	return !wait(72);
}

static int credits_port()
{
	clear_screen();
	write_text("ibm version by", 0x1a56);
	write_text("the connelley group", 0x1f50);
	show_screen();
	return !wait(72);
}

static int show_title()
{
	clear_screen();
	load_bcg("title.bcg", 0x15e0);
	show_screen();
	if (!wait(18))
		return 1;
	blit_sprite(92, 40, 180);
	show_screen();
	return !wait(72);
}

static int scroller()
{
	int i, j;

	clear_screen();

	for (i = 0; text[i]; i++) {
		write_text(text[i], FB_WIDTH * FB_HEIGHT / 4);

		for (j = 0; j < 15; j++) {
			memmove(framebuffer, framebuffer + FB_WIDTH,
						FB_WIDTH * (FB_HEIGHT + 11));
			show_screen();
			if (!wait(1)) {
				return 1;
			}
		}
	}

	return 0;
}

static int story()
{
	game_flags = 0;
	clear_screen();
	attract_mode = 1;
	stage = 4;

	compile_script("cal00", bytecode);
	read_sprite(6);
	execute_bytecode();

	return 0;
}

static int check_x()
{
	return 1;
}

static int demo()
{
	int end = 0;

	stage = 0;
	compile_script("cal01", bytecode);
	load_bcg("fuji.bcg", FUJI_OFFSET);
	read_bal(0);
	execute_bytecode();

	stage = 0;
	/* check_key_in_script = 1; */

	while (!end) {
		poll_timer();

		/* call_22e1(); */
		/* call_2bdd(); */

		do_scr();

#if 0
		if (some_state >= 0) {
			if (some_state == 2) {
				play_sound(8);
			} else {
				play_sound(1);
			}

			some_state = 0;
		}
#endif

		draw_fence();
		end = check_x();
	}

	return 0;
}

void intro()
{
	for (;;) {

#if 0
		broderbund_logo();

		if (credits_author())
			break;

		if (credits_port())
			break;

		if (show_title())
			break;

		if (scroller())
			break;
#endif

		if (story())
			break;

		if (demo())
			break;

		break;
	}

	get_input();
	
}
