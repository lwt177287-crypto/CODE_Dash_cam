#include "video_car.h"
void SMS()
{
 int socket_fd=socket(AF_INET,SOCK_STREAM ,0);
     if(-1==socket_fd)
     {
        fprintf(stderr,"create socket error %s",strerror(errno));
        // close(lcd_fd);  //记得关闭文件
        exit(1);
    }

struct sockaddr_in dest_addr;
  memset(&dest_addr,0,sizeof(dest_addr));
  dest_addr.sin_family=AF_INET;
  dest_addr.sin_port=htons(HTTP_TCP_PORT);   //将转成网络字节序的80端口赋给变量
  dest_addr.sin_addr.s_addr=inet_addr(SMS_IP);  //ip(7)推荐
  int ret = connect(socket_fd,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
   
  if(-1==ret)
  {
    fprintf(stderr,"connect bemfa.com error %s",strerror(errno));
    close(socket_fd);  //记得关闭文件
    exit(1);
  }
  int a=(rand()+1000)%9999;
  printf("------------连接互亿无线成功----------------\n");
  char recvbuf[4096]={0};
    char sendbuf[4096]={0};
    char info[1024];
    sprintf(info,"account=C86827116&password=a0f66a8e656ca53235809f8ba2d8db9d&mobile=17728752392&content=您的验证码是：%d。请不要把验证码泄露给其他人。",a);
    

  sprintf(sendbuf,
        "POST /sms/Submit.json HTTP/1.1\r\n"
        "Host:api.ihuyi.com\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length:%ld\r\n"
        "Connection:close\r\n"        
        "\r\n"
        "%s",strlen(info),info
    );
        send(socket_fd,sendbuf,strlen(sendbuf),0);
    char * p=NULL;
    printf("____________________\n");
        recv(socket_fd,recvbuf,1024,0);
        printf("recvbuf=%s\n",recvbuf);
        if((p=strstr(recvbuf,"\"code\":2,"))!=NULL)
      {
        printf("提交成功\n");
      }
      else
      {
        printf("提交失败\n");
      }
      if(p!=NULL)
      {
        p+=strlen("\"code\":");
       printf("%c\n",p[1]);
      }
      p=strstr(recvbuf,"\"msg\":\"");
      if(p!=NULL)
      {
        p+=strlen("\"msg\":\"");
        
        char * end=strchr(p,'\"');
        if(end!=NULL)
        {
          int len=end-p;
          char msg[50]={0};
          strncpy(msg,p,len);
          printf("msg=%s\n",msg);
          if(strcmp(msg,"提交成功")==0)
          {
            printf("发送成功\n");
          }
          else
          {
            printf("发送失败\n");
          }

        }
      }
}
