#include "myHeader.h"


#define MAX 128	//buffer maximum size

void prompt(char []); // 프롬프트 출력과 명령어 입력
void stdout_redirection_execute(char *[], char *[]); // '>' 리다이렉션
void stdin_redirection_execute(char *[], char *[]); // '<' 리다이렉션
void background_process_sigfunc(int signo); // 백그라운드 처리
int parsing(char * , char *[]); // 파싱
void execute (char *[], int); // 파싱된 명령어 실행
void pipe_execute(char *[], char *[]); // '|' 파이프
void additional_parsing(char *, char*[], char *[]); //파이프, 리다이렉션을 위한 추가적인 파싱
void normal_command_execute(char *[], int); // bash의 기본 명령어 실행
void my_command_execute(char *[], int); // 내장명령어(my명령어) 실행

// 명령어 함수
void change_directory(char *[]);
void mypwd(char *[]);
void myls(char *[]);
void mycmp(char *[]);
void mycp(char *[]);
void mymv(char *[]);
void mymkdir(char *[]);
void myrmdir(char *[]);
void mycat(char *[]);
void myhead(char *[]);
void mytail(char *[]);
void myrm(char *[]);

int main(void)
{
	char buffer[MAX] = {0,}; // 명령어 입력을 위한 버퍼
	char buffer2[MAX] = {0,}; // 추가적인 파싱을 위한 버퍼
	char *commandSet[10]; // 파싱된 명령어 저장을 위한 포인트배열
	char *rear_command[10]; // 파이프,리다이렉션을 위한 포인트배열
	int flag = 0; // '>', '<', '|' 구분을 위한 플래그

	while(1)
	{
		memset(commandSet, 0x00, sizeof(char *)*10);  
		memset(rear_command, 0x00, sizeof(char *)*10);
		memset(buffer, 0x00, sizeof(buffer));
		memset(buffer2, 0x00, sizeof(buffer2));	
		flag = 0;

		prompt(buffer); // 프롬프트 출력

		if(strlen(buffer) == 0) // if you enter anything, continue the procedure
			continue;
		if(strcmp(buffer, "exit") == 0) // 종료
		{
			fprintf(stdout, "Goodbye! Shellprogram is finished\n");
			return 0;
		}
		if(flag == 2)
		{
			additional_parsing(buffer2, commandSet, rear_command);
			stdout_redirection_execute(commandSet, rear_command);
		}
		else if(flag == 3)
		{
			additional_parsing(buffer2, commandSet, rear_command);
			stdin_redirection_execute(commandSet, rear_command);
		}
		else if (flag == 4)
		{
			additional_parsing(buffer2, commandSet, rear_command);
			pipe_execute(commandSet, rear_command);
		}
		else
		{
			if(strcmp(commandSet[0], "cd") == 0) // cd명령 수행
				change_directory(commandSet);
			else
				execute(commandSet, flag);
		}
	}
	return 0;
}

void prompt(char buffer[]) 
{
	char cur_working_dir[MAX] = {0,};//현재 디렉터리의 경로를 저장하기 위한 배열
	
	getcwd(cur_working_dir, MAX); //현재 디렉터리
	
	printf("[%s %s] $ ", "eight team Shellprogram", strrchr(cur_working_dir, '/')+1); // 프롬프트 출력
	

	fgets(buffer, MAX, stdin); // 표준입력으로부터 명령어를 받아 버퍼에 저장
	buffer[strlen(buffer)-1] = '\0';
	fflush(stdin);
}


void stdin_redirection_execute(char *commandSet[], char *rear_command[])
{
	int fd, in; // fd = 파일 저장을 위한 파일 디스크립터 변수                                in = 표준입력을 복사, 저장하기위한 변수
	
	in = dup(0); // dep(0)을 통해 in변수에 표준입력을 복사 저장
	if((fd = open(rear_command[0], O_RDONLY, 0644)) < 0)
	{
		fprintf(stderr, "ERROR : File open error ocurred\n");
		exit(1);
	}
	fflush(stdin);
	close(0);
	dup2(fd, 0); // file fd => input
	close(fd);
	execute(commandSet, 0);
	close(0);
	dup2(in, 0); // change to stdin
}

