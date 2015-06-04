
#ifndef COMMON_H_
#define COMMON_H_

#define FILENAME_SIZE 20

int read16l(FILE *);
void write_text(char *, int);
void clear_screen(void);
int load_bcg(char *, int, int);
void intro(void);

#endif
