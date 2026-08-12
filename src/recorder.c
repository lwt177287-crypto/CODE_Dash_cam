#include "video_car.h"
//记录仪

 char        *              recorder_read_buf;
int xunhuan;

//行车记录仪应该每200帧保存一次，具体为满200帧后另开线程处理转换成图片，记录仪线程清零且重新创建文件夹，关机前应自动保存
//获得帧
void *child_recorder(void * a )
{
    struct stat statbuf;
//创建行车记录仪的文件夹，只有一个，一直覆盖
    if(-1== stat(DIR_RECORDER_PATH,&statbuf))
    {
        mkdir(DIR_RECORDER_PATH,0777);
    }
 
//  int *  recorder_read=calloc(4,IMAGE_HEIGHT*IMAGE_WIDTH);
    recorder_read_buf=calloc(RECORDER_FRAME,IMAGE_HEIGHT*IMAGE_WIDTH*2);
    if(recorder_read_buf==NULL)
        {
            printf("===================堆空间创建失败===============\n");
                return NULL;                
        }
      
    for(;;)
    {
        for(recorder_frame=0;recorder_frame<RECORDER_FRAME;recorder_frame)
         {
            if(s_recorder.recorder_create==1) //打开记录仪
            {
                pthread_create(&thread_image_jpg, NULL,recorder_image_jpg,NULL);
                sem_wait(&sem_recorder_complete);
                s_recorder.recorder_create=0;
                 
            }
            // 如果点击关机，先保存再结束
            else if(s_settings.shutdown_flgs==1&&s_recorder.recorder_flgs==0)   //关机
            {
                printf("保存录像\n");
                  s_recorder.recorder_flgs=1;
                pthread_t  thread_image_jpg;        
              pthread_create(&thread_image_jpg, NULL,recorder_image_jpg,NULL);//触控，永远打开，除非系统退出，不需要exit
                pthread_join(thread_image_jpg,NULL);
                pthread_exit(NULL);
            }
             else if(s_recorder.new_Camera_frame_flgs==1)
             { 
                // printf("frame=%d\n",recorder_frame);
                //获取摄像头采集的yuyv
                pthread_mutex_lock(&mutex_reading_flgs);
                memcpy(recorder_read_buf+(recorder_frame*IMAGE_HEIGHT*IMAGE_WIDTH*2),
                p_buffer[read_index],IMAGE_HEIGHT*IMAGE_WIDTH*2);
                s_recorder.new_Camera_frame_flgs=0;    //拿取新帧完毕
                pthread_mutex_unlock(&mutex_reading_flgs);
                //用完再++，当前位置是旧数据
                recorder_frame++;
           }
        }
        
        xunhuan=1;

        //    free(recorder_read_buf);
        // char * recorder_image_buf=recorder_read_buf;
    //保存录像线程
       

    }
}



void * recorder_image_jpg(void *a)
{
    printf("正在保存记录仪\n");
    //初始化
    char * recorder_image_buf=recorder_read_buf;
    int stat_frame;
    int end_frame=0;
    //如果不是关机
    if(s_v_c_main.set_flgs!=1)
    {
        printf("重新分配内存\n");
        recorder_read_buf=calloc(RECORDER_FRAME,IMAGE_HEIGHT*IMAGE_WIDTH*2);
    }
    //判断堆内存创建是否成功
    if(recorder_read_buf==NULL)
    {
        printf("malloc fail\n");
        return NULL;
    }
    //当前位置的下一帧是否为空，若不为空则已被覆盖
    if(xunhuan==1)
    {
         stat_frame=recorder_frame;
         xunhuan=0;
    }   
    else 
    {
        stat_frame=0;
        end_frame=recorder_frame;
    }
       
    
    //已经把存记录仪yuyv数据的缓存更新地址了，旧地址可以随便访问了
    sem_post(&sem_recorder_complete);

     char         filename_path [70]      = {0};
     char          filetime[50]      = {0};
    
    //找到路径
     memcpy(filename_path,DIR_RECORDER_PATH,strlen(DIR_RECORDER_PATH));
     strcat(filename_path,"/");
     //获得时间戳
     File_time(filetime);
    //获得文件夹名字
    sprintf(filename_path+strlen(filename_path),"recoreder-%s-Car/",filetime);
    //创建文件夹
    mkdir(filename_path,0777);
    
    char  *filename =calloc(1,150*sizeof(char));
    printf("保存回放中...\n");
    for(int i=0;i<200;i++)
    {
        //获取时间戳,要么添加成结构体要么不加时间戳
        // File_time(filetime);
        sprintf(filename,"%s-car%d.jpg",filename_path,i);   
        printf("%d\n",i);   
        //文件路径加名字
        //转成图片
        write_JPEG_file (filename, 80,(recorder_image_buf)+(stat_frame%200)*IMAGE_HEIGHT*IMAGE_WIDTH*2);
        stat_frame++;
        if(end_frame==stat_frame&&end_frame!=0)
        {
        break;
        }
    }
   s_recorder.recorder_flgs=0;
    printf("完毕\n");
    free(recorder_image_buf);
    free(filename);
}


//    //创建文件夹,行车记录仪的文件夹
    // chdir("/root/shiyan/CODE/Car");
    // char * filetime=calloc(1,100);
    // filename=calloc(1,100);
    
    // File_time(filetime);
    // sprintf(filename,"%s-Car",filetime);
    // mkdir(filename,0777);
    // chdir(filename);