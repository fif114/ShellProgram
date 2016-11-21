#include "myHeader.h"

#define MAX 128

void mymkdir(char *commandSet[])
{
	char *temp_token = NULL; // [PATH]를 자르기 위한 임시포인터
	char path_buffer[MAX]={0,}; // 절대경로를 저장하기위한 임시버퍼
	char *dirset[20]; //자른 [PATH]를 저장하기위한 포인터배열
	int i=0, cur_dir_pointer = 0; // path에 있는디렉터리 개수 확인을 위한 변수
	bool create_dir_check = true; // 디렉터리 존재 확인을 위한 변수

	memset(dirset, 0x00, sizeof(char *)*20);

	if(commandSet[1] == 0) // 입력되지 않을떄 에러메시지 출력
	{
		fprintf(stderr, "ERROR : Mymkdir error ocurred\n");
	}
	else if(strcmp(commandSet[1], "--help") == 0) // 사용법 출력
	{
		fprintf(stdout, "USAGE : mkdir [DIRECTORY]\n\n");
		fprintf(stdout, "no option	- make [DIRECTORY]directory\n");
		fprintf(stdout, "-p [PATH]	- make all directory in [PATH]\n\n");
	}
	else
	{
		if(strcmp(commandSet[1], "-p") == 0) // -p option
		{
			temp_token = strtok(commandSet[2], "/");
			while(temp_token)
			{
				dirset[i++]= temp_token;
				temp_token = strtok(NULL, "/");
			}
			cur_dir_pointer = i;
			i=0;
			while(i<cur_dir_pointer) // 디렉터리를 확인하고 생성
			{
				if(access(dirset[i], F_OK) == 0) // 디렉터리가 존재하면
				{
					chdir(dirset[i]);
					create_dir_check = false;
				}
				else // 디렉터리가 없으면
				{	
					chdir(dirset[i-1]);
					if(mkdir(dirset[i], 0755) != 0)
						create_dir_check = false;
					create_dir_check = true;
				}
				i++;
			}
			chdir(dirset[i-1]);
			getcwd(path_buffer, MAX);
			chdir("..");
			i=0;
			while(i<cur_dir_pointer) // 현재위치 이동
			{
				chdir("..");
				i++;
			}
		}
		else // 옵션이 없을때
		{
			if(mkdir(commandSet[1], 0755) != 0)
			{
				create_dir_check = false;
			}
			chdir(commandSet[1]);
			getcwd(path_buffer, MAX);
			chdir("..");
		}
		if(create_dir_check == true) // 경로 출력
			printf("Absolute path : %s\n", path_buffer);
		else
			printf("Creating new directory is not successful\n");
	}
}
