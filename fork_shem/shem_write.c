#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define QUIT_STR "quit"

#define SHM_SIZE BUFSIZ

int main (void)
{
	int shm_id = -1;

	char *shm_buf = NULL;

	/* 1.创建共享内存 */
	if ((shm_id = shmget (ftok (".", 'a'), SHM_SIZE, IPC_CREAT | 0666)) < 0) {
		perror ("shmget");
		exit (EXIT_FAILURE);
	}


	/*2.映射共享内存到用户空间 */
	shm_buf = (char *) shmat (shm_id, NULL, 0);
	if (shm_buf == (void *) -1) {
		perror ("shmat");

		//FIMXE!!! 
		//是否要删除共享内存

		exit (EXIT_FAILURE);
	}


	/*3. 使用 */
	printf ("Share Memory writer starting ....OK!\n");

	/* 从标准键盘上读入数据,写入管道，直到用户输入quit就退出 */
	while (1) {
		fprintf (stderr, "writer: please input string:");
//		bzero (shm_buf, BUFSIZ);

		if (fgets (shm_buf, BUFSIZ - 1, stdin) == NULL) {
			perror ("fgets");
			continue;
		}

		if (!strncasecmp (shm_buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入quit
			break;
		}
	}


	/* 4.撤销映射 */
	shmdt(shm_buf);

	/* 删除共享内存 */
	shmctl (shm_id, IPC_RMID, NULL);


	return 0;
}
