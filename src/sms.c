#include "video_car.h"

int sms_time;

int  SMS(char * phone_number)
{
   srand(time(NULL));
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

  printf("传入的手机号：%s\n",phone_number);
  strcpy(s_SMS_verification_code.phone_Number,s_keyboard.phone_number);
  
  s_SMS_verification_code.sms_out=rand()%9000+1000;
  printf("系统发送的验证码：%d\n", s_SMS_verification_code.sms_out);

 return s_SMS_verification_code.sms_out;

  #if 0
  
  printf("------------连接互亿无线成功----------------\n");
  char recvbuf[4096]={0};
    char sendbuf[4096]={0};
    char info[1024];
    
    sprintf(info,"account=C29437780&s_keyboard.password=85908d5d3fccc78522f251c89625b9ec&mobile=%s&content=您的验证码是：%d。请不要把验证码泄露给其他人。",s_SMS_verification_code.phone_Number,s_SMS_verification_code.sms_out);
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
          printf("%s\n",msg);
          if(strcmp(msg,"提交成功")==0)
          {
            printf("发送成功\n");
            return s_SMS_verification_code.sms_out;
          }
          else
          {
            printf("发送失败\n");
            return 0;
          }

        }
      }
      #endif
}


void SMS_Get()
{
     if(strlen(s_keyboard.phone_number)==11)
      {
          //已经获取过验证码，等待60秒
          if(time(NULL)-sms_time<=60)
          {
              printf("等待重新获取验证码（%d）\n",60-time(NULL)+sms_time);
          }
          else
          {
              sms_time=time(NULL);
              printf("%d\n",sms_time);
              int a=0;
              //可以直接用结构体了,这里可以换成获取验证码是否有误
              if((a=SMS(s_keyboard.phone_number))==0)
              {
                  fprintf(stderr,"get ssm error %s",strerror(errno));
              }
              else
              {
                //获得验证码
                printf("注册得到验证码\n");
                sprintf(s_admin.sms_out,"%d",a);
                  printf("手机号：%s\n",s_SMS_verification_code.phone_Number);
                printf("验证码：%d\n",s_SMS_verification_code.sms_out);
              }
          }
      }
      else
      {
          //没输入手机号就获取验证码
          printf("手机号格式错误！\n");
      }
}




int SMS_YES()
{
   if(strlen(s_keyboard.phone_number)!=11)
    {
        printf("手机号格式错误\n");
        en0_clear();
        return INSTRATTON_STATE;
    }

     //传过来的验证码少于思四位，一般不用
    // if(strlen(s_admin.sms_out)!=4)
    // {
    //     printf("系统验证码错误~");
    //       en0_clear();
    //       return INSTRATTON_STATE;
    // }

    if(time(NULL) - sms_time > 60)
    {
        printf("验证码已过期\n");
        memset(s_admin.sms_out,0,sizeof(s_admin.sms_out));
               return INSTRATTON_STATE;

    }
    if(strcmp(s_admin.sms_out,s_keyboard.sms_input)!=0)
    {
        printf("验证码输入错误！请重新输入\n");
          en0_clear();
               return INSTRATTON_STATE;
    }
      if(strcmp(s_keyboard.phone_number,s_SMS_verification_code.phone_Number)!=0)
    {
        printf("该验证码不是这个手机号请求的\n");
        return INSTRATTON_STATE;


    }
    return  -1;
}