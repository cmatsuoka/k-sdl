#include <stdio.h>
#include "common.h"
#include "graphics.h"

void intro()
{
	clear_screen();
	write_text("a game by", 0x1a5b);
	write_text("jordan mechner", 0x1f54);
	//wait(72);
}
