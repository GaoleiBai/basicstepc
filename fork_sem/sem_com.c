#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "sem_com.h"

/*自己封装的信号量的操作函数 */
int init_sem(int semid, int sem_num, int val)
{
	union semun sun;
	sun.val = val;
	return semctl(semid, sem_num, SETVAL, sun);

}

int sem_p(int semid, short sem_num, size_t nops)
{
	struct sembuf buf;
	buf.sem_num = sem_num;
	buf.sem_op = -1; 
        buf.sem_flg = SEM_UNDO;
	return semop(semid,&buf, nops);
}

int sem_v(int semid, short sem_num, size_t nops)
{
        struct sembuf buf;
        buf.sem_num = sem_num;
        buf.sem_op = 1;
        buf.sem_flg = SEM_UNDO;
        return semop(semid,&buf, nops);
}

int sem_try_p(int semid, short sem_num, size_t nops)
{
        struct sembuf buf;
        buf.sem_num = sem_num;
        buf.sem_op = -1;
        buf.sem_flg = IPC_NOWAIT|SEM_UNDO;
        return semop(semid,&buf, nops);
}

int del_sem(int semid)
{
	union semun sun; 
	return semctl(semid, 0, IPC_RMID,sun);
}
