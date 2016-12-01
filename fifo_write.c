
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

	umask(0);

	if (!access (FIFO_FILE, F_OK)) {	/*如管道文件事先存在，则删除 */
		unlink (FIFO_FILE);
		printf("unlink %s\n", FIFO_FILE);
	}
	
	/* 1.创建管道文件 */
	if (mkfifo (FIFO_FILE, 0666) < 0) {
		perror ("mkfifo");
		exit (1);
	}
	printf("after mkfifo!\n");
	/* 2.打开管道文件 */
	if ((fd = open (FIFO_FILE, O_RDWR)) < 0) {
		perror ("open");
		exit (1);
	}
	printf("Fifo writer starting ....OK!\n");
	/* 3. 写管道 */
	/* 从标准键盘上读入数据,写入管道，直到用户输入quit就退出 */
	while (1) {
		fprintf (stderr, "writer: please input string:");
		bzero (buf, BUFSIZ);
		if (fgets (buf, BUFSIZ - 1, stdin) == NULL) {
			perror ("fgets");
			continue;
		}

		do {
			ret = write (fd, buf, strlen (buf));	/* 写fd 管道 */
		} while (ret < 0 && EINTR == errno);
		if (ret < 0) {
			perror ("write");
			continue;
		}

		if (!ret || !strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入quit
			break;
		}
	}

	close (fd);

	return 0;
}
