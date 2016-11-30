////////////////////////////////////////////////////////////////
/// COPYRIGHT Polelink
/// Copyright (c) 2013
/// All rights reserved.
/// 
/// @file  d_connectpool.h
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
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>

#include "d_connectpool.h"
#include "d_trace.h"

pthread_mutex_t stop_mutex;
LINK_INFO * pConnects = NULL;

/**********************************************************************/
/** @note 空闲连接信息链表操作*/
/**********************************************************************/

int IdleConnectLinker_create(linklist_pt * list)
{
	return linkList_create(list);
}


int IdeleConnectLinker_destroy(linklist_pt list)
{
	/** @note linklist中不对元素释放空间，需要自己释放*/
	linklistentry_pt entry = list->header->next;
	while (entry != list->header) {
		linklistentry_pt next = entry->next;
		entry->next = entry->prev = NULL;
		free((int *)(entry->elem));
		entry->elem = NULL;
		free(entry);
		entry = next;
	}
	list->header->next = list->header->prev = list->header;
	list->size = 0;
	return 0;
}


int IdleConnectLinker_add(linklist_pt list, int * elem)
{
	return linkList_addElement(list, elem);
}

int IdleConnectLinker_remove(linklist_pt list, int index)
{
	int * pElem = (int *)linkList_removeIndex(list, index);
	if(NULL != pElem)
		return *pElem;
	else 
		return -1;
}

int IdleConnectLinker_isEmpty(linklist_pt list)
{
	return linkList_isEmpty(list);
}


/**********************************************************************/
/** @note 连接池信息创建与销毁*/
/**********************************************************************/

int createConnectArray(LINK_INFO **links, int len)
{
	*links = ( LINK_INFO * )malloc( len * sizeof(LINK_INFO) );
	if(NULL != *links)
		return 0;
	else
		return -1;
}

int destroyConnectArray(LINK_INFO *links)
{
	if(links != NULL)
	{
		free(links);
		return 0;
	}else{
		return -1;
	}
}

/**********************************************************************/
/** @note 连接池对应事件创建与销毁*/
/**********************************************************************/

int createEvtArray(struct epoll_event ** events, int len)
{
	*events = ( struct epoll_event * )malloc( len * sizeof(struct epoll_event) );
	if(NULL != *events)
		return 0;
	else
		return -1;
}

int destroyEvtArray(struct epoll_event * events)
{
	if(events != NULL)
	{
		free(events);
		return 0;
	}else{
		return -1;
	}
}


/**********************************************************************/
/** @note 关闭socket连接*/
/**********************************************************************/

void clearConnect(LINK_INFO * connect)
{
	connect->ActiveTime = 0;
	connect->fd       = 0;
	connect->Registed = 0;
	connect->ShutSig  = 0;
	connect->CopyCnt  = 0;
	connect->vin[0]   = '\0';
}

void initConnect(LINK_INFO * connect, int Fd)
{
	time( &(connect->ActiveTime) );
	connect->fd = Fd;
	connect->Registed = 0;
	connect->ShutSig  = 0;
	connect->CopyCnt  = 0;
	connect->vin[0]   = '\0';
}

/** 关闭epoll事件的连接 并 释放相应的资源
 * 
 * @param index 待处理事件序号
 * @param add2linker 是否需要打入资源列表，
 * @return    无
 * @attention 不是所有的资源都能分配到资源，（只是link了，后面没有数据通讯就是一种）
 * @attention 关闭事件连接 >> 增加空闲进程到链表 >> 统计连接数目
 */
void closeConnect(linklist_pt IdleLinker, int epollFd, LINK_INFO connect, int index)
{
	if(connect.ActiveTime != 0)
	{
		struct epoll_event evt;
		evt.data.u32 = index;
		epoll_ctl(epollFd, EPOLL_CTL_DEL, connect.fd, &evt);
		shutdown(connect.fd, SHUT_RDWR);
		close(connect.fd);
		
		int * pIndex = (int *)malloc( sizeof(int) );
		*pIndex = index;
		IdleConnectLinker_add(IdleLinker, pIndex);
		
		/** @note 对连接计数 */
		updateCurConnects(IS_SUB);
		
		/** @note 清空当前队列中时间以示空闲 */
		connect.vin[0]   = '\0';
		connect.Registed = 0;
		connect.ShutSig  = 0;
		connect.CopyCnt  = 0;
		
		BeDebug(isDebug, "\n------------closed[%s]\n", connect.vin);
	}
}

/** 断开连接
 * 
 * @param index 通道索引
 * @param useCnt 线程状态
 * @param logo 显示字符
 * @return  无
 */
void stopConnect(linklist_pt IdleLinker, int epollFd, LINK_INFO connect, int index, int useCnt, char logo)
{
	if(1 == connect.ShutSig)
	{
		if(useCnt == connect.CopyCnt)
		{
			BeDebug(isDebug, "T(%s:%d) stop (%c)\n", connect.vin, index, logo);
			closeConnect(IdleLinker, epollFd, connect, index);
		}
	}
}
