#include "main_dk.h"

//http的端口
#define HTTP_PORT 8080
#define HOST_IP "192.168.32.68"



int main()
{

  printf("进入main\n");
  
  //初始化信号量
 if(-1==sem_init(&sem_iat,0,0))
 {
     fprintf(stderr," sem_iat init error %s",strerror(errno));
        exit(1);
 } 

  if(-1==sem_init(& sem_dk,0,0))
 {
     fprintf(stderr,"sem_dk init error %s",strerror(errno));
        exit(1);
 } 
 if(-1==sem_init(& sem_tts,0,0))
 {
     fprintf(stderr,"sem_tts init error %s",strerror(errno));
        exit(1);
 } 
 int socket_fd=socket_s();
 
 for(;;)
 {
      int fd_buf;
      fd_buf=accept(socket_fd,NULL,0);
      if(-1==fd_buf)
      {
          fprintf(stderr,"accept host error:%s",strerror(errno));
          exit(1);
      }
      printf("已有设备连接到服务器\n");
      char secvbuf[256]={0};
      recv(fd_buf,secvbuf,256,0);
      // close(socket_fd);
      // close(fd_buf);
      printf("接收到数据\n");
      
        //等待信号
        pthread_t dk_pthread;
        if(-1==pthread_create(&dk_pthread,NULL,deep_seek,NULL))
        {
          fprintf(stderr,"pthread deepseek error %s",strerror(errno));
          close(socket_fd);  //记得关闭文件
          exit(1);
        }
          // pthread_detach(dk_pthread);
      pthread_t pthread_iat;  //语音转文字
      if(-1==pthread_create(&pthread_iat,NULL,open_iat,NULL))
    {
      fprintf(stderr,"pthread deepseek error %s",strerror(errno));
      close(socket_fd);  //记得关闭文件
      exit(1);
    }
          // pthread_detach(pthread_iat);
          
          sem_wait(&sem_tts);
          tts_lwt(recvbuf);
          char return_tts='0';
          send(socket_fd,&return_tts,sizeof(return_tts),0);
          pthread_join(pthread_iat,NULL);
          pthread_join(dk_pthread,NULL);
}

  #if 0
 pthread_t pthread_tts; //文字合成语音
  if(-1==pthread_create(&pthread_tts,NULL,open_tts,NULL))
  {
     fprintf(stderr,"pthread deepseek error %s",strerror(errno));
    close(socket_fd);  //记得关闭文件
    exit(1);
  }
  pthread_join(pthread_tts,NULL);
#endif

    return 0;
}