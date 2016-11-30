////////////////////////////////////////////////////////////////
/// COPYRIGHT POLELINK
/// Copyright (c) 2013
/// All rights reserved.
///  
/// @file  d_sysinit.c
/// @brief 系统初始化
///
/// @version $Revision: 0 $
/// @author Wuchunyu
/// @date $Date: 2013-04-13 22:51:31 +0800 (Sat, 13 Apr 2013) $
///
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>			/* socket类定义需要 */
#include <netinet/tcp.h>		/* socket类定义需要 */
#include <arpa/inet.h>			/* socket字符串转化 */
#include <errno.h>
#include <pthread.h>
#include <string.h>

#include "d_syscntl.h"
#include "t_message.h"

int In_MaxThread = 300;


/** 设置系统可打开的最大文件数目
 * 
 * @param 无
 * @return    无
 * @note 一般由系统控制
 */
void setMaxFile(void) 
{
	struct rlimit rlt;
	rlt.rlim_max = rlt.rlim_cur = MaxFileNO;
	fprintf(stderr, "Set OS limit max open files to %d > ", MaxFileNO);
	if( setrlimit( RLIMIT_NOFILE, &rlt ) == -1 )
	{
		fprintf(stderr, "Error : %d（please run as root）\n", errno);
		syslog(LOG_ERR, "Setrlimit MaxOpenFile Error : %d（please run as root）\n", errno);
		exit( EXIT_FAILURE );
	}
}


/** 设置每个线程的最大堆栈
 * 
 * @param signo 信号类型
 * @return    无
 * @note 一般由系统控制
 */
void setMaxStack(void) 
{
	fprintf(stderr, "Set OS limit stack size to %10.3f Kbytes > ", MaxStack/1024.0);
	pthread_attr_t thr_attr, *pAttr = NULL;
	if(pAttr == NULL)
	{
		pAttr = &thr_attr;
		pthread_attr_init(pAttr);
		/** @attention 更改系统堆栈为1M，区别于setrlimit */
		pthread_attr_setstacksize(pAttr, MaxStack);
		pthread_attr_setdetachstate(pAttr, PTHREAD_CREATE_DETACHED);
	}
	fprintf(stderr, "OK\n");
}



/** 系统配置初始化
 * 
 * @param signo 信号类型
 * @return    无
 * @note  系统配置初始化
 */
void initSysConf( void ) 
{
	/** @note 消息标识 */
	openlog("DXGD", LOG_CONS | LOG_PID, 0);
	syslog(LOG_INFO, "DXG daemon program's pid=%d\n", getpid());
	
	//  setMaxFile();
	//  setMaxStack();
}



/** 建立TCP服务器端
 * 
 * @param void 无
 * @return 0：成功 -1：失败
 * @note  初始化固定数量线程处理与客户端的通讯
 */
int createTCPSrv(int Srv_Port, int Max_Back)
{
	int listen_fd;
	unsigned int optval;			/**< socket可重用参数 */
	struct sockaddr_in	servaddr;
	struct linger		optval1;	/**< socket关闭时操作参数 */
	
	/** @note 初始化服务器一端地址接口 */
	bzero( &servaddr, sizeof( servaddr ) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	servaddr.sin_port = htons( Srv_Port );
	
	/** @note 建立套接字 */
	if( ( listen_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		fprintf(stderr, "Socket Error...%d\n" , errno );
		syslog(LOG_ERR, "Socket Error...%d\n" , errno );
		exit( EXIT_FAILURE );
	}
	
	{
	//	int keepalive = 1;	//!> 打开TCP KEEPALIVE开关
	//	int keepidle = 20;	//!> 系统等待多长时间开始检测 14400=2 hours
	//	int keepintvl = 5;	//!> 系统每次探测时间 150=75s
	//	int keepcnt = 3;	//!> 系统探测几次后执行关闭操作
	//	
	//	/** @note 非阻塞模式设置 */
	//	/** @note 暂时屏蔽非阻塞，由于这里使用线程互斥完成数据通讯，应该是不需要的，有待商榷 */
	//	if( setnonblocking( listen_fd ) == -1 )
	//	{
	//		fprintf(stderr, "Setnonblocking Error : %d\n", errno);
	//		exit( EXIT_FAILURE );
	//	}
	
	//	ret = setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepalive, sizeof(keepalive));
	//	if(ret == 0)
	//	{
	//		fprintf(stderr, "bang");
	//		setsockopt(listen_fd, SOL_TCP,    TCP_KEEPIDLE, (char *)&keepidle,  sizeof(keepidle));
	//		setsockopt(listen_fd, SOL_TCP,    TCP_KEEPINTVL,(char *)&keepintvl, sizeof(keepintvl));
	//		setsockopt(listen_fd, SOL_TCP,    TCP_KEEPCNT,  (char *)&keepcnt,   sizeof(keepcnt));
	//	}else{
	//		
	//		fprintf(stderr, "error\n");
	//		exit(0);
	//	}
	}
	
	/** @note 设置套接字可重用 */
	/** @note 情况1： 当有一个有相同本地地址和端口的socket1处于TIME_WAIT状态时，而你启动的程序的socket2要占用该地址和端口，你的程序就要用到该选项 */
	/** @note 情况2： 允许同一port上启动同一服务器的多个实例(多个进程)，但每个实例绑定的IP地址是不能相同的 */
	/** @note 情况3： 允许单个进程绑定相同的端口到多个socket上，但每个socket绑定的ip地址不同 */
	/** @note 情况4： 允许完全相同的地址和端口的重复绑定。但这只用于UDP的多播，不用于TCP */
	optval = 0x1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, 4);
	
	/** @note 套接口关闭时内核将拖延1s */
	optval1.l_onoff = 1;
	optval1.l_linger = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_LINGER, &optval1, sizeof(struct linger));
	
	/** @note 绑定 */
	if( bind( listen_fd, ( struct sockaddr *)&servaddr, sizeof( struct sockaddr ) ) == -1 )
	{
		fprintf(stderr, "Bind Error : %d\n", errno);
		syslog(LOG_ERR, "Bind Error : %d\n", errno);
		exit( EXIT_FAILURE );
	}

	/** @note 监听 */
	if( listen( listen_fd, Max_Back ) == -1 )
	{
		fprintf(stderr, "Listen Error : %d\n", errno);
		syslog(LOG_ERR, "Listen Error : %d\n", errno);
		exit( EXIT_FAILURE );
	}
	
	return listen_fd;
}


void close_FD(int fd)
{
	close(fd);
}
