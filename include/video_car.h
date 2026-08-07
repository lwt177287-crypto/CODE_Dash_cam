#ifndef VIDEO_CAR_EXE_H  // 1. 检查是否未定义 
#define VIDEO_CAR_EXE_H // 2. 如果未定义，则定义它（通常用大写文件名）

#include <arpa/inet.h>



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

  #include <errno.h>    

#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>

//目录
// #include <dirent.h>


//主界面按钮
#define OPEN_FILM en0_x>=450&&en0_x<619&&en0_y>70&&en0_y<237      //打开摄像机
 
#define    MAIN_RECORD_VIDEO_SAVE  en0_x>321&&en0_x<391&&en0_y>121&&en0_y<180   //保存记录仪
#define   MAIN_DIR  en0_x>670&&en0_x<740&&en0_y>121&&en0_y<180  //打开视频、录音文件夹

#define MAIN_RECORD_AWA en0_x>280&&en0_x<340&&en0_y>320&&en0_y<385    //打开录音
#define MAIN_SRCEENSHOT en0_x>375&&en0_x<445&&en0_y>320&&en0_y<385    //音乐
#define MAIN_LOCK_SCREEN en0_x>624&&en0_x<690&&en0_y>320&&en0_y<385    //锁屏
#define MAIN_MIC en0_x>480&&en0_x<590&&en0_y>290&&en0_y<400    //麦克风
#define MAIN_SETTINGS en0_x>720&&en0_x<780&&en0_y>320&&en0_y<385    //打开设置


//摄像显示页面按钮
#define REALTIME_SPEECH     en0_x>23&&en0_x<70&&en0_y>98&&en0_y<133     //打开导航
#define REALTIME_RECOE      en0_x>23&&en0_x<70&&en0_y>170&&en0_y<204     //打开录像
#define REALTIME_IMC        en0_x>23&&en0_x<70&&en0_y>247&&en0_y<283     //打开语音
#define EIXT_FILM             en0_x>23&&en0_x<70&&en0_y>317&&en0_y<357     //关闭摄像机

#define GUIDELINE                en0_x>725&&en0_x<776&&en0_y>98&&en0_y<133    //打开辅助线
#define REALTIME_VOLUME_ADD         en0_x>725&&en0_x<776&&en0_y>170&&en0_y<204   //加大音量
#define REALTIME_VOLUME_RM          en0_x>725&&en0_x<776&&en0_y>247&&en0_y<283   //减少音量
#define REALTIME_RETURN            en0_x>725&&en0_x<776&&en0_y>317&&en0_y<357    //返回
 
 

//要改
//设置界面按钮
//设置
#define SET_LOGIN     en0_x>190&&en0_x<430&&en0_y>62&&en0_y<130     //登录
#define  SET_WIFI    en0_x>190&&en0_x<430&&en0_y>136&&en0_y<203     //wifi
#define  SET_LANGUAGE   en0_x>190&&en0_x<430&&en0_y>206&&en0_y<279   //语言
#define SET_SYSTEM_INFO   en0_x>190&&en0_x<430&&en0_y>285&&en0_y<350    //系统信息
#define SET_SHUTDOWN   en0_x>190&&en0_x<430&&en0_y>358&&en0_y<425     //关机


//安全



//显示



//声音



// 蓝牙



//关于




#define SET_RETURN       en0_x>747&&en0_x<800&&en0_y>10&&en0_y<50     //右上角添加返回





//登录界面按钮
#define LOGIN_USERNAME     en0_x>255&&en0_x<347&&en0_y>163&&en0_y<193     //手机号
#define LOGIN_PASSWORD     en0_x>255&&en0_x<347&&en0_y>228&&en0_y<258     //密码、验证码
#define LOGIN_YES      en0_x>255&&en0_x<545&&en0_y>296&&en0_y<330    //登录按钮
  #define LOGIN_FORGOT_PASSWORD     en0_x>505&&en0_x<547&&en0_y>270&&en0_y<280  //忘记密码
