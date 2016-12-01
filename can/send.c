#include<stdio.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<linux/can.h>
#include<string.h>
#include<linux/if.h>
#include <sys/ioctl.h>
#include<linux/can/raw.h>
#include<unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>


struct timeval tBegin, tEnd;
int s;
struct can_frame frame;
pthread_t thread1,thread2;
long deltaTime;
int sum=0;

void can_send()
{

        
        int nbyte;
        while(1)
        {       
                //gettimeofday(&tBegin, NULL);
                nbyte=write(s,&frame,sizeof(frame));
                if(nbyte<sizeof(frame))
                {       
                        perror("write");
                }
                 //gettimeofday(&tEnd, NULL);
                 //     long deltaTime = 1000000L * (tEnd.tv_sec - tBegin.tv_sec ) + (tEnd.tv_usec - tBegin.tv_usec);
                 //     printf("Time spent: %ldus\n", deltaTime/1000);
                sum++;
                unsigned int a=100,i;
           //     for(i=0;i<20;i++)
                   while(a--);
                 //  a=10000000;
                 //  while(a--);  
                     a=10000;
                                         a=100000;
                     while(a--);

        }


}


int main(void)
{

        struct ifreq ifr;
        struct sockaddr_can addr;

        s=socket(PF_CAN,SOCK_RAW,CAN_RAW);
        if(s<0)
        {
                perror("socket can\n");

        }
        //  int r0=1;
        //   setsockopt(s,SOL_CAN_RAW,CAN_RAW_RECV_OWN_MSGS,&r0,sizeof(r0)); 

        strcpy(ifr.ifr_name,"can0");

        ioctl(s,SIOCGIFINDEX,&ifr);

        addr.can_family=AF_CAN;
        addr.can_ifindex=ifr.ifr_ifindex;
        int ret;
        ret=bind(s,(struct sockaddr *)&addr,sizeof(addr));
        if(ret<0)
        {

                perror("bind");
        }
        setsockopt(s,SOL_CAN_RAW,CAN_RAW_FILTER,NULL,0);

        frame.can_id=0x11;
        frame.can_dlc=1;
        frame.data[0]=0x11;

        gettimeofday(&tBegin, NULL);
        deltaTime = 1000000L * tBegin.tv_sec  +  tBegin.tv_usec;  //å¾
        printf("the begin time : %ldms\n", deltaTime);
                                                  
                                                    pthread_create(&thread1,NULL, (void *)can_send, NULL);
    //  pthread_create(&thread2,NULL, (void *)can_send, NULL);

        char c;
        while(1)
        {
                if(getchar()=='q')
                        printf("already exit the program of can send test\n");
                break;
        }
        close(s);
        pthread_cancel(thread1);


        gettimeofday(&tEnd, NULL);
        deltaTime = 1000000L * tEnd.tv_sec  + tEnd.tv_usec;
        printf("the end time : %ldms\n", deltaTime);

        deltaTime = 1000000L * (tEnd.tv_sec - tBegin.tv_sec ) + (tEnd.tv_usec - tBegin.tv_usec);
        printf("Time spent: %ldms\n", deltaTime);
        printf("num is %d  the average of send  time is %ld   and hao miao is %ld \n",sum,deltaTime/sum,deltaTime/(sum*1000));

        return 1;
}