void stdout_redirection_execute(char *commandSet[], char *rear_command[])
{
	int fd, out; // fd = 파일 저장을 위한 파일 디스크립터 변서
		     // out = 표준출력을 복사, 저장하기위한 변수

	out = dup(1); // dep(1)을 통해 out변수에 표준출력을 복사 저장
	if((fd = open(rear_command[0], O_CREAT|O_WRONLY|O_TRUNC, 0644)) < 0)
	{
		fprintf(stderr, "ERROR : File open error ocurred\n");
		exit(1);
	}
	fflush(stdout);

	close(1);
	dup2(fd, 1); // result => file fd
	close(fd);
	execute(commandSet, 0);
	close(1);
	dup2(out, 1); // change to stdout
}

int parsing(char *buffer, char *commandSet[])
{
	char *temp_token = NULL; //자른 명령어를 임시적으로 저장하기위한 포인터
	int i=0, flag = 0; // flag는 백그라운드, 리다이렉션, 파이프 상태를 저장하기위한 flag

	//0 = no 
	// 1 = background
	// 2 = '>' redirection
	// 3 = '<' redirection
	// 4 = '|' pipe


	temp_token = strtok(buffer, " \0\n");
	// strtok()를 이용하여 입력된 명령어를 기준(' ','\0',\n')에 따라 자르고 *commandSet[] 포인터 배열에 저장

	while(temp_token)
	{
		commandSet[i] = temp_token;
		if(strcmp(temp_token, "&") == 0)
		{
			flag = 1;
			commandSet[i] = NULL;
		}
		else if (strcmp(temp_token, ">")==0)
		{
			flag = 2;
			break;
		}
		else if (strcmp(temp_token, "<")==0)
		{
			flag = 3;
			break;
		}
		else if (strcmp(temp_token, "|")==0)
		{
			flag = 4;
			break;
		}
		else
		{
			flag = 0;
		}
		temp_token = strtok(NULL, " \0\n");
		i++;
	}
	commandSet[i] = NULL;
	return flag;
	// 상태를 플래그 값에 저장하여 리턴 값으로 반환
}

void execute(char *commandSet[], int flag)
{
	if(strncmp(commandSet[0], "my", 2) == 0)
	{
		my_command_execute(commandSet, flag);
	}
	// 입력된 명령어가 직접 작성한 my명령어 일 경우 my_command_Execute() 실행

	else 
	{
		normal_command_execute(commandSet, flag);
	}
	// 입력된 명령어가 기본적인 bash의 명령어 일 경우 normal_command_execute() 실행

}

void normal_command_execute(char *commandSet[], int flag)
{
	pid_t pid; //자식프로세스의 정보를 저장하기위한 변수
	int status; //자식프로세스의 상태를 저장하기위한 변수
	struct sigaction act; //백그라운드 환경을 위한 sigaction변수
	act.sa_handler = background_process_sigfunc;
	act.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &act, NULL);

	if((pid = fork()) < 0) // fork()를 통해 자식프로세스 생성
	{
		fprintf(stderr, "ERROR : Fork error ocurred\n");
		exit(1);
	}
	else if (pid == 0)
	{
		if(execvp(commandSet[0], commandSet) < 0) // evecvp() 를 통해 bash에 있는 기본 명령어를 실행
		{
			fprintf(stderr, "ERROR : Exec error ocurred\n");
			exit(1);
		}
	}
	else
	{
		if(flag == 0) //백그라운드 실행이 아닐경우
		{
			if(waitpid(pid, &status, 0) < 0)
			{
				fprintf(stderr, "ERROR : waitpid() error ocurred\n");
				exit(1);
			}
		}
// 부모 프로세스가 waitpid()를 통해 실행이 끝난 자식프로세스를 기다렸다가 종료

		else if(flag == 1) // 백그라운드 실행환경일 경우
		{
			printf("Background process pid : %d\n", pid);
//백그라운드 프로세스 pid를 출력
			waitpid(pid, &status, WNOHANG);
//WNOHANG옵션으로 waitpid()를 실행, WNOHANG 옵션을 사용하면 block되지 않고 다른 작업 실행 가능
		}
	}
}

