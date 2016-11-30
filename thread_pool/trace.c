////////////////////////////////////////////////////////////////
/// COPYRIGHT Polelink
/// Copyright (c) 2013
/// All rights reserved.
/// 
/// @file  d_trace.h
/// @brief 记录系统状态
///
/// @version $Revision: 0 $
/// @author Wuchunyu
/// @date $Date: 2013-03-25 10:21:33 +0800 (Mon, 25 Mar 2013) $
///
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>
#include <time.h>

#include "d_trace.h"

int running = 1;
int isDebug = 1;

time_t DXGStartTime;

int CurLinks = 0;
int MaxLinks = 0;

int CurEvts  = 0;
int MaxEvts  = 0;

int CurThreads = 0;
int MaxThreads = 0;

int SysErrNO = 0;
int SysWrnNO = 0;
int MsgErrNO = 0;

char LocalIpAddr[IPADDR_LEN];

int TimeInsertPoint = 0;
long TopWorst3[TIMELIST_LEN] = {0};
long TopRecent3[TIMELIST_LEN] = {0};

static pthread_mutex_t TimeList_mutex;
static pthread_mutex_t ThreadCnt_mutex;
static pthread_mutex_t DebugInfo_mutex;
static pthread_mutex_t SysWrnCnt_mutex;
static pthread_mutex_t SysErrCnt_mutex;
static pthread_mutex_t MsgErrCnt_mutex;


/***********************************************************************
 ** 系统状态更新
 **********************************************************************/

void updateStartTime(void)
{
	time(&DXGStartTime);
}

int getMaxConnects(void)
{
	return MaxLinks;
}

int updateCurConnects(int isAdd)
{
	if(isAdd)
		CurLinks++;
	else
		CurLinks--;
	
	if(CurThreads < 0)
	{
		CurLinks = 0;
		return -1;
	}else{
		if(CurLinks>MaxLinks)
			MaxLinks = CurLinks;
		return 0;
	}
}

void updateCurEvts(int evts)
{
	CurEvts = evts;
	if(CurEvts>MaxEvts)
		MaxEvts = CurEvts;
}

int updateCurThreads(int isAdd)
{
	pthread_mutex_lock(&ThreadCnt_mutex);
	if(isAdd)
		CurThreads++;
	else
		CurThreads--;
	
	if((CurThreads > MaxThreads) || (CurThreads < 0) )
	{
		pthread_mutex_unlock(&ThreadCnt_mutex);
		return -1;
	}else{
		if(CurThreads>MaxThreads)
			MaxThreads = CurThreads;
		pthread_mutex_unlock(&ThreadCnt_mutex);
		return 0;
	}
}

void updateSysErrNO(void)
{
	pthread_mutex_lock(&SysErrCnt_mutex);
	SysErrNO++;
	pthread_mutex_unlock(&SysErrCnt_mutex);
}

void updateSysWrnNO(void)
{
	pthread_mutex_lock(&SysWrnCnt_mutex);
	SysWrnNO++;
	pthread_mutex_unlock(&SysWrnCnt_mutex);
}

void updateMsgErrNO(void)
{
	pthread_mutex_lock(&MsgErrCnt_mutex);
	MsgErrNO++;
	pthread_mutex_unlock(&MsgErrCnt_mutex);
}

int updateLocalIpAddr(char * string)
{
	int IpAddrLen = strlen(string);
	if(strlen(string) > IPADDR_LEN-1)
	{
		return -1;
	}else{
		memset(LocalIpAddr, 0, IPADDR_LEN);
		memcpy(LocalIpAddr, string, IpAddrLen);
		LocalIpAddr[IpAddrLen] = '\0';
		return 0;
	}
}


void replaceMini(long elem, long * array, int len)
{
	int i = 0;
	for(i=0; i<len; i++)
	{
		if(elem >= array[i])
		{
			int j = len-1;
			for( ; j>i; j--)
				array[j] = array[j-1];
			array[j] = elem;
			break;
		}
	}
}

void updateTimeList(long kTime)
{
	pthread_mutex_lock(&TimeList_mutex);
	
	if( TimeInsertPoint >= TIMELIST_LEN )
		TimeInsertPoint = 0;
	
	TopRecent3[TimeInsertPoint] = kTime;
	
	replaceMini(kTime, TopWorst3, TIMELIST_LEN);
	
	pthread_mutex_unlock(&TimeList_mutex);
}


/***********************************************************************
 ** 系统显示
 **********************************************************************/

void BeDebug(int Debug_flag, const char * format, ...)
{

	va_list arg_ptr;
	
	va_start(arg_ptr, format);
	
	if(Debug_flag)
	{
		pthread_mutex_lock(&DebugInfo_mutex);
		vfprintf(stderr, format, arg_ptr);
		pthread_mutex_unlock(&DebugInfo_mutex);
	}else{
		char kString[200];
		memset(kString, 0, 200);
		vsprintf(kString, format, arg_ptr);
		syslog(LOG_WARNING, kString);
	}
	va_end(arg_ptr);

}
