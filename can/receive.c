int main(void)
{
    
    
      /*********************/
      pid_t pro1;
      pthread_t child_pthread1;
      /*********************/
    
      /*********************/
      struct ifreq ifr;
      struct sockaddr_can addr;
    
      s=socket(PF_CAN,SOCK_RAW,CAN_RAW);
      if(s<0)
      {   
        perror("socket can\n");
      }   
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
    
      struct can_filter rfilter;
      rfilter.can_id=0x11;
      rfilter.can_mask=CAN_SFF_MASK;
      setsockopt(s,SOL_CAN_RAW,CAN_RAW_FILTER,&rfilter,sizeof(rfilter));












====================================================================================================================================================================
void can_read()
{     
      int ret;
      while(1)
      {
                
            ret=read(s,&frame,sizeof(frame));
            if(ret>0)
            {

                  
                  //  sem_p(sem_id,0,1);  
                  gettimeofday(&tBegin, NULL);
                  deltaTime = 1000000L * tBegin.tv_sec  +  tBegin.tv_usec;
                  //printf("the begin time : %ldms\n", deltaTime);
                  start_time[start_count]=deltaTime;
                        start_count ++;
                        
                  //measure the number of framme and save data and date to deal       
             //     frame_buffer[frame_num]=frame;
               //   frame_num++;
            //    if(frame_num >=100)
            //    frame_num=0;
                        
                  count ++;
                  //printf("ID =0x%X   DLC =%d   data[0]=0x%X   ------- %d\n",frame.can_id,frame.can_dlc,frame.data[0],count);
                        int j=10000;
                        while(j--);
                        
                        //end time/
                  gettimeofday(&tEnd, NULL);
                  deltaTime = 1000000L * tEnd.tv_sec  + tEnd.tv_usec;
                  //printf("the end time : %ldms\n", deltaTime);
                        end_time[end_count]=deltaTime;
                        end_count ++;
                  //  sem_v(sem_id,0,1);    
                  
                  }
      
      }
}
