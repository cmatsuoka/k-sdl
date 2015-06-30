#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"
#include "draw.h"
#include "script.h"

#define LINE_SIZE	80

unsigned char bytecode[BYTECODE_SIZE];
unsigned char bytecode_bal[BYTECODE_BAL_SIZE];

struct fig fig[NUM_FIGS];
int fig_index;
int attract_mode;
int stage;
int game_flags;
int sound_state;
int current_tune;
int wipe;

#define CMD_SET_TUNE		0
#define CMD_SET_BG		1
#define CMD_SET_FIG		2
#define CMD_CHG_FIG		3
#define CMD_DO_SCR		4
#define CMD_DEL_FIG		5
#define CMD_SET_WIPE		6
#define CMD_SET_NOWIPE		7
#define CMD_WAIT		8
#define CMD_INIT_SAL		9
#define CMD_SET_POS		10
#define CMD_INC_X		11
#define CMD_LOOP		12
#define CMD_END_ANIMATION	255

static void cmd_set_tune(unsigned char *);
static void cmd_set_bg(unsigned char *);
static void cmd_set_fig(unsigned char *);
static void cmd_chg_fig(unsigned char *);
static void cmd_do_scr(unsigned char *);
static void cmd_del_fig(unsigned char *);
static void cmd_set_wipe(unsigned char *);
static void cmd_set_nowipe(unsigned char *);
static void cmd_wait(unsigned char *);
static void cmd_init_sal(unsigned char *);
static void cmd_set_pos(unsigned char *);
static void cmd_inc_x(unsigned char *);
static void cmd_loop(unsigned char *);

static char *bal[NUM_BAL] = {
	"bal00",
	"bal01",
	"bal02",
	"bal03"
};

static char *script_cmd[NUM_CMD] = {
	"set_tune",
	"set_bg",
	"set_fig",
	"chg_fig",
	"do_scr",
	"del_fig",
	"set_wipe",
	"set_nowipe",
	"wait",
	"init_sal",
	"set_pos",
	"inc_x",
	"loop",
	"end_animation"
};

void (*bytecode_cmd[NUM_CMD - 1])(unsigned char *) = {
	cmd_set_tune,
	cmd_set_bg,
	cmd_set_fig,
	cmd_chg_fig,
	cmd_do_scr,
	cmd_del_fig,
	cmd_set_wipe,
	cmd_set_nowipe,
	cmd_wait,
	cmd_init_sal,
	cmd_set_pos,
	cmd_inc_x,
	cmd_loop
};


/* Bytecode vars */
int executing_bytecode;
static int bytecode_ip;
static int current_bg;
static int do_scr_wait_key = 0;

static int parse_line(char *line, unsigned char *bytecode, int *pos)
{
	int i, val1, val2, val3, val4;
	char *t;
	int p = *pos;

	t = strtok(line, ",");

	for (i = 0; i < NUM_CMD; i++) {
		if (!strncmp(t, script_cmd[i], LINE_SIZE)) {
			break;
		}
	}

	if (i == NUM_CMD) {
		/* Invalid command */
		return -1;
	}

	t = strtok(NULL, "");
	
	switch (i) {
	case CMD_SET_TUNE:
		sscanf(t, "%d", &val1);
		D_(D_INFO "%s,%d", script_cmd[i], val1);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		break;
	case CMD_SET_BG:
		sscanf(t, "%d", &val1);
		D_(D_INFO "%s,%d" , script_cmd[i], val1);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		break;
	case CMD_SET_FIG:
		sscanf(t, "%d %d %d", &val1, &val2, &val3);
		D_(D_INFO "%s,%d %d %d", script_cmd[i], val1, val2, val3);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		bytecode[p++] = val2 & 0xff;
		bytecode[p++] = val2 >> 8;
		bytecode[p++] = val3 & 0xff;
		break;
	case CMD_CHG_FIG:
		sscanf(t, "%d %d %d %d", &val1, &val2, &val3, &val4);
		D_(D_INFO "%s,%d, %d %d %d", script_cmd[i],
						val1, val2, val3, val4);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		bytecode[p++] = val2;
		bytecode[p++] = val3 & 0xff;
		bytecode[p++] = val3 >> 8;
		bytecode[p++] = val4 & 0xff;
		break;
	case CMD_DO_SCR:
		bytecode[p++] = i;
		break;
	case CMD_DEL_FIG:
		sscanf(t, "%d", &val1);
		D_(D_INFO "%s,%d", script_cmd[i], val1);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		break;
	case CMD_SET_WIPE:
		D_(D_INFO "%s" , script_cmd[i]);
		bytecode[p++] = i;
		break;
	case CMD_SET_NOWIPE:
		D_(D_INFO "%s" , script_cmd[i]);
		bytecode[p++] = i;
		break;
	case CMD_WAIT:
		sscanf(t, "%d", &val1);
		D_(D_INFO "%s,%d" , script_cmd[i], val1);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		break;
	case CMD_INIT_SAL:
		D_(D_INFO "%s" , script_cmd[i]);
		bytecode[p++] = i;
		break;
	}

	*pos = p;

	return 0;
}

