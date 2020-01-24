#pragma once

#include <stdio.h>
#include <stdlib.h>


static void read_file(const char* filepath, char** buffer, int* length)
{
	FILE* f = 0;
	f = fopen(filepath, "r");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		*length = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

		*buffer = (char*)malloc(*length);
		if (*buffer)
			fread(*buffer, sizeof(char), *length, f);
		else
			*length = 0;

		fclose(f);
	}
	else
		*length = 0;
}

static bool write_file(const char* filepath, const char* buffer)
{
	FILE* f = 0;
	f = fopen(filepath, "w");

	if (f)
	{
		fprintf(f, buffer);
		fclose(f);

		return true;
	}
	else
		return false;
}