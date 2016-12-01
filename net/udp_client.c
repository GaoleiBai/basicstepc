/*TCP客户端程序 */
/*usage: ./client serv_ip [serv_port] */

#include "net.h"

void usage(char *s)
{
	printf("Usage:\n\t%s serv_ip [serv_port]\n\n", s);
}

int main(int argc, char *argv[])
{
        int fd = -1, ret = -1;
	char buf[BUFSIZ];

        struct sockaddr_in sin;

	int serv_port = SERV_PORT;

	/* 参数处理*/
	if(argc <2 || argc >3 ) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if(argc == 3) {
		serv_port = atoi(argv[2]);
	}

        /* 1.创建UDP套接字*/
        if( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("socket");
                exit(EXIT_FAILURE);
        }

	/* 2. 连接到服务器 */
	 /*2.1 填充sockaddr_in结构体，填充服务器的IP地址和端口号 */
        sin.sin_family = AF_INET;
        sin.sin_port = htons(SERV_PORT);
        sin.sin_addr.s_addr = inet_addr(argv[1]);
        
	bzero(sin.sin_zero, 8);

	printf("Simple UDP demo starting ....OK!\n");
        
	/* 3. 写套接字 */
        /* 从标准键盘上读入数据,写入套接字，直到用户输入quit就退出 */
        while (1) {
                fprintf (stderr, "writer: please input string:");
                bzero (buf, BUFSIZ);
                if (fgets (buf, BUFSIZ - 1, stdin) == NULL) {
                        perror ("fgets");
                        continue;
                }

		if( sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
			perror("sendto");
			continue;
		}

                if (!strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {  //用户输入quit
                        break;
                }
        }

        close (fd);
}
