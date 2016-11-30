////////////////////////////////////////////////////////////////
/// COPYRIGHT Polelink
/// Copyright (c) 2013
/// All rights reserved.
/// 
/// @file  d_sysinit.h
/// @brief 系统初始化
///
/// @version $Revision: 0 $
/// @author Wuchunyu
/// @date $Date: 2013-03-25 10:21:33 +0800 (Mon, 25 Mar 2013) $
///
////////////////////////////////////////////////////////////////

#ifndef D_SYSCNTL_H
#define D_SYSCNTL_H

#define MaxFileNO (20000)
#define MaxStack  (512*1024)

extern int In_MaxThread;

void initSysConf( void );

int  createTCPSrv(int Srv_port, int Max_Back);

void close_FD(int fd);

#endif
