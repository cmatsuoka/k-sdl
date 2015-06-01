#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "graphics.h"

int main(int argc, char **argv)
{
	init_graphics();

	clear_screen();
	write_text("a long time ago|", framebuffer + 320 * 80 + 16);
	write_text("high atop a craggy cliff{{{", framebuffer + 320 * 96 + 16);
	flush_screen();
	do_update();

	sleep(3);

	deinit_graphics();	

	exit(EXIT_SUCCESS);
}
