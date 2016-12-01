#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler(int signo)
{
	printf("Receive a SIGALRM signal\n");
}

int main(void)
{
	printf("alarm demo\n");
	
	signal(SIGALRM, alarm_handler);
	
	printf("before alarm\n");
	alarm(5);
	printf("after alarm\n");

	pause();
	printf("after pause\n");

	return 0;
}
