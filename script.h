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


extern unsigned char bytecode[BYTECODE_SIZE];
extern int attract_mode;
extern int stage;
extern int game_flags;
extern int sound_state;
extern int executing_bytecode;
extern struct fig fig[NUM_FIGS];
extern int fig_index;
extern int current_tune;
extern int wipe;

void compile_script(char *, unsigned char *);
void execute_bytecode(void);
void read_bal(int);

#endif
