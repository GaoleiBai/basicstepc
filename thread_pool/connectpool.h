////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////

#ifndef D_CONNECTPOOL_H
#define D_CONNECTPOOL_H

#include <sys/epoll.h>

#include "d_linklist.h"
#include "d_const.h"

typedef struct{
	time_t ActiveTime;	// 最近一次的活动时间
	int fd;				// 占用当前通道的句柄
	int Registed;		// 标识是否被注册
	int ShutSig;		// 标识是否应该关闭通道
	int CopyCnt;		// 处理当前通道消息的线程数
	pthread_mutex_t CntMtx; // 通道处理互斥
	char vin[VEH_IDLEN+1];		// 当前通道的VIN码
}LINK_INFO;

extern pthread_mutex_t stop_mutex;
extern LINK_INFO * pConnects;


/**********************************************************************/
/** @note 空闲连接链表管理*/
/**********************************************************************/

int IdleConnectLinker_create(linklist_pt * list);
int IdleConnectLinker_destroy(linklist_pt list);
int IdleConnectLinker_add(linklist_pt list, int * elem);
int IdleConnectLinker_remove(linklist_pt list, int index);
int IdleConnectLinker_isEmpty(linklist_pt list);

/**********************************************************************/
/** @note 每个连接通道对应一个epoll的事件*/
/**********************************************************************/

int createConnectArray(LINK_INFO **links, int len);
int destroyConnectArray(LINK_INFO *links);

int createEvtArray(struct epoll_event ** events, int len);
int destroyEvtArray(struct epoll_event * events);


/**********************************************************************/
/** @note 每个连接通道关闭事件*/
/**********************************************************************/
void clearConnect(LINK_INFO * connect);
void initConnect(LINK_INFO * connect, int Fd);
void closeConnect(linklist_pt IdleLinker, int epollFd, LINK_INFO connect, int index);
void stopConnect(linklist_pt IdleLinker, int epollFd, LINK_INFO connect, int index, int useCnt, char logo);

#endif
