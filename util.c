
#include <stdio.h>
#include "common.h"

int read16l(FILE *f)
{
	int a, b;

	a = fgetc(f);
	b = fgetc(f);

	return (b << 8) | a;
}

