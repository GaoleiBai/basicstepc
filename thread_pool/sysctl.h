////////////////////////////////////////////////////////////////

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
