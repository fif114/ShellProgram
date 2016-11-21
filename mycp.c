#include "myHeader.h"

void mycp(char *commandSet[])
{
	int fd1, fd2; // 파일 디스크립터 변수
	char ch[1];

	if(commandSet[1] == 0) // 파일 이름이 지정안됬을때
	{
		fprintf(stderr, "ERROR : File open error ocurred\n");
	}
	else if(strcmp(commandSet[1], "--help") == 0) // 사용법 설명
	{
		fprintf(stdout, "Usage : mycp [SOURCE][DEST]\n\n");
		fprintf(stdout, "[SOURCE][DEST]	- copy [SOURCE]file to [DEST] file\n\n");
	}
	else
	{
		if((fd1 = open(commandSet[1], O_RDONLY)) == -1 || (fd2 = open(commandSet[2], O_WRONLY|O_CREAT|O_EXCL, 0755)) == -1) // 파일오픈 에러 출력
		{
			fprintf(stderr, "ERROR : File open error ocurred\n");
		}
		else // [SOURCE]을 [DEST]로 복사
		{
			lseek(fd1, 0, SEEK_SET);
			while( 0 != read(fd1, ch, 1))
				write(fd2, ch, 1);
		}
		close(fd1);
		close(fd2);
	}
}

