#include "video_car.h"


int en0_x, en0_y;


void Main_Interface_init()
{
 // 显示开机动画

  // char pdf[177]={0};
  //   for(int i=0;i<177;i++)
  //     {
  //       sprintf(pdf,"LX/FLLX%d.jpg",i);
  //       read_JPEG_file (pdf, mp);
  //       usleep(1000);
  //     }    
      


  //广告
  
// if(s_admin.admin_login_flgs!=1)
// {
//    char pdf[177]={0};
//  int now_time=time(NULL);
//     for(int i=0;i<=71;i++)
//       {
        
//         sprintf(pdf,"/root/shiyan/CODE/guanggao/Frame%d.jpg",i);
//         read_JPEG_file (pdf, mp);
//         usleep(50000);
//         if(time(NULL)-now_time>=5)
//         {
//           //点击跳过
//         }
//       }    
// }
 
 
 
  //显示主界面


//  read_JPEG_file (CAT_STAT_IMAGE, mp);
memset(&s_v_c_main,0,sizeof(s_v_c_main));
memset(&s_realtime_video,0,sizeof(s_realtime_video));
memset(&s_recorder,0,sizeof(s_recorder));
memset(&s_keyboard,0,sizeof(s_keyboard));
memset(&s_settings,0,sizeof(s_settings));
memset(&s_instration_re,0,sizeof(s_instration_re));
memset(&s_SMS_verification_code,0,sizeof(s_SMS_verification_code));
memset(&s_admin,0,sizeof(s_admin));

s_v_c_main.Interface=1;
s_keyboard.Keyboard_flgs=-1;

// memset(s_keyboard.keyboard_out,0,sizeof(s_keyboard.keyboard_out));
// memset(s_keyboard.phone_number,0,sizeof(s_keyboard.phone_number));
// memset(s_keyboard.password,0,sizeof(s_keyboard.password));

write_buf=NULL;

 //初始化信号量
 sem_init(&sem_record_video_pth, 0, 0);
 sem_init(&sem_record_video_flgs, 0, 0);
 sem_init(&sem_recorder_complete, 0, 0);
sem_init(&sem_Realtime_video, 0, 0);
sem_init(&sem_lvgl, 0, 0);
sem_init(&sem_input_info, 0, 0);
pthread_cond_init(&sem_en0_ABS, NULL);
pthread_cond_init(&sem_key, NULL);


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
      fprintf(stderr,"初始化互斥锁失败%s",strerror(errno));
      exit(1);
    }  
    if(0!=pthread_mutex_init(&mutex_round_flgs, NULL))
    {
      fprintf(stderr,"初始化互斥锁失败%s",strerror(errno));
      exit(1);
    }  
    if(0!=pthread_mutex_init(&mutex_en0_ABS_flgs, NULL))//坐标xy的锁
    {
      fprintf(stderr,"初始化互斥锁失败%s",strerror(errno));
      exit(1);
    }  
      if(0!=pthread_mutex_init(&mutex_reading_flgs, NULL))//读取摄像头缓冲区的锁
    {
      fprintf(stderr,"初始化互斥锁失败%s",strerror(errno));
      exit(1);
    }   
      if(0!=pthread_mutex_init(&mutex_wr_buf_mp, NULL))//写入lcd屏缓冲区的锁
    {
      fprintf(stderr,"初始化互斥锁失败%s",strerror(errno));
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
    printf("1\n");
    pthread_create(&thread_recorder, NULL,child_recorder,NULL);//打开记录仪
    printf("2\n");
    pthread_create(&thread_flgs_ctrl, NULL,child_flgs_ctrl,NULL);//控制按钮
    printf("3\n");   
     pthread_create(&thread_lvgl_key, NULL,child_lvgl_key,NULL);//lvgl按钮
    printf("4\n");
    pthread_t thread_stream;
    pthread_create(&thread_stream, NULL, stream_server_thread, NULL);//推流服务器
    pthread_detach(thread_stream);
    printf("5\n");
}
void Pthread_Join()
{
  printf("等待记录仪\n");
  pthread_join(thread_ent0, NULL);
  printf("等待记录仪\n");
  pthread_join(thread_video, NULL);
  printf("等待记录仪\n");
  pthread_join(thread_recorder, NULL);
  printf("等待记录仪\n");
  pthread_join(thread_flgs_ctrl,NULL);
  printf("等待记录仪\n");

}



