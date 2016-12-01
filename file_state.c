#include <stdio.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	struct stat st;
	struct tm *pt;

	if(argc != 2){
		fprintf(stderr,"Usage:%s <filename>\n",argv[0]);
		exit(1);
	}

	if(lstat(argv[1],&st)<0){
		perror("lstat");
		exit(1);
	}
	//判断文件类型
	if(S_ISREG(st.st_mode))
		printf("-");
	else if(S_ISDIR(st.st_mode))
		printf("d");
	else if(S_ISCHR(st.st_mode))
		printf("c");
	else if(S_ISBLK(st.st_mode))
		printf("b");
	else if(S_ISFIFO(st.st_mode))
		printf("p");
	else if(S_ISSOCK(st.st_mode))
		printf("s");
	else if(S_ISLNK(st.st_mode))
		printf("l");
	//获取文件权限
	int i;
	char str[] = "xwr-";
	for(i = 8; i >= 0; i--)
		if(st.st_mode & 1<<i)
			printf("%c",str[i%3]);
		else
			printf("%c",str[3]);
	printf(" %u ",st.st_nlink);	
	printf("%s ",getpwuid(st.st_uid)->pw_name);	
	printf("%s ",getgrgid(st.st_gid)->gr_name);	
	printf("%ld ",st.st_size);	
	pt = localtime(&st.st_atime);
	printf("%02d-%02d %02d:%02d ",pt->tm_mon+1,pt->tm_mday,pt->tm_hour,pt->tm_min);
	printf("%s\n",argv[1]);
		

	return 0;
}

