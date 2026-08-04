#ifndef MAIN_H
#define MAIN_H



#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//套接字的
  #include <errno.h>    
#include <arpa/inet.h>
  #include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
//线程
#include <sys/types.h>          /* See NOTES */
    #include <pthread.h>
 #include <semaphore.h>

//http的端口
#define HTTP_PORT 8080
#define HOST_IP "192.168.32.68"




sem_t sem_iat;
sem_t sem_dk;
sem_t sem_tts;


char * speech_string;
    char recvbuf[4096];


  void * deep_seek(void *a);
  void * open_iat(void *a);
  void * open_tts(void *a);
   int socket_s();
void  tts_lwt(char * recvbuf);


#endif