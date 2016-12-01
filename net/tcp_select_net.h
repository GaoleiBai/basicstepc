#ifndef __QQ_NET_H__
#define __QQ_NET_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>			/* superset of previous */
#include <arpa/inet.h>

#define QUIT_STR "quit"

#define SERV_IP "192.168.7.6"
#define SERV_PORT 5001
#define BACKLOG 5

#include "list.h"

struct cli_info
{
	struct list_head list;
	int cli_fd;
	struct sockaddr_in cin;
	//...
};


#endif
