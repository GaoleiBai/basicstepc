
/*TCP客户端程序 */

/*usage: ./client serv_ip [serv_port] */

#include "net.h"

int input_handle (int sock_fd, char *buf, int buf_max_len);
int data_from_serv_handle(int sock_fd, char *buf, int buf_max_len);

void usage (char *s)
{
	printf ("Usage:\n\t%s serv_ip [serv_port]\n\n", s);
}

int main (int argc, char *argv[])
{
	int fd = -1, ret = -1;
	char buf[BUFSIZ];

	struct sockaddr_in sin;

	int serv_port = SERV_PORT;

	/* 参数处理 */
	if (argc < 2 || argc > 3) {
		usage (argv[0]);
		exit (EXIT_FAILURE);
	}

	if (argc == 3) {
		serv_port = atoi (argv[2]);
	}

	/* 1.创建套接字 */
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		perror ("socket");
		exit (EXIT_FAILURE);
	}

	/* 2. 连接到服务器 */
	/*2.1 填充sockaddr_in结构体，填充服务器的IP地址和端口号 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons (SERV_PORT);
	sin.sin_addr.s_addr = inet_addr (argv[1]);

	bzero (sin.sin_zero, 8);

	if (connect (fd, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
		perror ("connect");
		exit (EXIT_FAILURE);
	}

	printf ("Client starting ....OK!\n");

	/*  1. 定义fd_set的集合rset */
	fd_set rset;
	int maxfd = -1;
	struct timeval tout;

	while (1) {
			
		tout.tv_sec = 5;
		tout.tv_usec = 0;
	
		/*２.把rset清零 */
		FD_ZERO (&rset);
		/* 3.依次把所有要监控的文件描述符加入到集合rset中，同时记录一下maxfd */
		FD_SET (0, &rset);
		FD_SET (fd, &rset);
		maxfd = fd;
		/*4.调用select()函数 */
		ret = select (maxfd + 1, &rset, NULL, NULL, &tout);
		/* 5. 依次判断那些被监控的文件描述上是否有数据 */
		if (FD_ISSET (0, &rset)) {	/* 标准键盘上有输入 */
			bzero (buf, BUFSIZ);
			input_handle (fd, buf, BUFSIZ - 1);
		}
		if (FD_ISSET (fd, &rset)) {	/* 套接字上有数据 */
			bzero(buf, BUFSIZ);
			data_from_serv_handle(fd, buf, BUFSIZ-1);					
		}
		/* 6. 确认一下是否是超时返回*/
		if(!ret || (tout.tv_sec == 0 && tout.tv_usec ==0)) {
			printf("Timeout\n");
		} 
	}

	close (fd);
}

int data_from_serv_handle(int sock_fd, char *buf, int buf_max_len)
{
	int ret = -1;
	       
	do {

                        ret = read (sock_fd, buf, BUFSIZ - 1);
          } while (ret < 0 && EINTR == errno);
                if (ret < 0) {                  //出错
                        perror ("read");
			return -1;
          }
		
          //按照协议拆包
	  //FIXME!!!
	  //...
		
	  printf("Recive message:%s\n", buf);
	  return 0;	
}

int input_handle (int sock_fd, char *buf, int buf_max_len)
{
	int ret = -1;
	/* 从标准键盘上读入数据,写入套接字，直到用户输入quit就退出 */

	//FIXME!!! 提示信息可能需要改动
	fprintf (stderr, "writer: please input string:");

	if (fgets (buf, buf_max_len, stdin) == NULL) {
		perror ("fgets");
		return -1;
	}
	
	/*把读到的字符串按照协议打包 */
	//FIXME!!!!
	//...
	do {
		ret = write (sock_fd, buf, strlen (buf));
	} while (ret < 0 && EINTR == errno);
	if (ret < 0) {
		perror ("write");
		return -1;
	}

	if (!ret || !strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入quit
		return 0;
	}

	return 0;
}
