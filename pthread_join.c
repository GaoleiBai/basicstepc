#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>

#include <pthread.h>

#define MAX_NAME_LEN 32
struct person_info {
	int sex;
	char name[MAX_NAME_LEN+1];
	int age;
	//....
};

void *sub_thread_routine(void * arg)
{
	int i;
	struct person_info *pInfo = NULL;

	printf("I'm sub thread,my thread ID is:0x%x, arg =%s\n", (int)pthread_self(), (char *)arg);
	
	pInfo = (struct person_info *)malloc(sizeof(struct person_info)); 	
	if(!pInfo) {	/* 内存分配不成功*/
		pthread_exit(NULL);
	}

	pInfo->sex = 1;
	pInfo->age = 18;
	
	bzero(pInfo->name, MAX_NAME_LEN+1);
	int len = strlen((char *)arg);
	if(len > MAX_NAME_LEN) {
		len = MAX_NAME_LEN;
	} 
	strncpy(pInfo->name, arg, len);

	pthread_exit((void *)pInfo);

	//return NULL;
}


int main(void)
{

	pthread_t tid;

	char *msg = "Hello, 1510";
	struct person_info *pInfo = NULL;

	printf("thread demo!\n");

	pthread_create(&tid, NULL, sub_thread_routine, (void *)msg);
	printf("after pthread_create()\n");

	pthread_join(tid, (void *)&pInfo);
	printf("after pthread_join()\n");
	
	if(pInfo) {
		printf("The return value: sex=%d, name=%s, age=%d\n", pInfo->sex, pInfo->name, pInfo->age);
		free(pInfo);	
	}

	return 0;
}
