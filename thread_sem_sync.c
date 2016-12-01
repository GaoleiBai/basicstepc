#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

#define QUIT_STR "quit"
#define MAX_NAME_LEN 32
struct person_info {
	int sex;
	char name[MAX_NAME_LEN+1];
	int age;
	//...
	sem_t sem_w;
	sem_t sem_r;
};

//sem_t sem;  /* 无名信号量 */

void *write_thread_routine(void * arg)
{
	int i;
	char buf[BUFSIZ];
	struct person_info *pInfo = (struct person_info *)arg;

	
	printf("I'm sub thread,my thread ID is:0x%x\n", (int)pthread_self());
	
	if(!pInfo) {	/* 内存如果为空，则退出*/
		pthread_exit(NULL);
	}


      /* 从标准键盘上读入数据,写入pInfo->name，直到用户输入quit就退出 */
        while (1) {
		sem_wait(&pInfo->sem_w);  //Pw操作
                
		fprintf (stderr, "writer: please input string:");
                bzero (buf, BUFSIZ);
                if (fgets (buf, BUFSIZ - 1, stdin) == NULL) {
                        perror ("fgets");
                        continue;
                }
		
		int len = strlen(buf);
		if(len > MAX_NAME_LEN) {
			len = MAX_NAME_LEN;
		}
//		sem_wait(&pInfo->sem_w);  //Pw操作 
	        pInfo->sex = 1;
	        pInfo->age = 18;
	
		bzero(pInfo->name, MAX_NAME_LEN +1);
		strncpy(pInfo->name, buf, len);

                sem_post(&pInfo->sem_r);  //Vr操作

		if ( !strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {  //用户输入quit
                        break;
                }
        }
	

	pthread_exit((void *)pInfo);

	//return NULL;
}

void read_thread_routine(void *arg)
{
	struct person_info *pInfo = (struct person_info *)arg;

	printf("I'm read thread!\n");
	
	if(!pInfo) {
		 pthread_exit(NULL);
	}

	while(1) {
		sem_wait(&pInfo->sem_r);  //Pr操作
	
                if ( !strncasecmp (pInfo->name, QUIT_STR, strlen (QUIT_STR))) {  //用户输入quit
                        sem_post(&pInfo->sem_w);  //Vw操作
			break;
                }
		printf("read: sex = %d, age=%d, name=%s\n", pInfo->sex, pInfo->age, pInfo->name);

		sem_post(&pInfo->sem_w);  //Vw操作
	}	 

        pthread_exit(NULL);
}

int main(void)
{

	pthread_t tid_w, tid_r;

	char *msg = "Hello, 1510";
	struct person_info *pInfo = NULL;

	printf("thread demo!\n");
	
	pInfo = (struct person_info *)malloc(sizeof(struct person_info));
        if(!pInfo) {    /* 内存分配不成功*/
                exit(EXIT_FAILURE);
        }


	/* 初始化信号量: 初始化成可用*/
	sem_init(&pInfo->sem_w, 0, 1);
	sem_init(&pInfo->sem_r, 0, 0);

	pthread_create(&tid_w, NULL, write_thread_routine, (void *)pInfo);
	pthread_create(&tid_r, NULL, (void *)read_thread_routine, (void *)pInfo);
	
	pthread_join(tid_w, NULL);
	pthread_join(tid_r, NULL); 

	if(pInfo) free(pInfo);
	
	return 0;
}
