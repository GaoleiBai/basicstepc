#include <stdio.h>
#include <stdlib.h>

#define SIZE 18
struct A{
	int sno;
	char name[SIZE];
	float score;
};
int main(void)
{
	FILE *fp;
	//int buf[BUFSIZ];
	struct A st[5];
	int ret;

	//打开文件
	fp = fopen("1.txt","r");
	if(NULL == fp){
		perror("fopen");//打印被调用函数调用失败的原因
		exit(1);
	}
	
	//ret = fread(buf,sizeof(char),BUFSIZ,fp);
	ret = fread(st,sizeof(struct A),5,fp);
	printf("ret = %d\n",ret);
	printf("%d\t%s\t%f\n",st[0].sno,st[0].name,st[0].score);
	printf("%d\t%s\t%f\n",st[1].sno,st[1].name,st[1].score);
	printf("%d\t%s\t%f\n",st[2].sno,st[2].name,st[2].score);
	//关闭文件
	fclose(fp);
	return 0;
}
