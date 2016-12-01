#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main(int argc,char *argv[])
{
	FILE *read_fp,*write_fp;
	char buf[SIZE];

	if(argc != 3){
		printf("Usage:%s <src_filename> <des_filename>\n",argv[0]);
		exit(1);
	}

	//打开文件
	read_fp = fopen(argv[1],"r");
	if(NULL == read_fp){
		perror("fopen");//打印被调用函数调用失败的原因
		exit(1);
	}
	write_fp= fopen(argv[2],"w");
	if(NULL == write_fp){
		perror("fopen");//打印被调用函数调用失败的原因
		exit(1);
	}
	//文件拷贝
	while(fgets(buf,SIZE,read_fp)){
		printf("%s",buf);
		fputs(buf,write_fp);
		fflush(write_fp);
	}
	while(1);
	//关闭文件
	fclose(read_fp);
	fclose(write_fp);
	return 0;
}
