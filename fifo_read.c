
/* 实现一个有名管道：fifo_write: 写, fifo_read: 读  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define QUIT_STR "quit"
#define FIFO_FILE "/tmp/myfifo.1"
int main (void)
{
	int ret, fd = -1;
	char buf[BUFSIZ];

	if (access (FIFO_FILE, F_OK | R_OK) < 0) {	/*如管道文件事先不存在或不可读，则退出 */
		perror("access");
		exit (1);
	}


	/* 1.以读的方式打开管道文件 */
	if ((fd = open (FIFO_FILE, O_RDONLY)) < 0) {
		perror ("open");
		exit (1);
	}
	printf("Fifo reader starting ....OK!\n");

	/* 3. 读管道 */
	while (1) {
		bzero (buf, BUFSIZ);

		do {
			ret = read (fd, buf, BUFSIZ - 1);	/* 读fd 管道 */
		} while (ret < 0 && EINTR == errno);
		if (ret < 0) {
			perror ("read");
			continue;
		}
		printf ("reader---read data: %s\n", buf);
		if (!strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入quit
			break;
		}
	}


	close (fd);

	return 0;
}
