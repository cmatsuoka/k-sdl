
#ifndef COMMON_H_
#define COMMON_H_

#define FILENAME_SIZE 20

#include <stdio.h>

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
