#include "main_dk.h"

#define S_TCP_PORT 9999

#if 1
int  socket_s()
{
    int socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==socket_fd)
    {
        fprintf(stderr,"socket error:%s",strerror(errno));
        exit(1);
    }    
    struct sockaddr_in dest_addr;
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(S_TCP_PORT);
    dest_addr.sin_addr.s_addr=INADDR_ANY;

    int ret=bind(socket_fd,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
    if(-1==ret)
    {
        fprintf(stderr,"bind host error:%s",strerror(errno));
        exit(1);
    }   

    listen(socket_fd,5);
    printf("已开启服务器\n");
   return socket_fd;
}

#endif