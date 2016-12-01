#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void ctrl_c_handler(int signo)
{
	if(signo == SIGINT) {
		printf("Receive a SIGINT signal");
		exit(EXIT_SUCCESS);
	}	

}

int main(void)
{
	pid_t pid = -1;
	printf("signal_demo");
    
	sigset_t set;
	/* 1.阻塞要处理的信号 */
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigaddset(&set, SIGQUIT|SIGINT);
	sigprocmask(SIG_BLOCK, &set, NULL);

	/*2.对各个信号安装处理句柄 */
#if 0 
	signal(SIGQUIT, SIG_IGN); /* 2. 忽略信号  IGN: ignore*/
        signal(SIGINT, ctrl_c_handler);/* 3. 捕获信号*/
#else
	
	struct sigaction act;

#if 1	
	/* SIGCHLD信号的处理*/
	act.sa_handler = SIG_DFL;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART|SA_NOCLDWAIT;
	sigaction(SIGCHLD, &act, NULL); 
#endif
	/* 安装SIGINT的处理方法*/
	act.sa_handler = ctrl_c_handler;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_flags = 0;
	sigaction(SIGINT, &act,NULL);
	/*忽略SIGQUIT */
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaction(SIGQUIT, &act, NULL);
#endif
	/* 3.放开前面的被阻塞的信号 */
	sigprocmask(SIG_UNBLOCK,&set,NULL);
	
	/* 4. 如果阻塞期间发生被阻塞的信号，现在可以送过来处理*/
	sigpending(&set);

	/* 因为前面安装了SIGCHLD的信号处理方法时，sa_flags设置了SA_NOCLDWAIT, 所以后续的代码中有fork()，那么父进程就不需要去wait()/waitpid() */
	pid = fork();
	switch (pid) {
		case -1: /* fork()出错*/
			perror("fork");
			exit(1);
		break;

		case 0: /* 子进程 */
			printf("Child Process: pid=%d, ppid=%d\n", getpid(), getppid());	
			sleep(2);
			printf("Child is exiting...\n");
			exit(EXIT_SUCCESS);
		break;

		default: /* 父进程 */
			while(1) {
				printf("I'm in loop\n");
				usleep(200000); //sleep 0.2s
			}
			break;
				
	}

	return 0;
}	