void my_command_execute(char *commandSet[], int flag)
{
	pid_t pid; // 자식프로세스의 정보를 저장하기위한 변수
	int status; // 자식프로세스의 상태를 저장하기 위한 변수

	if((pid = fork()) < 0) // fork()를 통해 자식프로세스 생성
	{
		fprintf(stderr, "ERROR : fork() error ocurred\n");
		exit(1);
	}
	else if (pid == 0) // 자식프로세스에서 내장명령어(my명령어)를 확인하고 실행
	{
		if(strcmp(commandSet[0], "mypwd") == 0)
		{
			mypwd(commandSet);
			exit(0);
		}
		else if(strcmp(commandSet[0], "myls") == 0)
		{
			myls(commandSet);
			exit(0);
		}
		
		else if(strcmp(commandSet[0], "mycmp") == 0)
		{
			mycmp(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "mycp") == 0)
		{
			mycp(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "mymv") == 0)
		{
			mymv(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "mymkdir") == 0)
		{
			mymkdir(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "myrmdir") == 0)
		{
			myrmdir(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "mycat") == 0)
		{
			mycat(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "myhead") == 0)
		{
			myhead(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "mytail") == 0)
		{
			mytail(commandSet);
			exit(0);
		}

		else if(strcmp(commandSet[0], "myrm") == 0)
		{
			myrm(commandSet);
			exit(0);
		}
		else
		{
			fprintf(stderr, "ERROR : My command is not correct\n");
			exit(0);
		}
	}
	else
	{
		if(flag == 0)
		{
			if(waitpid(pid, &status, 0) < 0)
			{
				fprintf(stderr, "ERROR : waitpid() error ocurred\n");
				exit(1);
			}
		}
		else if (flag == 1)
		{
			printf("Background process pid : %d\n", pid);
			waitpid(pid, &status, WNOHANG);
		}
	}
}

		
void pipe_execute(char *commandSet[], char *rear_command[])
{
	int fd[2]; // 파이프를 위한 배열
	int in, out; // 표준 입/출력을 복사, 저장하기위한 변수

	in = dup(0);
	out = dup(1);
	// dup(0), dup(1)을 이용하여 표준 입/출력을 in, out에 복사 저장

	if(pipe(fd) <0)
		fprintf(stderr, "ERROR : Pipe error ocurred\n");
	
	close(1);

	dup2(fd[1], 1);  // 앞 명령어의 결과를 표준출력이아닌 뒷 명령어의 입력값이 되도록 설정
	close(fd[1]);
	execute(commandSet, 0);
	dup2(out, 1);

	close(0);
	dup2(fd[0], 0);
	close(fd[0]);
	execute(rear_command, 0);
	dup2(in, 0);
}

void additional_parsing(char buffer[], char *front_command[], char *rear_command[]) // 파이프와 리다이렉션을 위한 추가적 파싱
{
	char *buf1 = NULL;
	char *buf2 = NULL;
// 앞과 뒤 명령어를 나눈것을 임시 저장하기 위한 버퍼
	
	buf1 = strtok(buffer, "><|");
	buf2 = strtok(NULL, "><|");
	parsing(buf1, front_command);
	parsing(buf2, rear_command);
} // 파싱 과정을 거쳐 front_command와 rear_command에 명령어 저앙

void background_process_sigfunc(int signo)
{
	pid_t pid; // 자식 프로세스의 pid를 저장하기 위한 변수
	int status; // 자식 프로세스의 상태를 저장하기 위한 변수

	while((pid = waitpid(-1, &status, WNOHANG)) > 0) // signal을 받아 이 함수를 실행시키고 함수가 실행되면 백그라운드 프로세스의 pid를 출력하면서 자식프로세스를 종료. 백그라운드 프로세스가 없을 때 까지 반복문이 실행

	{
		printf("Background process finished : %d\n", pid);
	}
}
