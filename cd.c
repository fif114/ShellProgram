#include "myHeader.h"

void change_directory(char *commandSet[])
{
	if(commandSet[1] == 0 || strcmp(commandSet[1], "~") == 0)
	{
		chdir(getenv("HOME"));
	}
	else if(strcmp(commandSet[1], "--help") == 0)
	{
		fprintf(stdout, "Usage : cd [FILENAME]\n\n");
		fprintf(stdout, "[FILENAME]		- Change directory\n");
		fprintf(stdout, "No [FILENAME] or '~'	- Go to HOME directory\n\n");
	}
	else
	{
		if(chdir(commandSet[1]) < 0)
		{
			fprintf(stderr, "ERROR : change directory error ocurred\n");
		}
	}
}
