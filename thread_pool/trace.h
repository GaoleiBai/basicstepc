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



#ifndef d_TRACE_H
#define d_TRACE_H

#define IS_ADD 1
#define IS_SUB 0
#define IPADDR_LEN 16
#define TIMELIST_LEN 3

extern int running;
extern int isDebug;

extern time_t DXGStartTime;

/** @note 连接统计*/
extern int CurLinks;
extern int MaxLinks;

/** @note 事件统计*/
extern int CurEvts;
extern int MaxEvts;

/** @note 线程使用统计*/
extern int CurThreads;
extern int MaxThreads;

/** @note 系统错误和警告统计*/
extern int SysErrNO;
extern int SysWrnNO;
extern int MsgErrNO;

extern char LocalIpAddr[IPADDR_LEN];

extern long TopWorst3[TIMELIST_LEN];
extern long TopRecent3[TIMELIST_LEN];


void updateStartTime(void);

int  getMaxConnects(void);

int  updateCurConnects(int isAdd);
void updateCurEvts(int evts);
int  updateCurThreads(int isAdd);

void updateSysErrNO(void);
void updateSysWrnNO(void);
void updateMsgErrNO(void);

int updateLocalIpAddr(char * string);


void BeDebug(int Debug_flag, const char * format, ...);

#endif
