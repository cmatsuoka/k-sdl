#include <stdio.h>
#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"
#include "script.h"

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
	clear_screen();

	compile_script("cal00", bytecode);
	read_sprite(6);
	execute_bytecode();

	return 0;
}

static int demo()
{
	clear_screen();

	compile_script("cal01", bytecode);
	load_bcg("fuji.bcg", 0x02a5);
	read_bal(0);
	execute_bytecode();

	return 0;
}

void intro()
{
	for (;;) {
/*
		broderbund_logo();

		if (credits_author())
			break;

		if (credits_port())
			break;

		if (show_title())
			break;

		if (scroller())
			break;
*/
		if (story())
			break;

		if (demo())
			break;

		break;
	}

	get_input();
	
}
