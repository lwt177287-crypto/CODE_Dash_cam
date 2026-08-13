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





//两个登录界面按钮
#define LOGIN_USERNAME     en0_x>255&&en0_x<347&&en0_y>163&&en0_y<193     //手机号
#define LOGIN_PASSWORD     en0_x>255&&en0_x<347&&en0_y>228&&en0_y<258     //密码、验证码
#define LOGIN_YES      en0_x>255&&en0_x<545&&en0_y>296&&en0_y<330    //登录按钮
  #define LOGIN_FORGOT_PASSWORD     en0_x>505&&en0_x<547&&en0_y>270&&en0_y<280  //忘记密码
#define LOGIN_SMS     en0_x>252&&en0_x<302&&en0_y>270&&en0_y<280     //短信登录 
#define LOGIN_IFICATION      en0_x>411&&en0_x<453&&en0_y>349&&en0_y<362     // 注册
#define LOGIN_RETURN      en0_x>368&&en0_x<423&&en0_y>383&&en0_y<410     // 退出
#define LOGIN_GET_SMS      en0_x>468&&en0_x<550&&en0_y>233&&en0_y<263     // 获取验证码

//注册
#define INSTRATTON_USERNAME     en0_x>245&&en0_x<336&&en0_y>123&&en0_y<157     //手机号
#define INSTRATTON_GET_SMS      en0_x>490&&en0_x<551&&en0_y>187&&en0_y<222     // 获取验证码
#define INSTRATTON_SMS          en0_x>245&&en0_x<336&&en0_y>187&&en0_y<222      //输入验证码
#define INSTRATTON_PASSWORD     en0_x>245&&en0_x<336&&en0_y>252&&en0_y<281     //输入密码
#define INSTRATTON_AGAIN_PASSWORD    en0_x>245&&en0_x<336&&en0_y>290&&en0_y<323     //密码、验证码
#define INSTRATTON_YES          en0_x>245&&en0_x<557&&en0_y>339&&en0_y<372    //立即注册
#define INSTRATTON_LOGIN        en0_x>351&&en0_x<447&&en0_y>390&&en0_y<406     // 返回登录
#define INSTRATTON_RETURN       en0_x>364&&en0_x<424&&en0_y>420&&en0_y<442    // 退出

//账户信息页面
#define ACCOUNT_VIP_RECHARGE          en0_x>200&&en0_x<400&&en0_y>183&&en0_y<246    // 充值vip
#define ACCOUNT_SAFE                  en0_x>200&&en0_x<400&&en0_y>253&&en0_y<317    // 账号安全
#define ACCOUNT_DEVICE_MANAGEMENT     en0_x>200&&en0_x<400&&en0_y>323&&en0_y<391    // 设备管理
#define ACCOUNT_EXIT                  en0_x>200&&en0_x<400&&en0_y>395&&en0_y<458    //账号退出
#define ACCOUNT_RETURN                en0_x>750&&en0_x<781&&en0_y>18&&en0_y<50    //账号退出


     
enum Account_start
{
  ACCOUNT_START,  //选择
  ACCOUNT_START_VIP_RECHARGE,//会员充值
  ACCOUNT_START_SAFE, //账号安全
  ACCOUNT_START_DEVICE_MANAGEMENT,  //管理设备账号
  ACCOUNT_START_EXIT,   //退出账号
  ACCOUNT_START_RETURN  //返回上一界面
  
};


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
#define SETTINGS_ACCOUNT "/root/shiyan/CODE/account.jpg"
#define SETTINGS_SMS_LOGIN "/root/shiyan/CODE/yzm_login.jpg"
#define SETTINGS_ISTRATION "/root/shiyan/CODE/istration.jpg"
#define KEYBOARD_IMAGE  "/root/shiyan/CODE/stdout.jpg"



#define ADMIN_DATA_PATH "/root/shiyan/CODE/admin.txt"

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
  int record_video_pth;//录制获取帧状态
  int record_video_flgs;//录制开启状态
  int record_video_exit;//录制关闭
  int round_pth;//录制标志闪烁状态
  int round_flgs; //录制标志开启状态
  int round_exit;//录制标志关闭
  //当录像开启，两个小状态跟着开启，因为关闭条件有些不一样就没弄
  int record_stat;//整个录像开启
  
  int line_pth; //辅助线的状态

};

struct str_recorder
{
int recorder_flgs;       //查看记录仪标志位
int new_Camera_frame_flgs; //摄像头有没有新帧状态
int recorder_play;    //播放记录仪
int recorder_create;    //创建记录仪回放

};
 
struct str_settings
{
  int shutdown_flgs;
};
struct str_keyboard
{
    int Keyboard_flgs; //键盘状态
  char keyboard_out[100]; //键盘输出
  char phone_number[12]; //手机号
  char password[20]; //密码
  char again_password[20]; //再次密码
  char sms_input[5]; //验证码
};

