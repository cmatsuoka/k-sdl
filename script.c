#include <string.h>
#include "common.h"
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
#define CMD_END_ANIMATION	13

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

static int parse_line(char *line, unsigned char *bytecode, int *pos)
{
	int i, val;
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
		sscanf(t, "%d", &val);
		D_(D_INFO "%s,%d", script_cmd[i], val);
		bytecode[p++] = i;
		bytecode[p++] = val & 0xff;
		bytecode[p++] = val >> 8;
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

	
}
