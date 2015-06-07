#include <string.h>
#include "common.h"
#include "graphics.h"
#include "sprite.h"
#include "script.h"

#define LINE_SIZE 80
#define NUM_CMD 14

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

static int parse_line(char *line, unsigned char *bytecode, int *pos)
{
	int i, val1, val2, val3;
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
		break;
	case CMD_SET_FIG:
		sscanf(t, "%d %d %d", &val1, &val2, &val3);
		D_(D_INFO "%s,%d %d %d", script_cmd[i], val1, val2, val3);
		bytecode[p++] = i;
		bytecode[p++] = val1;
		bytecode[p++] = val2;
		bytecode[p++] = val3;
		break;
	case CMD_CHG_FIG:
		break;
	case CMD_DO_SCR:
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
}

static int bytecode_ip;

void execute_bytecode()
{
	bytecode_ip = 0;

	for (;;) {
		int opcode = bytecode[bytecode_ip++];

		D_(D_INFO "opcode=%d (%s)", opcode, script_cmd[opcode]);

		if (opcode == 255) {
			D_(D_WARN "End animation");
			break;
		}

		bytecode_cmd[opcode](bytecode + bytecode_ip);
	};
}

static void cmd_set_tune(unsigned char *v)
{
	bytecode_ip++;
}

static void cmd_set_bg(unsigned char *v)
{
}

static void cmd_set_fig(unsigned char *v)
{
	blit_sprite(v[0], v[1], v[2], 0);
	bytecode_ip += 3;
}

static void cmd_chg_fig(unsigned char *v)
{
}

static void cmd_do_scr(unsigned char *v)
{
	show_screen();
	get_input();
}

static void cmd_del_fig(unsigned char *v)
{
}

static void cmd_set_wipe(unsigned char *v)
{
}

static void cmd_set_nowipe(unsigned char *v)
{
}

static void cmd_wait(unsigned char *v)
{
}

static void cmd_init_sal(unsigned char *v)
{
}

static void cmd_set_pos(unsigned char *v)
{
}

static void cmd_inc_x(unsigned char *v)
{
}

static void cmd_loop(unsigned char *v)
{
}