#define LOGIN_SMS     en0_x>252&&en0_x<302&&en0_y>270&&en0_y<280     //短信登录 
#define LOGIN_IFICATION      en0_x>411&&en0_x<453&&en0_y>349&&en0_y<362     // 注册
#define LOGIN_RETURN      en0_x>368&&en0_x<423&&en0_y>383&&en0_y<410     // 退出
#define LOGIN_GET_SMS      en0_x>468&&en0_x<550&&en0_y>233&&en0_y<263     // 获取验证码










 //键盘
#define KEYBOARD_1                  en0_x>9&&en0_x<140&&en0_y>275&&en0_y<334      //输入1
#define KEYBOARD_2                  en0_x>142&&en0_x<285&&en0_y>275&&en0_y<334      //输入2
#define KEYBOARD_3                  en0_x>285&&en0_x<423&&en0_y>275&&en0_y<334      //输入3
#define KEYBOARD_4                  en0_x>9&&en0_x<140&&en0_y>336&&en0_y<394    //输入4
#define KEYBOARD_5                  en0_x>142&&en0_x<285&&en0_y>336&&en0_y<394      //输入5
#define KEYBOARD_6                  en0_x>285&&en0_x<423&&en0_y>336&&en0_y<394       //输入6
#define KEYBOARD_7                  en0_x>9&&en0_x<140&&en0_y>395&&en0_y<480   //输入7
#define KEYBOARD_8                  en0_x>142&&en0_x<285&&en0_y>395&&en0_y<480     //输入8
#define KEYBOARD_9                  en0_x>285&&en0_x<423&&en0_y>395&&en0_y<480    //输入9
#define KEYBOARD_0                  en0_x>428&&en0_x<575&&en0_y>395&&en0_y<480       //输入0
#define KEYBOARD_LEFT               en0_x>576&&en0_x<685&&en0_y>275&&en0_y<334     //左移
#define KEYBOARD_RIGHT              en0_x>687&&en0_x<798&&en0_y>275&&en0_y<334      //右移
#define KEYBOARD_END                en0_x>428&&en0_x<575&&en0_y>275&&en0_y<334      //移到末尾
#define KEYBOARD_REMOVE             en0_x>576&&en0_x<685&&en0_y>336&&en0_y<480     //删除
#define KEYBOARD_YES                en0_x>687&&en0_x<798&&en0_y>336&&en0_y<480  //确定
#define KEYBOARD_DECIMAL_POINT      en0_x>428&&en0_x<575&&en0_y>336&&en0_y<394      //小数点
#define KEYBOARD_CLOSE              en0_x>0&&en0_x<800&&en0_y>240&&en0_y<260      //隐藏
#define KEYBOARD_RETURN              en0_x>0&&en0_x<800&&en0_y>0&&en0_y<HEIGHT-240      //隐藏




// //键盘
// #define Keyboard_1                  en0_x>9&&en0_x<140&&en0_y>35&&en0_y<94      //输入1
// #define Keyboard_2                  en0_x>142&&en0_x<285&&en0_y>35&&en0_y<94     //输入2
// #define Keyboard_3                  en0_x>285&&en0_x<423&&en0_y>35&&en0_y<94      //输入3
// #define Keyboard_4                  en0_x>9&&en0_x<140&&en0_y>96&&en0_y<154    //输入4
// #define Keyboard_5                  en0_x>142&&en0_x<285&&en0_y>96&&en0_y<154     //输入5
// #define Keyboard_6                  en0_x>285&&en0_x<423&&en0_y>96&&en0_y<154      //输入6
// #define Keyboard_7                  en0_x>9&&en0_x<140&&en0_y>155&&en0_y<480   //输入7
// #define Keyboard_8                  en0_x>142&&en0_x<285&&en0_y>155&&en0_y<480     //输入8
// #define Keyboard_9                  en0_x>285&&en0_x<423&&en0_y>155&&en0_y<480    //输入9
// #define Keyboard_0                  en0_x>428&&en0_x<575&&en0_y>155&&en0_y<480       //输入0
// #define Keyboard_LEFT               en0_x>576&&en0_x<685&&en0_y>35&&en0_y<94     //左移
// #define Keyboard_RIGHT              en0_x>687&&en0_x<798&&en0_y>35&&en0_y<94      //右移
// #define Keyboard_END                en0_x>428&&en0_x<575&&en0_y>35&&en0_y<94      //移到末尾
// #define Keyboard_REMOVE             en0_x>576&&en0_x<685&&en0_y>96&&en0_y<480     //删除
// #define Keyboard_YES                en0_x>687&&en0_x<798&&en0_y>&en0_y>96&&en0_y<480    //确定
// #define Keyboard_DECIMAL_POINT      en0_x>428&&en0_x<575&&en0_y>96&&en0_y<154      //小数点
// #define Keyboard_CLOSE              en0_x>0&&en0_x<800&&en0_y>240&&en0_y<260      //隐藏









