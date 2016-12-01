
/* TCP服务器程序 */

/* usage: ./server [serv_port] */
#include "net.h"

void *cli_data_handler (void *arg);

void usage (char *s)
{
	printf ("Usage:\n\t%s [serv_port]\n\n", s);
}

int main (int argc, char *argv[])
{
	int fd = -1, newfd = -1;

	struct sockaddr_in sin;

	int serv_port = SERV_PORT;

	/* 参数处理 */
	if (argc > 2) {
		usage (argv[0]);
		exit (EXIT_FAILURE);
	}
	if (argc == 2) {
		serv_port = atoi (argv[1]);
	}

	/* 1.创建套接字 */
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		perror ("socket");
		exit (1);
	}

	/* 2. 绑定 */
	/*2.1 填充sockaddr_in结构体，填充绑定的IP地址和端口号 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons (SERV_PORT);
#if 0
	if (inet_pton (AF_INET, SERV_IP, &sin.sin_addr) != 1) {
		perror ("inet_pton");
		exit (EXIT_FAILURE);
	}
#else
	sin.sin_addr.s_addr = htonl (INADDR_ANY);
#endif
	bzero (sin.sin_zero, 8);

	/* 允许地址快速重用 * */
	int b_reuse = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof (int));

	/* 2.2 调用bind()函数去绑定在本地的某个IP地址+端口上 */
	if (bind (fd, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
		perror ("bind");
		exit (EXIT_FAILURE);
	}

	/* 3.把套接字变成被动套接字 */
	listen (fd, BACKLOG);
	printf ("server staring....OK!\n");

	/*4. 阻塞等待客户端连接 */
	while (1) {

		struct sockaddr_in cin;
		int clen = sizeof (cin);
		pthread_t tid;
		struct cli_info *pInfo = NULL;

		newfd = accept (fd, (struct sockaddr *) &cin, &clen);	/*阻塞等待客户端的连接请求 */
		pInfo = (struct cli_info *) malloc (sizeof (struct cli_info));

		/* 填充客户端信息的的结构体 */
		pInfo->cli_fd = newfd;
		memcpy (&pInfo->cin, &cin, sizeof (cin));

		//FIXME!!
		//...把pInfo加到主线程的客户端信息链表中 

		pthread_create (&tid, NULL, cli_data_handler, (void *) pInfo);
		pthread_detach (tid);
	}
	close (fd);
	return 0;
}

void *cli_data_handler (void *arg)
{
	struct cli_info *pInfo = (struct cli_info *) arg;
	if (!pInfo)
		return NULL;

	int newfd = pInfo->cli_fd;
	int ret = -1;
	char buf[BUFSIZ];
	char cli_ip_addr[16];
	//pthread_detach(pthread_self());

	if (inet_ntop (AF_INET, &pInfo->cin.sin_addr.s_addr, cli_ip_addr, sizeof (pInfo->cin)) != NULL) {
		printf ("Client(%s:%d) connected.\n", cli_ip_addr, ntohs (pInfo->cin.sin_port));
	} else {
		perror ("inet_ntop");
	}

	/* 读写 */
	while (1) {
		bzero (buf, BUFSIZ);
		do {

			ret = read (newfd, buf, BUFSIZ - 1);
		} while (ret < 0 && EINTR == errno);
		if (ret < 0) {			//出错
			perror ("read");
		}

		if (!ret || !strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	/* read 的返回值为0: 表示对方关闭 */
			printf ("Client(%s:%d) disconnected.\n", cli_ip_addr, ntohs (pInfo->cin.sin_port));
			close (newfd);
			break;
		}

		printf ("Receive data: %s\n", buf);

	}

	return NULL;
}
