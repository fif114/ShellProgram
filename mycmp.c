#include "myHeader.h"

#define MAX 128

void mycmp(char *commandSet[])
{
	FILE *fp1, *fp2; // 파일 포인터 변수

	int file_size1 = 0, file_size2 = 0, line_count1 = 0, line_count2 = 0; // size = 파일 크기를 저장하는 변수 count = 파일 라인 수를 저장하는 변수

	if(commandSet[1] == 0 || ((commandSet[2] == 0) && (strcmp(commandSet[1], "--help") != 0))) 
	{
		fprintf(stderr, "ERROR : mycmp error ocurred\n");
	}
	else if(strcmp(commandSet[1], "--help") == 0) // print help option // 사용법 출력
	{
		fprintf(stdout, "Usage : mycmp [FILENAME][FILENAME]\n\n");
		fprintf(stdout, "print difference between A file and B file\n\n");
	}
	else
	{
		if((fp1 = fopen(commandSet[1], "r")) == 0) // file open
		{
			fprintf(stderr, "ERROR : mycmp error(file open error) ocurred\n");
		}
		if((fp2 = fopen(commandSet[2], "r")) == 0)
		{
			fprintf(stderr, "ERROR : mycmp error(file open error) ocurred\n");
		}
		fseek(fp1, 0, SEEK_END); // 두개의 파일의 크기를 확인,저장
		file_size1 = ftell(fp1);
		fseek(fp2, 0, SEEK_END); 
		file_size2 = ftell(fp2);
		fseek(fp1, 0, SEEK_SET); // 파일 포인터 초기화
		fseek(fp2, 0, SEEK_SET);

		while(ftell(fp1) != file_size1)	// 두개의 파일의 라인수를 
		{
			if(fgetc(fp1) == '\n') // 결과를 포인터에 저장
				line_count1++; 
		}
		while(ftell(fp2) != file_size2)
		{
			if(fgetc(fp2) == '\n')
				line_count2++;
		}

		// 화면에 출력
		printf("%s		File size = %d, Lines = %d\n", commandSet[1], file_size1, line_count1);
		printf("%s		File size = %d, Lines = %d\n", commandSet[2], file_size2, line_count2);
		fclose(fp1); // 파일닫기
		fclose(fp2);
	}	

}