//摄像头采集图像的宽和高
#define  IMAGE_WIDTH  640
#define  IMAGE_HEIGHT  480

//行车记录仪录制时长（帧数）
#define RECORDER_FRAME 200    

#define CAT_STAT_IMAGE "/root/shiyan/CODE/Main.jpg"
#define TRALTIME_IMAGE "/root/shiyan/CODE/Realtime_video.jpg"
#define SETTINGS_IMAGE "/root/shiyan/CODE/caidan.jpg"
#define SETTINGS_LOGIN "/root/shiyan/CODE/login.jpg"
#define SETTINGS_SMS_LOGIN "/root/shiyan/CODE/yzm_login.jpg"
#define KEYBOARD_IMAGE  "/root/shiyan/CODE/stdout.jpg"



#define DIR_SAVE_VIDEO_PATH  "/root/shiyan/CODE/Video"
#define DIR_RECORDER_PATH "/root/shiyan/CODE/Recorder"


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

//短信
#define HTTP_TCP_PORT  80
#define SMS_IP  "118.31.68.22"


struct str_v_c_main
{
  int Interface; //主界面标志位
  int video_flgs;//打开倒车影像状态
  int set_flgs; //设置状态
  int mic_flgs; //是否开启麦克风
};



struct str_realtime_video
{
  int record_video_pth;//录制标志状态
  int round_pth;//录制标志状态
  int line_pth; //辅助线线的状态
  int round_flgs; //录制开启状态
};

struct str_recorder
{
int recorder_flgs;       //查看记录仪标志位
  int new_Camera_frame_flgs; //摄像头有没有新帧状态
};

struct str_keyboard
{
  int Keyboard_flgs; //键盘状态
char keyboard_out[100]; //键盘输出
char phone_number[12]; //手机号
char password[20]; //密码
char sms_input[5]; //验证码
};


 sem_t sem_record_video_flgs; //信号量
 sem_t sem_record_video_pth; //信号量
 sem_t sem_recorder_complete; //信号量
 sem_t sem_Realtime_video; //信号量
 sem_t sem3; //信号量
pthread_mutex_t  mutex_round_pth;  //录制互斥锁
pthread_mutex_t  mutex_round_flgs;  //互斥锁
pthread_mutex_t  mutex_en0_ABS_flgs;  //互斥锁
pthread_mutex_t  mutex_reading_flgs;  //互斥锁  表示摄像头的这块缓冲正在写
pthread_mutex_t  mutex_wr_buf_mp;  //互斥锁  表示缓冲和开发板正在读写

//识别状态
struct str_v_c_main s_v_c_main;
struct str_realtime_video s_realtime_video;
struct str_recorder s_recorder;
struct str_keyboard s_keyboard;

 

unsigned int buf_i;   //摄像头正在映射的那块内存号
 unsigned int *mp;  //开发板屏幕
unsigned int read_index;  //读到摄像头哪个缓冲的下标
char *write_buf; //缓冲区写完
unsigned int * read_buf;  //开发板读完
char* (p_buffer[4]);  //开发板读完
int recorder_frame;       //记录仪获取新帧的位置
//跟麦克风传递状态的套接字
int socket_fd;

