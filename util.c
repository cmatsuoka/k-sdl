
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "graphics.h"

int read16l(FILE *f)
{
	int a, b;

	a = fgetc(f);
	b = fgetc(f);

	return (b << 8) | a;
}

int readmem16l(unsigned char *buffer)
{
	int a, b;

	a = buffer[0];
	b = buffer[1];

	return (b << 8) | a;
}

int get_input()
{
	while (!keypress()) {
		poll_timer();
	}

	return get_key() == 0x1b;
}

int wait(int t)
{
	int i;

	for (i = 0; i < t; i++) {
		if (keypress()) {
			get_key();
			return 0;
		}
		poll_timer();
	}

	return 1;
}

void wait_nokey(int t)
{
	int i;

	for (i = 0; i < t; i++) {
		poll_timer();
	}
}

void show_screen()
{
	flush_screen();
	do_update();
}

static int check_disk()
{
	clear_screen();
	write_text("make sure your karateka", 0x17ca);
	write_text("disk is in drive a{", 0x1cd2);
	write_text("press any key to continue", 0x21c8);
	show_screen();

	return get_input();
}

FILE *fopen_wrapper(char *filename, char *mode)
{
	FILE *f;
	char *u;
	int i, len;

	do {
		if ((f = fopen(filename, mode)) != NULL) {
			return f;
		}

		if ((u = strdup(filename)) == NULL) {
			return NULL;
		}

		len = strlen(filename);
		for (i = 0; i < len; i++) {
			u[i] = toupper(filename[i]);
		}

		if ((f = fopen(u, mode)) != NULL) {
			free(u);
			return f;
		}
	} while (check_disk());

	free(u);
	exit(EXIT_FAILURE);
}