void Realtime_Video() //打开实时画面
{
  s_main_btn[REALTIME_VIDEO_BTN].btn=0;
  s_v_c_main.Interface=0;//切换界面，主界面标志位关闭
  s_v_c_main.video_flgs=1;//切换到lcd摄像
  printf("用户点击打开相机\n");
  // read_JPEG_file (TRALTIME_IMAGE,lcd_fp);
        
//将摄像头采集的图片放在buffer_mp里
  pthread_create(&thread_lcd_buf, NULL,child_Realtime_buf,NULL);
//将处理过的图片到开发板
  pthread_create(&thread_lcd_video, NULL,child_Realtime_video,NULL);
  //影像按键检测线程,要恢复状态
  pthread_create(&thread_video_key, NULL,Realtime_video_key,NULL);
  if(s_realtime_video.round_flgs==1)
  {
    pthread_create(&thread_round1, NULL,lcd_round1,NULL);

  }
  //堵塞
  pthread_join(thread_video_key, NULL);
  printf("111\n");
  pthread_join(thread_lcd_video, NULL);
  printf("222\n");
  pthread_join(thread_lcd_buf, NULL);
  printf("333\n");
  en0_clear();
  //挺多共用的出来再关    

  memset(s_realtime_video_btn,0,sizeof(s_realtime_video_btn));


  s_v_c_main.video_flgs=0;//退出lcd摄像
  //添加退出按钮
}

