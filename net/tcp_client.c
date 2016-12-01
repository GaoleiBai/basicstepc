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

int main(void)
{
        int ret = -1;
        int fd = -1;
        struct sockaddr_in sin;


        /*1. 建立套接字 */
        if( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //ipV4的TCP编程
        {
                perror("socket");
                exit(1);
        }
	
	/*2.连接服务器 */
	/*2.1 填充要连接的服务器信息 */
	 bzero(&sin, sizeof(sin));
         sin.sin_family = AF_INET;
         sin.sin_port = htons(SERV_PORT);
         sin.sin_addr.s_addr = inet_addr(SERV_IP);
	 /*2.2 连接服务器 */
	 if( connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	 	printf("Client didn't conneted Server!\n");
		perror("connet");
		exit(1);
	 }
 
	/*和服务器数据交互*/
	char buf[BUFSIZ];
	while(1) {
		printf("Please input the string: ");
		bzero(buf, BUFSIZ);
		if(fgets(buf, BUFSIZ-1, stdin) == NULL) { /* 从标准输入端获取了了键盘输入*/
			perror("fgets error");
			continue;
		}		
		/* 写到套接字上去 */	
		do {
	        	ret = write(fd, buf, strlen(buf));
		}while(ret < 0 && EINTR == errno);
		if(ret < 0) {
			perror("write to socket error");
			continue;
		}
		
		if(!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR))) { /* 用户输入了quit*/
			break;
		} 
	}
	
	close(fd);

	return 0;	
}
