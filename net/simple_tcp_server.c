#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define QUIT_STR "quit"
#define SERV_IP  "192.168.7.4"
#define SERV_PORT 5002
#define BACKLOG 5
int main (void)
{
	int ret = -1;
	int fd = -1, newfd = -1;
	struct sockaddr_in sin;


	/*1. 建立套接字 */
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)	//ipV4的TCP编程
	{
		perror ("socket");
		exit (1);
	}

	/*2.绑定本机IP和端口号 */
	/* 2.1 填充sockaddr_In结构体 */
	bzero (&sin, sizeof (sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons (SERV_PORT);
	sin.sin_addr.s_addr = inet_addr (SERV_IP);
	/*2.2 绑定 */
	if (bind (fd, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
		perror ("bind");
		exit (1);
	}

	/*3.把套接字变为被动套接字 */
	if (listen (fd, BACKLOG) < 0) {
		perror ("listen");
		exit (1);
	}


	printf ("Server starting....OK!\n");
	
	/* 4. 阻塞等待客户端链接 */
#if 0
	newfd = accept (fd, NULL, NULL);	/* 不关心客户端是谁 */
#else
	/*取客户端信息 */
	struct sockaddr_in cin;
	socklen_t cli_len = sizeof (cin);
	char cli_ip_addr[16];
	newfd = accept (fd, (struct sockaddr *) &cin, &cli_len);

	if( (newfd > 0) && (inet_ntop(AF_INET, (void *)&cin.sin_addr, cli_ip_addr, sizeof(cin)) != NULL) ) {
		printf("A new(ip =%s, port =%d) client conneted.\n", cli_ip_addr, ntohs(cin.sin_port) );	
	} else {
		perror("inet_ntop() error");
	}

#endif
	if (newfd < 0) {
		perror ("accept error");
		exit (1);				//可能不一定合适
	}

	/*5.和客户端进行数据交互 */
	//....
	char buf[BUFSIZ];
	while (1) {
		bzero (buf, BUFSIZ);
		do {
			ret = read (newfd, buf, BUFSIZ - 1);
		} while (ret < 0 && EINTR == errno);

		if (ret < 0) {
			perror ("read error");
			exit (1);			//FIXME!!!!, there is a bug
		}
		if (ret == 0) {
			printf ("Client is exited.");
		}

		printf ("Received data: %s\n", buf);
		

                if(!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR))) { /* 用户输入了quit*/
              
			/*6.数据交互完成，关闭套接字 */
			close (newfd);
		        break;
                }

	}


	close (fd);
	return 0;
}
