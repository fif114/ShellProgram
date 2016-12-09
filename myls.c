#include "myHeader.h"

void myls(char *commandSet[])
{
	struct stat buf; // 파일의 정보를 갖고있는 구조체 변수
	struct passwd *user_buf; // 사용자 정보를 갖고 있는 구조체 변수
	struct group *group_buf; // 사용자 그룹의 정보를 갖고 있는 구조체변수
	struct tm *time_buf; // 시간 정보를 갖고 있는 구조체 변수
	DIR *dp; // 디렉터리의 정보를 갖고 있는 구조체 변수
	struct dirent *dirp; // 디렉터리 내용물의 정보를 갖고 있는 구조체 변수
	char type_rwx[11] = {0,}; // 파일 종류와 권한을 출력하기 위한 버퍼
	char *path = NULL; // 처음 디렉터리 경로를 저장한 포인터
	char time[30] = {0,}; // 수정된 시간을 저장하기 위한 버퍼
	int hl_count = 0, file_size = 0; // 하드링크 수와 파일의 크기를 저장하기 위한 변수

	path = getcwd(NULL, 0);
	
	if((dp = opendir(path)) == NULL)
		printf("opendir error\n");

	if(commandSet[1] == 0) // 모든 파일 목록 출력
	{
		while((dirp = readdir(dp)) != NULL)
		{
			lstat(dirp->d_name, &buf);
	
			if(S_ISREG(buf.st_mode))			type_rwx[0] = '-'; // 파일 type
			else if(S_ISBLK(buf.st_mode))	type_rwx[0] = 'b';
			else if(S_ISCHR(buf.st_mode))	type_rwx[0] = 'c';
			else if(S_ISFIFO(buf.st_mode))	type_rwx[0] = 'f';
			else if(S_ISSOCK(buf.st_mode))	type_rwx[0] = 's';
			else if(S_ISLNK(buf.st_mode))	type_rwx[0] = 'l';
			else if(S_ISDIR(buf.st_mode))	type_rwx[0] = 'd';	

			if(S_IRUSR & buf.st_mode)		type_rwx[1] = 'r'; // file permission
			else								type_rwx[1] = '-';
			if(S_IWUSR & buf.st_mode)		type_rwx[2] = 'w';
			else								type_rwx[2] = '-';
			if(S_IXUSR & buf.st_mode)		type_rwx[3] = 'x';
			else								type_rwx[3] = '-';
			if(S_IRGRP & buf.st_mode)		type_rwx[4] = 'r';
			else								type_rwx[4] = '-';
			if(S_IWGRP & buf.st_mode)		type_rwx[5] = 'w';
			else								type_rwx[5] = '-';
			if(S_IXGRP & buf.st_mode)		type_rwx[6] = 'x';
			else								type_rwx[6] = '-';
			if(S_IROTH & buf.st_mode)		type_rwx[7] = 'r';
			else								type_rwx[7] = '-';
			if(S_IWOTH & buf.st_mode)		type_rwx[8] = 'w';
			else								type_rwx[8] = '-';
			if(S_IXOTH & buf.st_mode)		type_rwx[9] = 'x';
			else								type_rwx[9] = '-';
	
			type_rwx[10] = '\0';
			hl_count = buf.st_nlink; // 하드링크 수 count
			user_buf = getpwuid(buf.st_uid);	// 소유자
			group_buf = getgrgid(user_buf->pw_gid); // 그룹
			file_size = buf.st_size; 		// 파일 크기
			time_buf = localtime(&(buf.st_mtime));
			strftime(time, 30, "%m월 %d %H:%M", time_buf); //시간

			// 파일 목록 출력
			fprintf(stdout, "%s %d %-7s%-7s%-7d%-15s%s\n", type_rwx, hl_count, user_buf->pw_name, group_buf->gr_name, file_size, time, dirp->d_name);
		}
	}
	else if(strcmp(commandSet[1], "--help") == 0) // 사용법
	{
		fprintf(stdout, "Usage : myls\n\n");
		fprintf(stdout, "no option	  	- print file list\n");
		fprintf(stdout, "[FILENAME]		- print [FILENAME] file\n\n");
	}
	else
	{
		while((dirp = readdir(dp)) != NULL)
		{	
			lstat(dirp->d_name, &buf);
			if(strcmp(dirp->d_name, commandSet[1]) == 0)
			{
				if(S_ISREG(buf.st_mode))			type_rwx[0] = '-'; // 파일 type
				else if(S_ISBLK(buf.st_mode))	type_rwx[0] = 'b';
				else if(S_ISCHR(buf.st_mode))	type_rwx[0] = 'c';
				else if(S_ISFIFO(buf.st_mode))	type_rwx[0] = 'f';
				else if(S_ISSOCK(buf.st_mode))	type_rwx[0] = 's';
				else if(S_ISLNK(buf.st_mode))	type_rwx[0] = 'l';
				else if(S_ISDIR(buf.st_mode))	type_rwx[0] = 'd';	
	
				if(S_IRUSR & buf.st_mode)		type_rwx[1] = 'r'; // 파일 허가권
				else								type_rwx[1] = '-';
				if(S_IWUSR & buf.st_mode)		type_rwx[2] = 'w';
				else								type_rwx[2] = '-';
				if(S_IXUSR & buf.st_mode)		type_rwx[3] = 'x';
				else								type_rwx[3] = '-';
				if(S_IRGRP & buf.st_mode)		type_rwx[4] = 'r';
				else								type_rwx[4] = '-';
				if(S_IWGRP & buf.st_mode)		type_rwx[5] = 'w';
				else								type_rwx[5] = '-';
				if(S_IXGRP & buf.st_mode)		type_rwx[6] = 'x';
				else								type_rwx[6] = '-';
				if(S_IROTH & buf.st_mode)		type_rwx[7] = 'r';
				else								type_rwx[7] = '-';
				if(S_IWOTH & buf.st_mode)		type_rwx[8] = 'w';
				else								type_rwx[8] = '-';
				if(S_IXOTH & buf.st_mode)		type_rwx[9] = 'x';
				else								type_rwx[9] = '-';
		
				type_rwx[10] = '\0';
				hl_count = buf.st_nlink; // 하드링크 수 count
				user_buf = getpwuid(buf.st_uid);	// 소유자
				group_buf = getgrgid(user_buf->pw_gid); // 그룹
				file_size = buf.st_size; 		// 파일 크기
				time_buf = localtime(&(buf.st_mtime));
				strftime(time, 30, "%m월 %d %H:%M", time_buf); //시간
	
				// 파일 목록 출력
				fprintf(stdout, "%s %d %-7s%-7s%-7d%-15s%s\n", type_rwx, hl_count, user_buf->pw_name, group_buf->gr_name, file_size, time, dirp->d_name);
				break;
			}
		}
	}
	closedir(dp);
}
