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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "d_threadpool.h"
#include "t_message.h"


int createThreadParaPool(ThreadPara_pt * ppThreadPara, int len )
{
	int CellSize = len * THREADINFO_NO * sizeof(int);
	*ppThreadPara = (ThreadPara_pt)malloc( CellSize );
	memset(*ppThreadPara, 0, CellSize);
	if(NULL != *ppThreadPara)
		return 0;
	else
		return -1;
}

int destroyThreadParaPool(ThreadPara_pt pThreadPara)
{
	if(pThreadPara != NULL)
	{
		free(pThreadPara);
		return 0;
	}else{
		return -1;
	}
}

int createThreadTidPool(pthread_t ** ppThreadTid, int len )
{
	*ppThreadTid = ( pthread_t * )malloc( len * sizeof(pthread_t) );
	if(NULL != *ppThreadTid)
		return 0;
	else
		return -1;
}

int destroyThreadTidPool(pthread_t * pThreadTid)
{
	if(pThreadTid != NULL)
	{
		free(pThreadTid);
		return 0;
	}else{
		return -1;
	}
}


int createThreadMtxPool(pthread_mutex_t ** ppThreadMtx, int len )
{
	*ppThreadMtx = ( pthread_mutex_t * )malloc( len * sizeof(pthread_mutex_t) );
	if(NULL != *ppThreadMtx)
		return 0;
	else
		return -1;
}

int destroyThreadMtxPool(pthread_mutex_t * pThreadMtx)
{
	if(pThreadMtx != NULL)
	{
		free(pThreadMtx);
		return 0;
	}else{
		return -1;
	}
}

int createThreadPoolResources(ThreadInfo_pt * ppThreadPool, int ThreadNO)
{
	*ppThreadPool = ( ThreadInfo_pt )malloc( sizeof(T_ThreadInfo) );
	if(NULL == *ppThreadPool)
	{
		return -1;
	
	}else{
		
		if( createThreadParaPool(&((*ppThreadPool)->tPara), ThreadNO) )
			return -1;
		
		if( createThreadTidPool(&((*ppThreadPool)->tTid), ThreadNO) )
			return -1;
		
		if( createThreadMtxPool(&((*ppThreadPool)->tMtx), ThreadNO) )
			return -1;
		
		(*ppThreadPool)->capacity = ThreadNO;
		
		return 0;
	}
	
}

int destroyThreadPoolResources(ThreadInfo_pt pThreadPool)
{
	if(pThreadPool != NULL)
	{
		if( destroyThreadParaPool(pThreadPool->tPara) )
			return -1;
		
		if( destroyThreadTidPool(pThreadPool->tTid) )
			return -1;
		
		if( destroyThreadMtxPool(pThreadPool->tMtx) )
			return -1;
		
		free(pThreadPool);
		
		return 0;
		
	}else{
		
		return -1;
	}
}


/** 线程池初始化
 * 
 * @param void 无
 * @return 0：成功 -1：失败
 * @note  初始化固定数量线程处理与客户端的通讯
 */
int AThreadPool_create(ThreadInfo_pt *ppThreadInfo, int ThreadNO, LINK_INFO * Connects, linklist_pt pIdleLinker, int kEpollFd)
{
	ThreadInfo_pt pThreadInfo = NULL;
	if( createThreadPoolResources((&pThreadInfo), ThreadNO) )
		return -1;
	
	int i;
	for(i=0; i<ThreadNO; i++)
	{
		pThreadInfo->tPara[i][0] = 0;
		pThreadInfo->tPara[i][2] = i;
		pthread_mutex_lock( &((pThreadInfo->tMtx)[i]) );
		
		MsgEntry_pt kMsgEntry = (MsgEntry_pt)malloc( sizeof(T_MsgEntry) );
		kMsgEntry->entry    = pThreadInfo;
		kMsgEntry->connects = Connects;
		kMsgEntry->IdleLinker = pIdleLinker;
		kMsgEntry->epollFd = kEpollFd;
		kMsgEntry->index    = i;
		if( pthread_create(&((pThreadInfo->tTid)[i]), 0, (void *)Thread_handleMsg, (void *)kMsgEntry ) )
			return -1;
	}
	*ppThreadInfo = pThreadInfo;
	
	return 0;
}

int AThreadPool_destroy(ThreadInfo_pt pThreadPool)
{
	return destroyThreadPoolResources(pThreadPool);
}


/**********************************************************************/
/** @note 建立链表索引线程池(链表(元素：结构体指针)->数组(线程资源))*/
/**********************************************************************/
int PoolLinker_create(linklist_pt * list)
{
	return linkList_create(list);
}

int PoolLinker_destroy(linklist_pt list)
{
	/** @note linklist中不对元素释放空间，需要自己释放*/
	
	linklistentry_pt entry = list->header->next;
	while (entry != list->header) {
		linklistentry_pt next = entry->next;
		entry->next = entry->prev = NULL;
		AThreadPool_destroy((ThreadInfo_pt)(entry->elem));
		entry->elem = NULL;
		free(entry);
		entry = next;
	}
	list->header->next = list->header->prev = list->header;
	list->size = 0;
	return 0;
}

int PoolLinker_add(linklist_pt list, ThreadInfo_pt elem)
{
	return linkList_addElement(list, elem);
}

int PoolLinker_init(linklist_pt * list, LINK_INFO * Connects, linklist_pt pIdleLinker, int kEpollFd)
{
	ThreadInfo_pt kThreadPool;
	if( AThreadPool_create(&kThreadPool, THREADPOLL_BASESIZE, Connects, pIdleLinker, kEpollFd) )
		return -1;
	
	if( PoolLinker_add(*list, kThreadPool) )
		return -1;
	
	return 0;
}

int PoolLinker_getElemI(linklist_pt list, int index, ThreadInfo_pt * pElem)
{
	int CurSum = -1;
	linklistentry_pt entry;
	
	if (index < 0)
		return -1;

	entry = list->header;
	while(index>CurSum)
	{
		entry = entry->next;
		CurSum += ((ThreadInfo_pt)(entry->elem))->capacity;
	}
	
	*pElem = (ThreadInfo_pt)(entry->elem);
	
	return ( ((ThreadInfo_pt)(entry->elem))->capacity - (CurSum - index + 1) );
}

int PoolLinker_getCapacity(linklist_pt list)
{
	//  int Sum = 0;
	//  linklistentry_pt entry = list->header->next;
	//  while (entry != list->header)
	//  {
	//  	CurSum += ((ThreadInfo_pt)(entry->elem))->capacity;
	//  	entry = entry->next;
	//  }
	//  
	//  return CurSum;
	
	int CurSum = list->size;
	
	if(CurSum > 0)
		return ( (CurSum-1)*THREADPOLL_ELEMSIZE + THREADPOLL_BASESIZE );
	else
		return 0;
}

int PoolLinker_expandCapacity(linklist_pt list, LINK_INFO * Connects, linklist_pt pIdleLinker, int kEpollFd)
{
	ThreadInfo_pt pThreadInfo;
	if(AThreadPool_create(&pThreadInfo, THREADPOLL_ELEMSIZE, Connects, pIdleLinker, kEpollFd))
		return -1;
	else
		if(PoolLinker_add(list, pThreadInfo))
			return -2;
	
	return 0;
}
