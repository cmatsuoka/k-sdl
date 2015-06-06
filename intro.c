#include <stdio.h>
#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"

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

static void scroller()
{
	int i, j;

	clear_screen();

	for (i = 0; text[i]; i++) {
		write_text(text[i], FB_WIDTH * FB_HEIGHT / 4);

		for (j = 0; j < 15; j++) {
			memmove(framebuffer, framebuffer + FB_WIDTH,
						FB_WIDTH * (FB_HEIGHT + 11));
			show_screen();
			wait(1);
		}
	}
}

void intro()
{
	for (;;) {
		read_sprite(13);
		clear_screen();
		blit_sprite(91, 106, 115, 0);
		show_screen();
		wait_nokey(72);

		clear_screen();
		write_text("a game by", 0x1a5b);
		write_text("jordan mechner", 0x1f54);
		show_screen();
		if (!wait(72))
			break;

		clear_screen();
		write_text("ibm version by", 0x1a56);
		write_text("the connelley group", 0x1f50);
		show_screen();
		if (!wait(72))
			break;

		clear_screen();
		load_bcg("title.bcg", 0x15e0, 0);
		show_screen();
		if (!wait(18))
			break;
		blit_sprite(92, 40, 180, 0);
		show_screen();
		if (!wait(72))
			break;

		scroller();
	}

	
}
