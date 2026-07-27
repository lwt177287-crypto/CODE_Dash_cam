#ifndef VIDEO_CAR_EXE_H  // 1. 检查是否未定义 
#define VIDEO_CAR_EXE_H // 2. 如果未定义，则定义它（通常用大写文件名）


#include<stdio.h> 
 #include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jpeglib.h"
#include <stddef.h>
#include <string.h>
#include <linux/input.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <sys/time.h>
#include <sys/ioctl.h>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <sys/wait.h>
  #include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
 #include <pthread.h>
 #include <semaphore.h>
  #include <semaphore.h>


#define OPEN_FILM en0_x>=530&&en0_x<710&&en0_y>70&&en0_y<250      //打开摄像机
#define EIXT_FILM en0_x>0&&en0_x<55&&en0_y>345&&en0_y<380     //关闭摄像机
#define CLOSE     en0_x>160&&en0_x<290&&en0_y>65&&en0_y<240      //关机
#define RECORD    en0_x>725&&en0_x<800&&en0_y>135&&en0_y<180      //打开录像
#define GUIDELINE en0_x>725&&en0_x<800&&en0_y>210&&en0_y<260    //打开辅助线

//摄像头采集图像的宽和高
#define  IMAGE_WIDTH  640
#define  IMAGE_HEIGHT  480


        
#define CAT_STAT "/root/shiyan/CODE/cat_stat.jpg"


// 开发板宽高
#define WIDTH  800
#define HEIGHT 480

#define LINE_H 350       //不能改
#define LINE_W  15        //线的宽度
#define SIZE    50      //一段线的长度
#define SIZE_H    20    //线与线间隔长度
#define ROW_W    60    //横线宽度
#define ROW_H    10    //横线长度
//更改颜色的占比，会导致长横线bug,长横线应该在固定的比例而不是草率的在每个颜色开头。。以后再弄
#define STAT    3
#define MID    3
#define END    3
//每个阶段的颜色
#define LINE_COLOR_STAT     0xFFFF0000
#define LINE_COLOR_MID      0xFFFFFF00
#define LINE_COLOR_END      0xFF00FF00



int video_flgs;//打开倒车影像状态
int round_flgs;//录制状态
int line_pth; //
int round_pth;
int Interface; //主界面标志位

 sem_t sem; //信号量
 sem_t sem1; //信号量
 sem_t sem2; //信号量
 sem_t sem3; //信号量
pthread_mutex_t  mutex;  //互斥锁,buffermp的
pthread_mutex_t  mutex_round_flgs;  //互斥锁
pthread_mutex_t  mutex_en0_ABS_flgs;  //互斥锁
pthread_mutex_t  mutex_reading_flgs;  //互斥锁  表示摄像头的这块缓冲正在写
pthread_mutex_t  mutex_wr_buf_mp;  //互斥锁  表示缓冲和开发板正在读写

unsigned int buf_i;   //摄像头正在映射的那块内存号
 unsigned int *mp;  //开发板屏幕
unsigned int * buffer_mp;//处理图片的缓冲区
unsigned int read_index;  //读到摄像头哪个缓冲的下标
unsigned int * write_buf; //缓冲区写完
unsigned int * read_buf;  //开发板读完
unsigned int * (p_buffer[4]);  //开发板读完


//摄像头的信息
struct buffer {
        void *                  start;
        size_t                  length;
};
extern struct buffer *         buffers;

//坐标
extern int en0_x; 
extern int en0_y;

unsigned int * lcd_fp;          //用来复位
extern char * car_video[200];   //行车记录仪回放
//进程
 pthread_t  thread_ent0;        
 pthread_t  thread_video;
pthread_t  thread_lcd_video;
pthread_t  thread_round1;
pthread_t  thread_video_key;

//接口
int read_JPEG_file (char * filename, unsigned int *mp); //图片转数据
void Abs_Cat( int event_fd, struct input_event * ev0);  //获取坐标


void Open_Video();      //打开摄像头
// 在 video_car.c 的最顶部添加：
int open_device(void);
static void init_device(void);
static void start_capturing(void);
static void mainloop(void);
int yuyv_to_rgb_pix(int Y,int Cb,int Cr);
void yuv2_rgb(unsigned int* read_buf,  char *write_buf);




void Line_Car(unsigned int *mp);    //辅助线
GLOBAL(void)
write_JPEG_file (char * filename, int quality,  char   *buf); //数据变成图片
void File_time(char * filetime);  //生成时间戳字符串赋给指针
//线程函数
void *child_ent0(void * a);
void *child_lcd_buf(void * a);
void *child_video(void * a);
void * child_lcd_video(void * a);
void * lcd_round1(void * a);
void * lcd_video_key(void * a);


void round1(); //显示录制圆点
void en0_clear();//坐标清零


void Mmap_Bmp(int lcd_fd,FILE * bmp_fd,unsigned int *mp); //另一个项目
FILE * Open_Bmp(char * pathname);   //另一个项目的
#endif