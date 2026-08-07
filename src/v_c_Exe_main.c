#include "video_car.h"


int en0_x, en0_y;


void Main_Interface_init()
{
 // 显示开机动画

  // char pdf[100]={0};
  //   for(int i=0;i<177;i++)
  //     {
  //       sprintf(pdf,"LX/FLLX%d.jpg",i);
  //       read_JPEG_file (pdf, mp);
  //       usleep(1000);
  //     }    
      
 //显示主界面

 read_JPEG_file (CAT_STAT_IMAGE, mp);
   printf("1\n");
s_v_c_main.Interface=1;
s_v_c_main.video_flgs=0;

s_realtime_video.round_pth=0;       
s_realtime_video.round_flgs=0;       
s_realtime_video.line_pth=0;
s_realtime_video.record_video_pth=0;

s_v_c_main.set_flgs=0;
s_v_c_main.mic_flgs=0;
s_recorder.recorder_flgs=0;
s_recorder.new_Camera_frame_flgs=0;

recorder_frame=0;

s_keyboard.Keyboard_flgs=-1;

memset(s_keyboard.keyboard_out,0,sizeof(s_keyboard.keyboard_out));
memset(s_keyboard.phone_number,0,sizeof(s_keyboard.phone_number));
memset(s_keyboard.password,0,sizeof(s_keyboard.password));


write_buf=NULL;

 //初始化信号量
 sem_init(&sem_record_video_pth, 0, 0);
 sem_init(&sem_record_video_flgs, 0, 0);
 sem_init(&sem_recorder_complete, 0, 0);
  sem_init(&sem_Realtime_video, 0, 0);
 //减信号量
    // sem_wait( &sem);
 //加信号量
//     sem_post( &sem);
}


void Pthread_Mutex_Init()
{
   //初始化互斥锁
   
    if(0!=pthread_mutex_init(&mutex_round_pth, NULL))
    {
      fprintf(stderr,"初始化互斥锁失败%s",errno);
      exit(1);
    }  
    if(0!=pthread_mutex_init(&mutex_round_flgs, NULL))
    {
      fprintf(stderr,"初始化互斥锁失败%s",errno);
      exit(1);
    }  
    if(0!=pthread_mutex_init(&mutex_en0_ABS_flgs, NULL))//坐标xy的锁
    {
      fprintf(stderr,"初始化互斥锁失败%s",errno);
      exit(1);
    }  
      if(0!=pthread_mutex_init(&mutex_reading_flgs, NULL))//读取摄像头缓冲区的锁
    {
      fprintf(stderr,"初始化互斥锁失败%s",errno);
      exit(1);
    }   
      if(0!=pthread_mutex_init(&mutex_wr_buf_mp, NULL))//写入lcd屏缓冲区的锁
    {
      fprintf(stderr,"初始化互斥锁失败%s",errno);
      exit(1);
    }  
     //上锁
//   pthread_mutex_lock(&mutex);

//解锁
//  pthread_mutex_unlock( &mutex);
}



void Pthread_Init()
{
     //创建线程
    pthread_create(&thread_ent0, NULL,child_ent0,NULL);//触控，永远打开，除非系统退出，不需要exit
    pthread_create(&thread_video, NULL,child_video,NULL);//打开摄像头，永远打开
    pthread_create(&thread_recorder, NULL,child_recorder,NULL);//打开记录仪
}
void Pthread_Join()
{
  pthread_join(thread_ent0, NULL);
  pthread_join(thread_video, NULL);
  printf("等待记录仪\n");
  pthread_join(thread_recorder, NULL);
}



void Realtime_Video() //打开实时画面
{
  s_v_c_main.Interface=0;//切换界面，主界面标志位关闭
  s_v_c_main.video_flgs=1;//切换到lcd摄像
  printf("用户点击打开相机\n");
  read_JPEG_file (TRALTIME_IMAGE,lcd_fp);
        
//将摄像头采集的图片放在buffer_mp里
  pthread_create(&thread_lcd_buf, NULL,child_Realtime_buf,NULL);
//将处理过的图片到开发板
  pthread_create(&thread_lcd_video, NULL,child_Realtime_video,NULL);
  //影像按键检测线程,要恢复状态
  pthread_create(&thread_video_key, NULL,Realtime_video_key,NULL);
  printf("round_pth1=%d\n",s_realtime_video.round_flgs);
  if(s_realtime_video.round_flgs==1)
  {
    pthread_create(&thread_round1, NULL,lcd_round1,NULL);

  }
  //堵塞
  pthread_join(thread_video_key, NULL);
  printf("1\n");
  pthread_join(thread_lcd_video, NULL);
  printf("1\n");
  pthread_join(thread_lcd_buf, NULL);
  printf("1\n");
  en0_clear();
  //挺多共用的出来再关    
  s_v_c_main.video_flgs=0;//退出lcd摄像
  //添加退出按钮
}

