#include "video_car.h"

#define HOST_IP "5.9.243.187"
// #define HOST_IP "wttr.in"
#define C_TCP_PORT 80

void Status_Bar()
{
   socket_fd=socket(AF_INET,SOCK_STREAM,0);
  if(-1==socket_fd)
  {
    fprintf(stderr,"create socket error %s",strerror(errno));
    return ;
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
    return ;
  }
  printf("连接和风天气成功\n");
  char send_buf[4096]={0};
  char recv_buf[4096]={0};

  sprintf(send_buf,
    "GET /Beijing?format=j1 HTTP/1.1\r\n"
    "Host: wttr.in\r\n"
    "Connection: close\r\n"
    "\r\n");

send(socket_fd,send_buf,strlen(send_buf),0);

int len;

while((len=recv(socket_fd,recv_buf,sizeof(recv_buf)-1,0))>0)
{
    recv_buf[len]=0;
    printf("%s",recv_buf);
}

}