void Shut_Down()
{
  printf("准备关机工作\n");
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


//此线程还未关闭
void * child_flgs_ctrl(void* a)
{
  printf("进入按键检测\n");
  for(;;)
  {
    pthread_mutex_lock(&mutex_en0_ABS_flgs);
    pthread_cond_wait(&sem_en0_ABS,&mutex_en0_ABS_flgs);
    printf("唤醒\n");
    pthread_mutex_unlock(&mutex_en0_ABS_flgs);
 
    if(s_v_c_main.video_flgs==1)
    {
      // printf("s_v_c_main.video_flgs=%d\n",s_v_c_main.video_flgs);
      // printf("s_realtime_video.round_flgs=%d\ns_realtime_video.record_video_flgs=%d\n",
      // s_realtime_video.round_flgs,s_realtime_video.record_video_flgs);
    
      //没有录像时点击录制
      if(REALTIME_RECOER&&s_realtime_video.round_flgs==0&& s_realtime_video.record_video_flgs==0)
      {
        printf("按下录制按钮\n");
        //通知
        s_realtime_video.record_stat=1;//可换成两个小的
        //将标志位打开
        //
        s_realtime_video.round_flgs=1;
        s_realtime_video.record_video_flgs=1;
      }
      //关闭
      //正在录制标志时点击录像
      //因为按下录制这两个是一起的
      else if(REALTIME_RECOER&&s_realtime_video.round_flgs==1&&s_realtime_video.record_video_flgs==1)   
      {
        printf("按下关闭按钮\n");
        s_realtime_video.round_exit=1;
        s_realtime_video.record_video_exit=1;
        sem_wait(&sem_record_video_flgs);
        //等待线程创建成功，才能退出
     
        pthread_join(thread_round1,NULL);
        pthread_join(thread_record_video,NULL);
     
        printf("退出两个线程成功！\n");
        s_realtime_video.round_flgs=0;  
        s_realtime_video.record_video_flgs=0;

      }
    }
    else if(s_v_c_main.Interface==1)
    {
      //点击保存记录仪按钮
    //   if(MAIN_RECORD_VIDEO_SAVE&&s_recorder.recorder_flgs==0)
    //   {
    //     // s_recorder.recorder_play=1;
    //     // s_recorder.recorder_create=1;
    //     // s_recorder.recorder_flgs=1;
    //   }
    // }
    // else if(s_v_c_main.set_flgs==1)
    // {
    //    if(SET_SHUTDOWN)
    //     {
    //       //关机
    //       // printf("检测到按下关机按钮\n");
    //       // s_settings.shutdown_flgs=1;
    //       // pthread_exit(NULL);
    //     }
    
  
    }
 

  }
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

  // Status_Bar();
// Lvgl_Key();


  for(;;)
  {   
    // printf("s_main_btn[REALTIME_VIDEO_BTN].btn=%d\n",s_main_btn[REALTIME_VIDEO_BTN].btn);
    // if( OPEN_FILM&&s_v_c_main.Interface==1&&s_v_c_main.video_flgs==0)//在主界面点击此坐标有效,进入倒车影像界面
    if(s_main_btn[REALTIME_VIDEO_BTN].btn==1)
      {
        //打开显示屏实时画面  
            Realtime_Video();
          // read_JPEG_file (CAT_STAT_IMAGE, mp);
      } //
    else if(s_main_btn[SETTINGS_BTN].btn==1)//进入设置
     {
        s_main_btn[SETTINGS_BTN].btn=0;
        s_v_c_main.set_flgs=1;
        en0_clear();
        printf("进入设置\n");
        Settings();
        printf("退出设置\n");
        en0_clear();
        //退出时清理坐标再清空标志位
        s_v_c_main.set_flgs=0;
        // read_JPEG_file (CAT_STAT_IMAGE, mp);
     }
    else if(MAIN_MIC &&s_v_c_main.Interface==1)  //点击麦克风
      {
        en0_clear();
        open_mic();
      }
      //之后改成文件夹，点开后可选择看记录仪或录像，现在直接播放行车记录仪
  
      else if(s_main_btn[MAIN_REALTIME_RECOER_BTN].btn==1&& s_recorder.recorder_flgs==0)  //点击行车记录仪
      {
        s_main_btn[MAIN_REALTIME_RECOER_BTN].btn=0;
        
        //没做重复点击检测
        //当视频准备好，就播放，现在视频生成实在太久，不用理会
        printf("开始播放记录仪\n");
        // 生成视频完毕，可以播放
        s_recorder.recorder_create=1;
        s_recorder.recorder_flgs=1;
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
//且画线缺失，保存图片要链表，且要点击后播放,还没释放内存，
//将动态麦克风贴上去
/*
//拓展功能，如音乐
//文件夹页面

/*(3)会员可跳过：行车记录仪具备广告播放功能，可以为汽车厂商或第三方服务商提供广告发布服务，用于发布宣传视频、活动信息、紧急通告等。

(4)会员支持：用户可以通过手机APP远程查看行车记录仪的实时画面，并能够调取历史录像。系统支持按时间、地点等条件进行录像检索，方便用户快速找到所需视频。

(5)系统支持会员功能，用户可以通过注册会员享受个性化设置和增值服务。会员用户可以自定义行车记录仪的工作模式、报警灵敏度等参数，并能够享受云端存储空间的扩展服务。

(8)要求设计产品LOGO，并且项目功能可以满足用户实际需求，另外自行拓展其他实用性功能，确保项目运行没有BUG，并编写项目测试文档。

(9)添加lvgl按键

//等学到再做

(6)系统具备碰撞检测功能，当检测到车辆发生碰撞时，自动保存碰撞前后的视频片段，并上传到云端。用户可以通过手机APP查看碰撞录像，便于事故处理。

(7)系统支持车辆健康监测功能，能够实时监测车辆的发动机、轮胎、电池等关键部件的状态。当检测到异常时，系统会发出故障预警，提醒用户及时维修。




/************************************************* 
*阅读Git hub项目
*面试题
*老师和自己笔记
*记单词


*题库、
*部署本地DeepSeek、
*试卷、
*阅读文档，
man手册，
*调试，
***************************************************/