//账号信息
struct str_admin
{
  //账户信息
char phone_number[12]; //手机号
char password[20]; //密码
  //会员
  int vip;
  //状态
  int user_input;
 int password_input;
 int password_again_input;
 int sms_input; //是否点击了验证码输入
  //登录状态
int admin_login_flgs;//此账号在此设备的登录状态，若为1点击登录弹出退出登录界面,升级VIP界面
//系统发来的验证码
 char sms_out[5];
};

struct str_admin s_admin;

//手机号+验证码
 struct str_SMS_verification_code 
 {
   char phone_Number[12];
  int sms_out;
 };
 struct str_SMS_verification_code s_SMS_verification_code;


 sem_t sem_record_video_flgs; //线程创建成功发送信号给退出线程，若重复点击录像也不会段错误
 sem_t sem_record_video_pth; //开启录像后，缓冲区获得新的yuyv数据告诉录像线程
 sem_t sem_recorder_complete; //
 sem_t sem_Realtime_video; //信号量
pthread_cond_t  sem_en0_ABS;
 pthread_cond_t  sem_key;


 
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
struct str_settings s_settings;

 

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
    LOGIN_STATE_PASSWORD, //密码登录模式
    LOGIN_STATE_INPUT_USER,
    LOGIN_STATE_INPUT_PASSWORD,
    LOGIN_STATE_IN_SMS,    //进入验证码模式
    LOGIN_STATE_ISTRATION,  //注册
    LOGIN_STATE_FORGOT_PASSWORD,//忘记密码
    LOGIN_STATE_YES,    //确认
    LOGIN_KEY_BOARD_STATE_TURETN, //键盘退出
    LOGIN_STATE_EXIT,   //退出
    
    LOGIN_STATE_IN_PASSWORD,//进入密码登录模式
    LOGIN_STATE_SMS,      //短信登录模式
    LOGIN_STATE_GET_SMS,  //获取验证码  

}LOGIN_STATE;

//登录注册枚举
typedef enum
{
    INSTRATTON_STATE, //注册模式
   INSTRATTON_STATE_INPUT_USER,//输入手机号
   INSTRATTON_STATE_GET_SMS,  //获取验证码  
   INSTRATTON_STATE_INPUT_SMS,//输入验证码
   INSTRATTON_STATE_INPUT_PASSWORD,//输入密码
    INSTRATTON_STATE_AGAIN_INPUT_PASSWORD,//再次输入密码
   INSTRATTON_STATE_YES,    //确认
   INSTRATTON_KEY_BOARD_STATE_TURETN, //键盘退出
   INSTRATTON_STATE_EXIT,   //退出
   INSTRATTON_STATE_LONIG,   //返回登录

}E_LOGIN_INSTRATTON;





//设置界面按钮
typedef enum
{
  SET_STATE_SET,
  SET_STATE_LOGIN,
  SET_STATE_WIFI,
  SET_STATE_LANGUAGE,
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
pthread_t  thread_record_video;
pthread_t  thread_video_key;
  pthread_t  thread_lcd_buf;
  pthread_t  thread_keyboard;
  pthread_t  thread_flgs_ctrl;
  //生成记录仪gif
  pthread_t  thread_image_jpg;  
  //lvgl线程      
  pthread_t  thread_lvgl_key;        


//接口
int read_JPEG_file (char * filename, unsigned int *mp); //图片转数据
void Abs_Cat( int event_fd, struct input_event * ev0);  //获取坐标

//退出圆、辅助线线程
void  lcd_round1_exit();
void  lcd_line_exit();

void Open_Video();      //打开摄像头
// 在 video_car.c 的最顶部添加：
int open_device(void);

int yuyv_to_rgb_pix(int Y,int Cb,int Cr);
void yuv2_rgb(unsigned int* read_buf,  char *write_buf);

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
void * child_flgs_ctrl(void* a);
void * child_lvgl_key(void* a);




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
void Clear_Keyboard(char *num,char *keyboard_out,int size);
void Input_Info(char *num,char * keyboard_out,int size);
//登录注册界面
//验证码 
int  SMS(char * phone_number);
void SMS_Login();
void SMS_Get();
int  SMS_YES();

//注册
void Login_Istration();

//账号
void Account_Info();

//状态栏
void Status_Bar();


//退出
void Return_Set();    //返回设置界面


//语音识别的信号
int socket_c();
void open_mic();


//LVGL
int Lvgl_Key();


void Mmap_Bmp(int lcd_fd,FILE * bmp_fd,unsigned int *mp); //另一个项目
FILE * Open_Bmp(char * pathname);   //另一个项目的
#endif