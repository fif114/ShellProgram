#include "myHeader.h"

#define MAX 128

void mypwd(char *commandSet[])
{
	char *buffer = NULL; // 절대 경로를 저장하기 위한 버퍼

	if(commandSet[1] == 0) 
	{
		printf("%s\n", getcwd(buffer, MAX));
	}
	else if(strcmp(commandSet[1], "--help") == 0) // 사용법 출력
	{
		fprintf(stdout, "Usage : mypwd\n\n");
		fprintf(stdout, "Print the present absolute path\n\n");
	}
	else // print wrong option
	{
		fprintf(stderr, "ERROR : wrong mypwd option\n");
	}
}
