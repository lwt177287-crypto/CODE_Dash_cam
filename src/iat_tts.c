#include "video_car.h"
//语音转文字，文字转语音
//实现：向另一服务器进程发送，由进程发送给服务器

#define HOST_IP "192.168.32.70"
#define C_TCP_PORT 9999

int socket_c()
{
   socket_fd=socket(AF_INET,SOCK_STREAM,0);
  if(-1==socket_fd)
  {
    fprintf(stderr,"create socket error %s",strerror(errno));
    return -1 ;
  }
  struct sockaddr_in dest_addr;
  memset(&dest_addr,0,sizeof(dest_addr));
  dest_addr.sin_family=AF_INET;
  dest_addr.sin_port=htons(C_TCP_PORT);   //将转成网络字节序的80端口赋给变量
  dest_addr.sin_addr.s_addr=inet_addr(HOST_IP);  //ip(7)推荐
  
  int ret = connect(socket_fd,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
   
  if(-1==ret)
  {
    fprintf(stderr,"connect host error %s\n",strerror(errno));
    return -1;
  }
  printf("------------连接deekseep服务器成功----------------\n");
//   char recvbuf[256]={0};
//     char sendbuf[256]={0};
char a='0';
  if(-1!=send(socket_fd,&a,sizeof(a),0))
    printf("发送成功 \n");
  else
  printf("发送失败！\n");
  
  return 0;
}

void open_mic()
{
  if(mic_flgs==0)
  {
    printf("-----mic_flgs=%d\n",mic_flgs);
      printf("打开麦克风111\n");
      if( 0==socket_c())
          mic_flgs=1;
      else 
      printf("打开失败,通信关闭\n");
    }
    else if(mic_flgs==1) 
    {
          char a[10]={0};
          recv(socket_fd,a,10,MSG_DONTWAIT);
          if(strlen(a)==1&&a[0]=='0')
          {
              printf("2\n");
              memset(a,0,strlen(a));
              if( 0!=socket_c())
                printf("打开失败，通信关闭~~\n");
          }  
          else
          {
            printf("正在聆听麦克风/处理/输出音频\n");

          }

      //如果等于0直接开麦克风啊，可以合并
    }
}