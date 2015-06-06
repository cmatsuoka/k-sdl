
#ifndef COMMON_H_
#define COMMON_H_

#define FILENAME_SIZE 20

#include <stdio.h>

#ifdef DEBUG
#define D_INFO "\x1b[33m"
#define D_CRIT "\x1b[31m"
#define D_WARN "\x1b[36m"
#define D_(args...) do { \
	printf("\x1b[33m%s \x1b[37m[%s:%d] " D_INFO, __FUNCTION__, \
		__FILE__, __LINE__); printf (args); printf ("\x1b[0m\n"); \
	} while (0)
#else
#define D_(args...) do {} while (0)
#endif

#define read8(f) fgetc(f)

FILE *fopen_wrapper(char *, char *);
int read16l(FILE *);
int readmem16l(unsigned char *);
void write_text(char *, int);
void clear_screen(void);
void show_screen(void);
int load_bcg(char *, int, int);
void intro(void);
int get_input(void);
int wait(int);
void wait_nokey(int);

#endif
