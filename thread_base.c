#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>

#include <pthread.h>

void *sub_thread_routine(void * arg)
{
	int i;
	printf("I'm sub thread,my thread ID is:0x%x, arg =%s\n", (int)pthread_self(), (char *)arg);
	
	for (i = 0; i < 20; i++) {
		printf("I'am in sub thread, i =%d\n", i);
		usleep(200000); //sleep 0.2s
	}
		
	pthread_exit(NULL);
	//return NULL;
}


int main(void)
{

	pthread_t tid;

	char *msg = "Hello, 1510";

	printf("thread demo!\n");

	pthread_create(&tid, NULL, sub_thread_routine, (void *)msg);
	printf("after pthread_create()\n");
	pthread_join(tid, NULL);
	printf("after pthread_join()\n");

	return 0;
}
