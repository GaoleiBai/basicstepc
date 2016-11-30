////////////////////////////////////////////////////////////////
/// COPYRIGHT Polelink
/// Copyright (c) 2013
/// All rights reserved.
/// 
/// @file  d_threadpool.h
/// @brief 线程池处理相关
///
/// @version $Revision: 0 $
/// @author Wuchunyu
/// @date $Date: 2013-03-25 10:21:33 +0800 (Mon, 25 Mar 2013) $
///
////////////////////////////////////////////////////////////////

#ifndef D_THREADPOOL_H
#define D_THREADPOOL_H

#include <pthread.h>

#include "d_connectpool.h"
#include "d_linklist.h"


#define THREADINFO_NO 3
#define THREADPOLL_BASESIZE 300
#define THREADPOLL_WARNSIZE 10
#define THREADPOLL_ELEMSIZE 50

typedef int (*ThreadPara_pt)[THREADINFO_NO] ;

typedef struct{
	ThreadPara_pt tPara;
	pthread_t * tTid;
	pthread_mutex_t * tMtx;
	int capacity;
}T_ThreadInfo;

typedef T_ThreadInfo * ThreadInfo_pt;

typedef struct{
	ThreadInfo_pt entry;		// poollinker 入口点
	LINK_INFO * connects;
	linklist_pt IdleLinker;
	int epollFd;
	int index;					// poollinker 索引
}T_MsgEntry;

typedef T_MsgEntry * MsgEntry_pt;

int AThreadPool_create(ThreadInfo_pt *ppThreadInfo, int ThreadNO, LINK_INFO * Connects, linklist_pt pIdleLinker, int kEpollFd);
int AThreadPool_destroy(ThreadInfo_pt ppThreadPool);

int PoolLinker_create(linklist_pt * list);
int PoolLinker_destroy(linklist_pt list);
int PoolLinker_init(linklist_pt * list, LINK_INFO * Connects, linklist_pt pIdleLinker, int kEpollFd);
int PoolLinker_add(linklist_pt list, ThreadInfo_pt elem);
int PoolLinker_getElemI(linklist_pt list, int index, ThreadInfo_pt * pElem);
int PoolLinker_getCapacity(linklist_pt list);
int PoolLinker_expandCapacity(linklist_pt list, LINK_INFO * Connects, linklist_pt pIdleLinker, int kEpollFd);

#endif
