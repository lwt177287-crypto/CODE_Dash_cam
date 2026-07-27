#include "video_car_exe.h"
//感悟：捋好每一步逻辑，标志位记录功能开关，信号量控制执行顺序，互斥锁防止同时访问
//重复代码段减少，且将一些特定的步骤封装成函数，不要将代码搞得太复杂，无用步骤别做
//线程间配合紧密，触摸参数xy要及时清零但不能提前，这个坐标没有用再清
//一堆小线程要同时退出，一样的步骤应在大线程退出里执行
//先传信号再锁，要有空间再给地址，不能影响摄像头实时性，应考虑功能的特殊需求
//循环不能卡住，要大退出时应进入按键负责的大退出模块等待小线程全部退出，小退出应该把状态复到未进线程时
//标志位是检验按钮开关的，不是让你用来实现暂停的，线程标志位是让你检验线程状态的
//互斥锁和信号量要细化，可以很好的给不同的内存一起访问，而同一块内存禁止访问
//点击按键坐标是否有效，要看现在在哪个界面，每个坐标判断都要加当前界面判断
//关机要释放资源
void  lcd_round1_exit();
void  lcd_line_exit();

void *child_ent0(void * a)
{
     int event_fd=open("/dev/input/event0",O_RDWR);
    if(-1==event_fd)
    {
        perror("open event_fd falied");
        exit(0);
    }
     struct input_event * ev0=calloc(sizeof(struct input_event),1);
     for(;;)
        {  
            //关机检测
                 if(CLOSE&&Interface==1) 
                 {
                    free(ev0);
                    close(event_fd);
                    pthread_exit(NULL);
                 }
              else   //检测触控
                Abs_Cat(event_fd,ev0);
        }

}

//打开摄像头
void *child_video(void * a)
{
  //在里面释放
    Open_Video();
    pthread_exit(NULL);

}

//传入buffer
void *child_lcd_buf(void * a)
    {

    //创建两个缓冲，写完第一个给lcd读，同时写第二个，lcd屏读完第一个读第二个，
    buffer_mp=(unsigned int *)
    calloc(2,IMAGE_HEIGHT *IMAGE_WIDTH*sizeof(unsigned int));
        printf("wirte_size=%d\n",IMAGE_HEIGHT *IMAGE_WIDTH*sizeof(unsigned int));
        write_buf=buffer_mp;
       read_buf=buffer_mp+IMAGE_HEIGHT *IMAGE_WIDTH;

    for(;;)
    {  

          pthread_mutex_lock(&mutex_reading_flgs);
            memcpy(write_buf,p_buffer[read_index],IMAGE_WIDTH*IMAGE_HEIGHT*2);        
            //此时write_buf已经有图片数据
          pthread_mutex_unlock(&mutex_reading_flgs);

            //转换格式,将图片塞进read_buf里
         pthread_mutex_lock(&mutex_wr_buf_mp);
          yuv2_rgb((unsigned int *) read_buf,(char *)write_buf);  //将w写到r，r供用户读

          if(round_flgs==1&&round_pth==1)//显示圆圈
        {
            round1();
            pthread_mutex_lock(&mutex_round_flgs);//上锁防止冲突
            round_flgs=0;
            pthread_mutex_unlock( &mutex_round_flgs);//解锁
        }      

        if(line_pth==1)//显示辅助线 
        {
            Line_Car( read_buf);
        }
            pthread_mutex_unlock(&mutex_wr_buf_mp);

        if(EIXT_FILM&&video_flgs==1)
        {
            printf("退出传输buffer\n");
            pthread_mutex_unlock( &mutex);//解锁
            sem_post(&sem2);
            pthread_exit(NULL);
        }
            pthread_mutex_unlock(&mutex_wr_buf_mp);
            sem_post(&sem2);
    }  
}


//显示图片
void * child_lcd_video(void * a)
{
    for(;;)
    {       
        //等待处理过的照片
             sem_wait(&sem2);
        
         pthread_mutex_lock(&mutex_wr_buf_mp);
                  for(int i=0;i<480;i++)
                {
                    memcpy(mp+i*WIDTH+80,read_buf+i*IMAGE_WIDTH,IMAGE_WIDTH*4);
                }     

                pthread_mutex_unlock(&mutex_wr_buf_mp);
 
                if(EIXT_FILM&&video_flgs==1)
                {
                    close(buffer_mp);
                    printf("退出摄像，这是显示画面模块\n");
                    pthread_exit(NULL);
                }

    }  }


//辅助线
void * lcd_line(void * a)
{
   for(;;)
    {       
        //当辅助线重复按表示关闭
        if(GUIDELINE&&line_pth==1)
        {
             en0_clear();
            lcd_line_exit();
        }
        if(EIXT_FILM&&video_flgs==1&&line_pth==1)   
        {
            lcd_line_exit();

        }
    }
          
}

//line_pth=1为开
void  lcd_line_exit()
{
          line_pth=0;
          printf("退出划线\n");
          pthread_exit(NULL);
}



//显示录制标志
void * lcd_round1(void * a)
{
                
    for(;;)
    {
            //再次点击关闭
        if(RECORD&&round_pth==1)   
            {  
                 en0_clear();
                 lcd_round1_exit();
            } 
            //关闭整个摄像机
         if(EIXT_FILM&&video_flgs==1&&round_pth==1)   
            {
                 lcd_round1_exit();
            }
            pthread_mutex_lock(&mutex_round_flgs);//上锁防止冲突
            round_flgs=1;   //开启圆点
            pthread_mutex_unlock( &mutex_round_flgs);//解锁
            usleep(1000*1000);
    }
}
    void  lcd_round1_exit()
{
    round_pth=0;
    printf("退出录制！\n");
    pthread_exit(NULL); 
} 



//进入摄像后的按键
void * lcd_video_key(void * a)
{
    printf("进入key\n");
    pthread_t  thread_line;
    pthread_t  thread_round1;

    for(;;)
    {
        //点击退出按键
        if(EIXT_FILM&&video_flgs==1)   
        {
            printf("退出key\n");
            if( line_pth==1)
            {  
                pthread_join(thread_line, NULL);
                line_pth=0;
            }
            if( round_pth==1)
            {
                pthread_join(thread_round1, NULL);
                round_pth=0;
            }

                video_flgs==0;
                pthread_exit(NULL);         
        }
            //点击录像按键
        else if(RECORD &&video_flgs==1 &&round_pth==0)
        {
            en0_clear();
            round_pth=1;
            printf("进入录像\n");
            //显示录制标志
            pthread_create(&thread_round1, NULL,lcd_round1,NULL);
            //开始录像
            
        }

        else if(GUIDELINE &&video_flgs==1 &&line_pth==0)
        {
            en0_clear();
            line_pth=1;
            printf("进入辅助线\n");
            pthread_create(&thread_line, NULL,lcd_line,NULL);
        }   
    } 

}