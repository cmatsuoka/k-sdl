#ifndef SCRIPT_H_
#define SCRIPT_H_

#define NUM_CMD		14
#define NUM_BAL		4
#define NUM_FIGS	32

struct fig {
	int sprite;
	int x;
	int y;
};

extern int executing_bytecode;
extern struct fig fig[NUM_FIGS];
extern int fig_index;

void compile_script(char *, unsigned char *);
void execute_bytecode(void);
void read_bal(int);

#endif
