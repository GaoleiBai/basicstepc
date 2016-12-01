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

#define SHM_SIZE 2048

#define NUM_MAX 100
#define NAME_MAX_LEN 32

struct person_info
{
//  struct list_head list;
	int sex;
	char name[NAME_MAX_LEN + 1];
	int age;

};

int main (void)
{
	int shm_id = -1;

	struct person_info *pInfo = NULL;

	printf ("before shmget()\n");
	system ("ipcs -m");

	/* 1.创建共享内存 */
	if ((shm_id = shmget (ftok (".", 'a'), NUM_MAX * sizeof (struct person_info), IPC_CREAT | 0666)) < 0) {
		perror ("shmget");
		exit (EXIT_FAILURE);
	}

	printf ("before shmat()\n");
	system ("ipcs -m");

	/*2.映射共享内存到用户空间 */
	pInfo = (struct person_info *) shmat (shm_id, NULL, 0);
	if (pInfo == (void *) -1) {
		perror ("shmat");

		//FIMXE!!! 
		//是否要删除共享内存

		exit (EXIT_FAILURE);
	}

	printf ("after shmat()\n");
	system ("ipcs -m");


	/*3. 使用 */
	//...

	/* 4.撤销映射 */
	shmdt (pInfo);

	printf ("after shmdt()\n");
	system ("ipcs -m");


	/* 删除共享内存 */
	shmctl (shm_id, IPC_RMID, NULL);

	printf ("after shmctl()\n");
	system ("ipcs -m");

	return 0;
}
