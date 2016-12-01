/* 实现父子进程对资源的互斥操作 */
#include <stdlib.h>

#include "sem_com.h" 

int main(void)
{
	int semid = -1;
	int pid = -1;
    /*1.申请信号量 */
	semid = semget(ftok(".", 'b'), 1, IPC_CREAT|0666);
	if(semid < 0) {
		perror("semget error");
		exit(1);
	}	
	/* 2.初始化信号量 */
	init_sem(semid, 0, 1);  //资源初始化成可用
	
	/*创建子进程 */
	if( (pid = fork()) <0) {
		perror("fork");
		exit(1);
	}

	if(pid >0) { //父进程
	   sem_p(semid, 0, 1);  //P操作

	   //操作资源
	   //...
	
	   sem_v(semid, 0, 1); //V操作
	
		
	} else {  //子进程
	   sem_p(semid, 0, 1);  //P操作
	  
	   //操作资源
       //...
	  sem_v(semid, 0, 1); //V操作
	 
       //删除信号量
       del_sem(semid);	
	}
			
	return 0;
}

