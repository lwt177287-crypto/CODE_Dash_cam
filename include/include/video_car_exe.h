#ifndef VIDEO_CAR_EXE_H  // 1. 检查是否未定义 
#define VIDEO_CAR_EXE_H // 2. 如果未定义，则定义它（通常用大写文件名）


#include<stdio.h>
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



#define WIDTH 800       //开发板宽高
#define HEIGHT 480          
#define CAT_STAT "cat_stat.jpg"
#define CLEAR(x) memset (&(x), 0, sizeof (x))




 unsigned int *mp;  //开发板屏幕

struct buffer {
        void *                  start;
        size_t                  length;
};

extern int en0_x; 
extern int en0_y;


//图片宽高
#define WIDTH  800
#define HEIGHT 480
#define PATHNAME "/root/rearview.bmp"
//辅助线数据
//线长宽不应超出开发板，横线长度不应该超过一段线的长度，线的间隔长度不应该比总线长...
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




int read_JPEG_file (char * filename, unsigned int *mp);
void Abs_Cat();
void Open_Video();
// 在 video_car.c 的最顶部添加：
int open_device(void);
static void init_device(void);
static void start_capturing(void);
static void
mainloop                        (void);
// static void stop_capturing(void);
int yuyv_to_rgb_pix(int Y,int Cb,int Cr);
void yuv2_rgb(char* buf,  unsigned int *mp);
unsigned int * lcd_fp;

void Mmap_Bmp(int lcd_fd,FILE * bmp_fd,unsigned int *mp);
FILE * Open_Bmp(char * pathname);
void Line_Car(unsigned int *mp);

//摄像头采集图像的宽和高
#define  IMAGE_WIDTH  640
#define  IMAGE_HEIGHT  480

#endif