void compile_script(char *filename, unsigned char *bytecode)
{
	FILE *f;
	char buffer[LINE_SIZE];
	char *line;
	int pos = 0;

	f = fopen_wrapper(filename, "r");

	for (;;) {
		line = fgets(buffer, LINE_SIZE, f);
		if (line == NULL)
			break;
		parse_line(line, bytecode, &pos);
	}

	bytecode[pos] = CMD_END_ANIMATION;

	D_(D_WARN "bytecode space used: %d", pos);
}

/* ----------------------- Bytecode interpreter ------------------------ */


void execute_bytecode()
{
	executing_bytecode = 1;

	cmd_init_sal(NULL);
	bytecode_ip = 0;

	for (;;) {
		int opcode = bytecode[bytecode_ip++];

		if (opcode == 255) {
			D_(D_WARN "End animation");
			break;
		}

		D_(D_INFO "opcode=%d (%s)", opcode, script_cmd[opcode]);

		bytecode_cmd[opcode](bytecode + bytecode_ip);
	};

	executing_bytecode = 0;
}

static void cmd_set_tune(unsigned char *v)
{
	current_tune = v[0];
	bytecode_ip++;
}

static void cmd_set_bg(unsigned char *v)
{
	current_bg = v[0];
	bytecode_ip++;
}

static void cmd_set_fig(unsigned char *v)
{
	fig[fig_index].sprite = v[0];
	fig[fig_index].x = readmem16l(v + 1);
	fig[fig_index].y = v[3];
	fig_index++;
	fig[fig_index].sprite = 255;
	bytecode_ip += 4;
}

static void cmd_chg_fig(unsigned char *v)
{
	int index = v[0];
	fig[index].sprite = v[1];
	fig[index].x = readmem16l(v + 2);
	fig[index].y = v[4];
	bytecode_ip += 5;
}

static void cmd_do_scr(unsigned char *v)
{
	do_scr();
	if (do_scr_wait_key) {
		get_input();
	}
}

static void cmd_del_fig(unsigned char *v)
{
	int i;

	fig_index--;
	for (i = v[0]; i < fig_index; i++) {
		fig[i].sprite = fig[i + 1].sprite;
		fig[i].x = fig[i + 1].x;
		fig[i].y = fig[i + 1].y;
	}
	fig[i].sprite = 255;
	bytecode_ip++;
}

static void cmd_set_wipe(unsigned char *v)
{
	wipe = 1;
}

static void cmd_set_nowipe(unsigned char *v)
{
	wipe = 0;
}

static void cmd_wait(unsigned char *v)
{
	if (do_scr_wait_key == 0) {
		wait_nokey(v[0]);
	}
	bytecode_ip++;
}

static void cmd_init_sal(unsigned char *v)
{
	current_tune = 0;
	wipe = 0;
	current_bg = 0;
	fig_index = 0;
}

static void cmd_set_pos(unsigned char *v)
{
#if 0
	if (do_scr_wait_key == 1) {
		cmd_do_scr(v);
		cmd_init_sal(v);
	}

	/* auto_x_offset = 2; */
	bytecode_ip += 2;
#endif
}

static void cmd_inc_x(unsigned char *v)
{
}

static void cmd_loop(unsigned char *v)
{
}

/* ------------------------- BAL script stuff -------------------------- */

static void index_bal_bytecode(int n)
{
	int i, bal_ip, bal_offset;

	bal_ip = 0;
	bal_offset = 0;

	for (i = 0; i < 16; i++) {
		
	}
}

void read_bal(int num)
{
	read_sprite(num);
	
	if (num < 4) {
		compile_script(bal[num], bytecode_bal);

		if (num < 2) {
			index_bal_bytecode(1);
		} else {
			index_bal_bytecode(6);
		}
	}

	if (num == 2) {
		/* check_tampering(); */
	}
		
	/* wtf4(num) */
}
