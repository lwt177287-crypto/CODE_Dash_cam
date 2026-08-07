#include "video_car.h"
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

//焚诀：重复点击关闭时，可这样设计，一个大判断为是否点击与是否在此页面，然后小判断，判断次功能是否已经开启，若开启则关闭，若未开启则开启
//则关闭，若关闭则开启，坐标已经清空，不会开启的同时关闭，值得注意的是标志位都是出来再关闭，耦合度难免增加
//其实也可以加互斥锁，一个锁标志位改变，一个锁判断进入
//if判断进入每个线程是否都需要有一个判断标志位来识别是否退出
//坐标应该使用临时值来获取，再上锁统一赋值   新：读取堆积的事件，每个都处理，识别到SYN就输出坐标，若缺失一轴坐标则清空
//状态机比falgs好管理
//如果两个变量互斥建议创建枚举
//做会员用结构体存储在文件里
//焚诀：将每个退出都放在结构体变量中，不同结构体变量改变就代表不同结构体退出，与宏作用一样，且资源还可以统一释放


pthread_t  thread_round1;
pthread_t  thread_record_video;

void *child_ent0(void * a)
{
     int event_fd=open("/dev/input/event0",O_RDWR);
    if(-1==event_fd)
    {
        perror("open event_fd falied");
        exit(0);
    }
     struct input_event * ev0=calloc(64,sizeof(struct input_event));
     for(;;)
        {  
            //关机检测
            if(SET_SHUTDOWN&&set_flgs==1) 
            {
            printf("退出触摸\n");
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
    printf("退出摄像头\n");
    pthread_exit(NULL);

}

//传入buffer
void *child_Realtime_buf(void * a)
    {

    //创建两个缓冲，写完第一个给lcd读，同时写第二个，lcd屏读完第一个读第二个，
     read_buf=(unsigned int *)
    calloc(1,IMAGE_HEIGHT *IMAGE_WIDTH*sizeof(unsigned int));
       write_buf=(char *)
    calloc(1,IMAGE_HEIGHT *IMAGE_WIDTH*sizeof(char)*2);
    // printf("wirte_size=%d\n",IMAGE_HEIGHT *IMAGE_WIDTH*sizeof(unsigned int));

    for(;;)
    {  

          pthread_mutex_lock(&mutex_reading_flgs);
            if(round_flgs==1&&record_video_pth==0)
            {
                printf("识别到录制开启\n");
                sem_post(&sem_record_video_pth);
                memcpy(write_buf,p_buffer[read_index],IMAGE_WIDTH*IMAGE_HEIGHT*2);        
                 sem_wait(&sem_record_video_pth);
            
            }  
            else
            {
                memcpy(write_buf,p_buffer[read_index],IMAGE_WIDTH*IMAGE_HEIGHT*2);        
            }
          //此时write_buf已经有图片数据
          pthread_mutex_unlock(&mutex_reading_flgs);

            //转换格式,将图片塞进read_buf里
         pthread_mutex_lock(&mutex_wr_buf_mp);
          yuv2_rgb((unsigned int *) read_buf,(char *)write_buf);  //将w写到r，r供用户读

          if(round_pth==1&&round_flgs==1)//显示圆圈
        {
            round1();
            pthread_mutex_lock(&mutex_round_flgs);//上锁防止冲突
            round_pth=0;
            pthread_mutex_unlock( &mutex_round_flgs);//解锁
        }      

        if(line_pth==1)//显示辅助线 
        {
            Line_Car( read_buf);
        }
            pthread_mutex_unlock(&mutex_wr_buf_mp);

        if((EIXT_FILM|| REALTIME_RETURN)&&video_flgs==1)
        {
            printf("退出传输buffer\n");
            sem_post(&sem_Realtime_video);
            if(round_flgs==1)
            {
                //等待圆圈线程关闭
                pthread_join(thread_round1,NULL);

            }
            pthread_exit(NULL);
        }
            pthread_mutex_unlock(&mutex_wr_buf_mp);
            sem_post(&sem_Realtime_video);
    }  
}


//显示rgb
void * child_Realtime_video(void * a)
{
    for(;;)
    {       
    //等待处理过的rgb
        sem_wait(&sem_Realtime_video);
        
        pthread_mutex_lock(&mutex_wr_buf_mp);
        //若录制开启且读完一帧
    
        for(int i=0;i<480;i++)
        {
            memcpy(mp+i*WIDTH+80,read_buf+i*IMAGE_WIDTH,IMAGE_WIDTH*4);
        }     

        pthread_mutex_unlock(&mutex_wr_buf_mp);

        if((EIXT_FILM|| REALTIME_RETURN)&&video_flgs==1)
        {
            printf("退出摄像，这是显示画面模块\n");
            pthread_exit(NULL);
        }
    } 
}


//辅助线
void * lcd_line(void * a)
{
   for(;;)
    {       
        //当辅助线重复按表示关闭
        if(GUIDELINE&&video_flgs==1&&line_pth==1)
        {
            en0_clear();
            lcd_line_exit();
        }
        if((EIXT_FILM|| REALTIME_RETURN)&&video_flgs==1&&line_pth==1)   
        {
            line_pth=0;
            lcd_line_exit();
        }
    }
          
}

//line_pth=1为开
void  lcd_line_exit()
{
    printf("退出划线\n");
    pthread_exit(NULL);
}



//显示录制标志
void * lcd_round1(void * a)
{
    printf("开启录制\n");
    for(;;)
    {
            //再次点击关闭
        if(REALTIME_RECOE&&video_flgs==1&&round_flgs==1)   
        {
            sem_wait(&sem_record_video_flgs);
            en0_clear();
            pthread_mutex_lock(&mutex_round_pth);
            round_flgs=0;
            pthread_mutex_unlock(&mutex_round_pth);
            lcd_round1_exit();  
        } 
            //关闭整个摄像机
         if((EIXT_FILM|| REALTIME_RETURN)&&video_flgs==1&&round_flgs==1)   
        {
            lcd_round1_exit();
        }
        pthread_mutex_lock(&mutex_round_flgs);//上锁防止冲突
        round_pth=1;   //开启圆点
        pthread_mutex_unlock( &mutex_round_flgs);//解锁
        usleep(1000*1000);
    }
}
    void  lcd_round1_exit()
{
   
    printf("退出录制！\n");
    pthread_exit(NULL); 
} 




//进入摄像后的按键
void * Realtime_video_key(void * a)
{
    printf("进入key\n");
    pthread_t  thread_line;

    for(;;)
    {
        //点击退出按键
        if((EIXT_FILM|| REALTIME_RETURN)&&video_flgs==1)   
        {
            printf("退出key\n");
            if( line_pth==1)
            {  
                pthread_join(thread_line, NULL);
                line_pth=0;
            }
            // if( round_pth1==1)
            // {
            //     pthread_join(thread_round1, NULL);
            //     round_pth1=0;
            // }

                video_flgs==0;
                pthread_exit(NULL);         
        }
        //点击录像按键
        else if(REALTIME_RECOE &&video_flgs==1 )
        {
            pthread_mutex_lock(&mutex_round_pth);
            if(round_flgs==0)
            {
                en0_clear();
                round_flgs=1;
                printf("进入录像\n");
                //开始录像
            pthread_create(&thread_record_video, NULL,Record_Video,NULL);
                //显示录制标志
            pthread_create(&thread_round1, NULL,lcd_round1,NULL);
            }
            pthread_mutex_unlock(&mutex_round_pth);
            // else if(round_pth1==1)
            // {
            //     printf("222\n");
            //     pthread_join(thread_round1,NULL);
            //          round_pth=0;
            // }
        }

        else if(GUIDELINE &&video_flgs==1)
        {
            if(line_pth==0)
            {
                en0_clear();
                printf("进入辅助线\n");
                pthread_create(&thread_line, NULL,lcd_line,NULL);
                line_pth=1;
            }
            else if(line_pth==1)
            {
                pthread_join(thread_line,NULL);
                line_pth=0;
            }
        }   
        else if(REALTIME_IMC &&video_flgs==1 )
        {
             en0_clear();
              open_mic();
        }
    } 

}

void *Record_Video(void *a)
{
    printf("开始录像\n");
        struct stat statbuf;
//创建保存录像的文件夹，只有一个
    if(-1== stat(DIR_SAVE_VIDEO_PATH,&statbuf))
    {
        mkdir(DIR_SAVE_VIDEO_PATH,0777);
    }
 
     char * record_video_buf=( char *)calloc(1,IMAGE_HEIGHT*IMAGE_WIDTH*2);
    if(record_video_buf==NULL)
        {
            printf("===================堆空间创建失败===============\n");
                return NULL;                
        }
        //处理成图片
     char          filename_path [70]      = {0};
     char          filetime[50]            = {0};
    
    //找到路径
     memcpy(filename_path,DIR_SAVE_VIDEO_PATH,strlen(DIR_SAVE_VIDEO_PATH));
     strcat(filename_path,"/");
     //获得时间戳
     File_time(filetime);
    //获得文件夹名字
    sprintf(filename_path+strlen(filename_path),"recoreder-%s-Car/",filetime);
    //创建文件夹
    mkdir(filename_path,0777);
    
    char  *filename =calloc(1,150*sizeof(char));
    for(int i=0;;i++)
    {
        printf("开始获取一帧图片\n");
        sem_wait(&sem_record_video_pth);
        record_video_pth=1;
        memcpy(record_video_buf,p_buffer[read_index],IMAGE_WIDTH*IMAGE_HEIGHT*2);        
        sem_post(&sem_record_video_pth);
       
        //获取时间戳,要么添加成结构体要么不加时间戳
        //文件路径加名字
        sprintf(filename,"%s-car%d.jpg",filename_path,i);   
        //转成图片 （yuyv数据转成图片）
        write_JPEG_file (filename, 80,record_video_buf);
        printf("录像获得一帧\n");
        record_video_pth=0;
            //重复点击
        if(REALTIME_RECOE&&video_flgs==1&&round_flgs==1)   
        {
            sem_post(&sem_record_video_flgs);
            free(filename);
           pthread_exit(NULL); 
        }
    }


        

}