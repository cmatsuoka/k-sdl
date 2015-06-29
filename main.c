#include <stdlib.h>
#include "common.h"
#include "graphics.h"
#include "draw.h"
#include "sound.h"
#include "game.h"


static void game()
{
	while (!end_game /* && !error */) {
		if (keypress()) {
			input_key = get_key();
			has_input = 1;
		}	

		poll_timer();

		game_1();
		game_2();
		do_scr();

		if (sound_state > 0) {
			if (sound_state == 2) {
				play_sound(8);
			} else if (sound_state == 3) {
				play_sound(24);
			} else if (sound_state == 4) {
				play_sound(22);
			} else {
				play_sound(1);
			}

			sound_state = 0;
		}

		/* attract_mode = 0; */

		if (!end_game) {
			/* end_game = important_routine(); */
		}

		/* word_6DCA sound stuff */

		draw_fence();
	}
}


int main(int argc, char **argv)
{
	init_graphics();
	clear_screen();

	do {
		/* init(); */
		intro();
		game();
	} while (0);

	deinit_graphics();

	exit(EXIT_SUCCESS);
}
