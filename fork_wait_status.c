#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid = -1;

	printf("I'm parent process!\n");
	
	pid = fork();
	if(pid < 0 ) { //出错
		perror("fork");
		exit(1);
	}		

	printf("aaaa\n");	

	if(pid > 0) { //父进程中的返回
		int status, ret;
		
//		usleep(100000); //0.1s,让子进程先运行 
		printf("I'm parent, pid =%d, child process PID =%d\n", getpid(), pid);
		ret = wait(&status); //阻塞等待子进程退出
		printf("partent process: ret =%d, status = %d, child exit value: %d\n", ret, status, WEXITSTATUS(status));

		//查看子进程的退出状态
		if(WIFEXITED(status)) {
			printf("the child terminated normally!\n");	
		}	

		if(WIFSIGNALED(status)){
			printf("the child process was terminated by a signal!\n");
		}

	} else {  // 0 = pid,子进程中返回 
		printf("I'm child process, pid =%d, ppid=%d\n", getpid(), getppid());
		sleep(2); //模拟现实耗时操作
		printf("child is exiting...\n");
		exit(1);
	}	

	return 0;
}
