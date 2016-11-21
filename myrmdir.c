#include "myHeader.h"

#define MAX 128

void myrmdir(char *commandSet[])
{
	struct stat *buf;// 파일정보를 저장하기 위한 구조체 변수
	int fd; // 파일 디스크립터 변수

	if(commandSet[1] == 0) // error ocurred
	{
		fprintf(stderr, "ERROR : Myrmdir error ocurred\n");
	}
	else if(strcmp(commandSet[1], "--help") == 0) // 사용법 출력
	{
		fprintf(stdout, "USAGE : rmdir [DIRECTORY]\n\n");
		fprintf(stdout, "if it is directory		 - erase [DIRECTORY]directory\n");
		fprintf(stdout, "if it is file or it is not empty - print error message\n\n");
	}
	else
	{
		fd = open(commandSet[1], O_RDONLY);
		fstat(fd, buf);
		if(S_ISDIR(buf->st_mode))
		{
			if(rmdir(commandSet[1]) != 0) // 디렉터리 삭제
			{
				if(errno == ENOTEMPTY) // 디렉터리가 비어있지 않으면
					fprintf(stderr, "ERROR : Directory is not empty\n");
				else
					fprintf(stderr, "ERROR : Myrmdir error ocurred\n");
			}
		}
		else //file
			fprintf(stderr, "ERROR : This file is not directory\n");
		close(fd);
	}
}