void Shut_Down()
{
  Pthread_Join();
  printf("已清空内存\n");
  en0_clear();
  printf("已清空内存\n");
  for(int i=0;i<HEIGHT;i++)
  for(int j=0;j<WIDTH;j++)
  mp[j+i*WIDTH]=0x00000000;

  close(socket_fd);
  close((int)lcd_fp);
  munmap(mp,800*480*4);
  exit(0);
}

int main()
{
    //打开设备与文件
    int lcd_fd=open("/dev/fb0",O_RDWR);
    if(-1==lcd_fd)
    {
        perror("open lcd failed");
        exit(0);
    }
    //映射内存
                    mp=mmap(
                            NULL,
                            HEIGHT*WIDTH*4,
                            PROT_READ|PROT_WRITE,
                            MAP_SHARED,
                            lcd_fd,
                            0);
      lcd_fp=mp;
 //开机
  Main_Interface_init();


//初始化互斥锁
  Pthread_Mutex_Init();
  
//初始化线程
  Pthread_Init();

// showfont(200,200,100,0xff0000,"小鸡");
  //一个按键一个状态，所以可以阻塞




  for(;;)
  {   
      if( OPEN_FILM&&s_v_c_main.Interface==1&&s_v_c_main.video_flgs==0)//在主界面点击此坐标有效,进入倒车影像界面
          {
            //打开显示屏实时画面
                Realtime_Video();
              read_JPEG_file (CAT_STAT_IMAGE, mp);

          } //
          else if(MAIN_SETTINGS &&s_v_c_main.Interface==1)//进入设置
          {
            s_v_c_main.set_flgs=1;
            en0_clear();
            printf("进入设置\n");
            Settings();
            en0_clear();
            //退出时清理坐标再清空标志位
            s_v_c_main.Interface=1;
            s_v_c_main.set_flgs=0;
            read_JPEG_file (CAT_STAT_IMAGE, mp);
          }
          else if(MAIN_MIC &&s_v_c_main.Interface==1)  //点击麦克风
          {
            en0_clear();
            open_mic();
          }
          //之后改成文件夹，点开后可选择看记录仪或录像，现在直接播放行车记录仪
          else if(MAIN_RECORD_VIDEO_SAVE &&s_v_c_main.Interface==1&&s_recorder.recorder_flgs==0)  //点击行车记录仪
          {               
             sem_wait(&sem_recorder_complete);
            //确认两个线程都进入记录仪
             s_recorder.recorder_flgs=1;
             printf("开始播放记录仪\n");
             pthread_create(&thread_image_jpg, NULL,recorder_image_jpg,NULL);
             sem_post(&sem_recorder_complete);
              // 生成视频完毕，可以播放
          }
          else if(MAIN_DIR &&s_v_c_main.Interface==1&&s_recorder.recorder_flgs==0)  //点击文件夹，要有给文件夹上锁功能
          {
              //显示文件夹，lvgl


          }
          else if( MAIN_SRCEENSHOT&&s_v_c_main.Interface==1&&s_recorder.recorder_flgs==0)  //音乐lvgl
          {


          }
          else if( MAIN_RECORD_AWA&&s_v_c_main.Interface==1&&s_recorder.recorder_flgs==0)  //点击录音，鸡肋
          {


          }
            
          else if( MAIN_LOCK_SCREEN&&s_v_c_main.Interface==1&&s_recorder.recorder_flgs==0)  //锁屏
          {


          }
              s_v_c_main.Interface=1;//退出转到主界面
  }
  
}

//要做的：
//添加手动录像，截图，放在不同位置
//且画线缺失，保存图片要链表，且要点击后播放,还没释放内存，
//将动态麦克风贴上去
/*
录像:缓冲区有用吗？没用，每一帧都保存？不要。采用lcd屏一样的思路。
//拓展功能，如截图
//语音识别，文件夹页面
//看笔记
//看别人代码
//看面试题
/*(3)行车记录仪具备广告播放功能，可以为汽车厂商或第三方服务商提供广告发布服务，用于发布宣传视频、活动信息、紧急通告等。

(4)用户可以通过手机APP远程查看行车记录仪的实时画面，并能够调取历史录像。系统支持按时间、地点等条件进行录像检索，方便用户快速找到所需视频。

(5)系统支持会员功能，用户可以通过注册会员享受个性化设置和增值服务。会员用户可以自定义行车记录仪的工作模式、报警灵敏度等参数，并能够享受云端存储空间的扩展服务。

(6)系统具备碰撞检测功能，当检测到车辆发生碰撞时，自动保存碰撞前后的视频片段，并上传到云端。用户可以通过手机APP查看碰撞录像，便于事故处理。

(7)系统支持车辆健康监测功能，能够实时监测车辆的发动机、轮胎、电池等关键部件的状态。当检测到异常时，系统会发出故障预警，提醒用户及时维修。

(8)要求设计产品LOGO，并且项目功能可以满足用户实际需求，另外自行拓展其他实用性功能，确保项目运行没有BUG，并编写项目测试文档。*/




/************************************************* 
*代码托管网站
*阅读Git hub项目
*面试题
*老师和自己笔记
*记单词


*题库、
*部署本地DeepSeek、
*试卷、
*学习状态机，
*调试，
*阅读文档，
man手册，
***************************************************/