//摄像头的信息
struct buffer {
        void *                  start;
        size_t                  length;
};
extern struct buffer *         buffers;

//登录页面枚举
typedef enum
{
    LOGIN_STATE_PASSWORD,
    LOGIN_STATE_INPUT_USER,
    LOGIN_STATE_INPUT_PASSWORD,
    LOGIN_STATE_IN_SMS,    //进入验证码模式
    LOGIN_STATE_ISTRATION,  //注册
    LOGIN_STATE_FORGOT_PASSWORD,//忘记密码
    LOGIN_STATE_YES,    //确认
    KEY_BOARD_STATE_TURETN, //键盘退出
    LOGIN_STATE_EXIT,
    
    LOGIN_STATE_IN_PASSWORD,
    LOGIN_STATE_SMS,      //短信登录模式
    LOGIN_STATE_GET_SMS,  //获取验证码  

}LOGIN_STATE;

typedef enum
{
  SET_STATE_SET,
  SET_STATE_LOGIN,
  SET_STATE_WIFI,
  SET_STATE_LANGUAGE ,
  SET_STATE_SYSTEM_INFO,
  SET_STATE_SHUTDOWN,
  SET_STATE_RETURN,
             
}SETTINGS;

//坐标
extern int en0_x; 
extern int en0_y;

unsigned int * lcd_fp;          //用来复位
//线程
 pthread_t  thread_ent0;        
 pthread_t  thread_video;
 pthread_t  thread_recorder;
pthread_t  thread_lcd_video;
pthread_t  thread_round1;
pthread_t  thread_video_key;
  pthread_t  thread_lcd_buf;
  pthread_t  thread_keyboard;
  //生成记录仪gif
  pthread_t  thread_image_jpg;        


//接口
int read_JPEG_file (char * filename, unsigned int *mp); //图片转数据
void Abs_Cat( int event_fd, struct input_event * ev0);  //获取坐标

//退出圆、辅助线线程
void  lcd_round1_exit();
void  lcd_line_exit();

void Open_Video();      //打开摄像头
// 在 video_car.c 的最顶部添加：
int open_device(void);
static void init_device(void);
static void start_capturing(void);
static void mainloop(void);
int yuyv_to_rgb_pix(int Y,int Cb,int Cr);
void yuv2_rgb(unsigned int* read_buf,  char *write_buf);

static void stop_capturing ();

static void uninit_device ();

static void close_device ();


void Line_Car(unsigned int *mp);    //辅助线
GLOBAL(void)
write_JPEG_file (char * filename, int quality,  char   *buf); //数据变成图片
void File_time(char * filetime);  //生成时间戳字符串赋给指针
//线程函数
void *child_ent0(void * a);
void *child_Realtime_buf(void * a);
void *child_video(void * a);
void * child_Realtime_video(void * a);
void * lcd_round1(void * a);
void * Realtime_video_key(void * a);
void *child_keyboard(void * a);  //键盘线程
void *child_recorder(void * a);
void * recorder_image_jpg(void *a);
void *Record_Video(void *a);


void round1(); //显示录制圆点
void en0_clear();//坐标清零

//简化代码
void Pthread_Mutex_Init();
void Main_Interface_init();
void Pthread_Init();
void Pthread_Join();
void Realtime_Video(); //打开实时画面
void Shut_Down();

//主界面功能
void Settings();

//设置界面功能
void Login();
void Key_Board_Open(); //打开键盘

//显示字体
void showfont(int x,int y,int size,int color,const char * font );

//键盘
char* Key_Board(unsigned int * keyboard_mp,char *p);
//验证码
int  SMS(char * phone_number);
void SMS_Login();


//语音识别的信号
int socket_c();
void open_mic();

void Mmap_Bmp(int lcd_fd,FILE * bmp_fd,unsigned int *mp); //另一个项目
FILE * Open_Bmp(char * pathname);   //另一个项目的
#endif