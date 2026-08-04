#include "main_dk.h"

//http的端口
#define HTTP_PORT 8080
#define HOST_IP "192.168.32.68"


#if 0
int main()
{
    //创建套接字
  int socket_fd=socket(AF_INET,SOCK_STREAM,0);
  if(-1==socket_fd)
  {
    fprintf(stderr,"create socket error %s",strerror(errno));
    exit(1);
  }

  //填写信息
  struct sockaddr_in dest_addr;
  memset(&dest_addr,0,sizeof(dest_addr));
  dest_addr.sin_family=AF_INET;
  dest_addr.sin_port=htons(HTTP_PORT);   //将转成网络字节序的80端口赋给变量
  dest_addr.sin_addr.s_addr=inet_addr(HOST_IP);  //ip(7)推荐
  
  //连接端口
  int ret = connect(socket_fd,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
   
  if(-1==ret)
  {
    fprintf(stderr,"connect bemfa.com error %s",strerror(errno));
    close(socket_fd);  //记得关闭文件
    exit(1);
  }
  printf("------------连接deepseek成功----------------\n");
  char recvbuf[4096]={0};
    char sendbuf[4096]={0};
    
// curl https://api.deepseek.com/chat/completions \
//   -H "Content-Type: application/json" \
//   -H "Authorization: Bearer ${DEEPSEEK_API_KEY}" \
//   -d '{
//         "model": "deepseek-v4-pro",
//         "messages": [
//           {"role": "system", "content": "You are a helpful assistant."},
//           {"role": "user", "content": "Hello!"}
//         ],
//         "thinking": {"type": "enabled"},
//         "reasoning_effort": "high",
//         "stream": false
//       }'
    char keybuf[1024]={0};
    printf("请输入问题\n");
  scanf("%s",keybuf);
     sprintf(sendbuf,
        "POST /chat HTTP/1.1\r\n"
        "Content-Length:%ld\r\n"
        "Connection:close\r\n"        
        "\r\n"
        "%s",
        strlen(keybuf),
        keybuf
                        );
    
    printf("%s\n",sendbuf);
    send(socket_fd,sendbuf,strlen(sendbuf),0);
    
    // for(;;)
    // {
        int ret_recv=recv(socket_fd,recvbuf,4096,0);
        // printf("ni\n");
        printf("%s\n",recvbuf);
        if(ret_recv<0)
        // break;
    // }
    return 0;
}
#endif