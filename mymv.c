#include "myHeader.h"

void mymv(char *commandSet[])
{
	FILE *fp1, *fp2; // 파일 포인터 변수
	char ch;

	if(commandSet[1] == 0) // 파일 이름을 지정안했을때
	{
		fprintf(stderr, "ERROR : File open error ocurred\n");
	}
	else if(strcmp(commandSet[1], "--help") == 0) // 사용법 출력
	{
		fprintf(stdout, "Usage : mymv [SOURCE][DEST]\n\n");
		fprintf(stdout, "[SOURCE][DEST]	- move [SOURCE]file to [DEST] file\n\n");
	}
	else
	{
		if((fp1 = fopen(commandSet[1], "r")) == 0 || (fp2 = fopen(commandSet[2], "w")) == 0) // 파일오픈 에러메시지 출력
		{
			fprintf(stderr, "ERROR : File open error ocurred\n");
		}
		else // [SOURCE]파일을 [DEST]로 이동
		{
			fseek(fp2, 0, SEEK_END);
			if(ftell(fp2) != 0) // [DEST]파일 존재 여부를 ftell로 확인
			{
				fprintf(stderr, "ERROR : File already exists\n");
			}
			fseek(fp1, 0, SEEK_SET);
			while( EOF != (ch = fgetc(fp1))) // [SOURCE] 파일의 내용을 fget()으로 읽음
				fputc(ch, fp2); // fputc()를 이용해 [DEST]파일에 입력
			fclose(fp1);
			unlink(commandSet[1]); // [SOURCE]파일을 unlink() 를 통해 삭제
		}
		fclose(fp2);
	}
}
