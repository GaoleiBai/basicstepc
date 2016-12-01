#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t tloc;
	long ret;
	struct tm *ploc;
	char *tp;

	ret = time(&tloc);  //获取日历时间
	printf("tloc = %ld\n",tloc);
	printf("ret = %ld\n",ret);

	ploc = localtime(&tloc); //将日历时间转换成本地时间
	printf("%d-%d-%d %d:%d:%d\n",
			ploc->tm_year+1900,ploc->tm_mon+1,ploc->tm_mday,\
			ploc->tm_hour,ploc->tm_min,ploc->tm_sec);
	tp = ctime(&tloc); //将日历时间转换成字符串
	printf("%s",tp);
	tp = asctime(ploc); //将本地时间转换成字符串
	printf("%s",tp);
	return 0;
}
