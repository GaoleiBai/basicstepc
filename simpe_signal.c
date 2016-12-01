
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void ctrl_c_handler(int signo)
{
	if(signo == SIGINT) {
		printf("Receive a SIGINT signal");
	}	

}

int main(void)
{
	printf("signal_demo");
    
	/* 处理信号的三种方法 */
	signal(SIGSEGV, SIG_DFL); /* 1. 默认处理, DFL: default*/
	signal(SIGQUIT, SIG_IGN); /* 2. 忽略信号  IGN: ignore*/
	
    signal(SIGINT, ctrl_c_handler);/* 3. 捕获信号*/

	while(1) {
		printf("I'm in loop\n");
		usleep(200000); //sleep 0.2s
	}
	
	return 0;
}	
