
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

int read16l(FILE *f)
{
	int a, b;

	a = fgetc(f);
	b = fgetc(f);

	return (b << 8) | a;
}

FILE *fopen_wrapper(char *filename, char *mode)
{
	FILE *f;
	char *u;
	int i, len;

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

	free(u);
	return NULL;
}
