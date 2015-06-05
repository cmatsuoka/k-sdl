#include <stdio.h>
#include "common.h"
#include "graphics.h"


void intro()
{
	for (;;) {
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
		
		if (!wait(72))
			break;
	}

	
}
