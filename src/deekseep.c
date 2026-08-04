#include "main_dk.h"
 

void * deep_seek(void *a)
{
    printf("------------连接deepseek成功----------------\n");
    char sendbuf[4096];
    char keybuf[1024];
     speech_string=calloc(1,1024);
    
     {     
        //创建套接字
         int  socket_fd=socket(AF_INET,SOCK_STREAM,0);
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
        printf("进入deepseek\n");

        //连接端口
        int ret = connect(socket_fd,(struct sockaddr *)&dest_addr,sizeof(dest_addr));

        if(-1==ret)
        {
        fprintf(stderr,"connect deepseek error %s",strerror(errno));
        close(socket_fd);  //记得关闭文件
        exit(1);
        }
        
        memset(sendbuf,0,4096);
        memset(recvbuf,0,4096);
        memset(keybuf,0,4096);
        printf("请说问题\n");
        {
            sem_post(&sem_iat);
            sem_wait(&sem_dk);

        }
        //将识别到的文字拷贝
        memcpy(keybuf,speech_string,strlen(speech_string));
            sprintf(sendbuf,
            "POST /chat HTTP/1.1\r\n"
            "Content-Length:%ld\r\n"
            "Connection:close\r\n"        
            "\r\n"
            "%s",
            strlen(keybuf),
            keybuf
                            );
        //问的啥
        printf("%s\n",sendbuf);
        //发给deepseek看看
        send(socket_fd,sendbuf,strlen(sendbuf),0);
        //deepseek回信
        char p_return[4096];
        int ret_recv=recv(socket_fd,p_return,4096,0);
        if(ret_recv>0)
        {
            char *p=strstr(p_return,"\r\n\r\n");
            memcpy(recvbuf,p,strlen(p));
        }
        else
        {
            memset(recvbuf,0,sizeof(recvbuf));
        }
        printf("%s\n",recvbuf);
        //转语音听听
        sem_post(&sem_tts);
        printf("退出deepseek");
        pthread_exit(NULL);

    }
}