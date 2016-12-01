#ifndef __SEM_COM_H__
#define  __SEM_COM_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifndef semun
union semun {
      int              val;    /* Value for SETVAL */
      struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
      unsigned short  *array;  /* Array for GETALL, SETALL */
     struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                          (Linux-specific) */
};
#endif

/*自己封装的信号量的操作函数 */
int init_sem(int semid, int sem_num, int val);
int sem_p(int semid, short sem_num, size_t nops);
int sem_v(int semid, short sem_num, size_t nops);
int sem_try_p(int semid, short sem_num, size_t nops);
int del_sem(int semid);

#endif
