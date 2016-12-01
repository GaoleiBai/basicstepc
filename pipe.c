#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define QUIT_STR "quit"

int main (void)
{
	char buf[BUFSIZ];

	int ret = -1;
	pid_t pid = -1;
	int fd[2];


	printf ("pipe demo!\n");

	if (pipe (fd) < 0) {
		perror ("pipe");
		exit (EXIT_FAILURE);
	}
	if ((pid = fork ()) < 0) {
		perror ("fork");
		exit (EXIT_FAILURE);
	}

	if (pid > 0) {				//父进程读管道
		close (fd[1]);			//关闭写端

		while (1) {
			bzero (buf, BUFSIZ);
			do {
				ret = read (fd[0], buf, BUFSIZ - 1);
			} while (ret < 0 && EINTR == errno);
			if (ret < 0) {
				perror ("read");
				continue;
			}

			if (!ret) {
				printf ("pipe is closed.\n");
				break;
			}
			printf ("Parent: read %s\n", buf);

			if (!strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入了quit
				break;
			}

		}
		close (fd[0]);

	} else {					//0 == pid,子进程内写管道
		close (fd[0]);			//关闭子进程的读端

		while (1) {
			fprintf(stderr, "Please input string:");
			bzero (buf, BUFSIZ);
			if(fgets (buf, BUFSIZ - 1, stdin) == NULL) {	//从标准键盘上获取数据    
				continue;
			}
			//buf[BUFSIZ-1] = '\0';
			do {
				ret = write (fd[1], buf, strlen (buf));
			} while (ret < 0 && EINTR == errno);

			if (!strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入了quit
				close (fd[1]);
				break;
			}

		}
	}



	return 0;
}
