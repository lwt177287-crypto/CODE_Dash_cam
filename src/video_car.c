#include "video_car.h"


static void init_device(void);
static void start_capturing(void);
static void mainloop(void);
static void stop_capturing ();

static void uninit_device ();

static void close_device ();




#define CLEAR(x) memset (&(x), 0, sizeof (x))
static char *           dev_name        = "/dev/video7";
static int              fd              = -1;
static unsigned int     n_buffers       = 0;
struct buffer *         buffers;

//行程记录仪
//打开摄像头全流程
void Open_Video()
{
    printf("进入Open_Video\n");
    //重置lcd屏位置  
    mp=lcd_fp;
    //打开摄像头
    open_device ();
    //初始化
    init_device ();
    //入队
    start_capturing();

    mainloop ();



    //退出处理


// exit (EXIT_SUCCESS);


}




//打开设备
int open_device(void)
{
    //打开摄像头设备
    fd = open (dev_name, O_RDWR, 0);

    //错误处理
    if (-1 == fd) 
    {
        printf("open /dev/video7 error\r\n");
        return -1; 
    }
}

//初始化摄像头
void init_device(void)
{
    //初始化采集格式  
    struct v4l2_format fmt;
	
    //清空结构体
    CLEAR (fmt);

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = IMAGE_WIDTH; 
    fmt.fmt.pix.height      = IMAGE_HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    ioctl(fd, VIDIOC_S_FMT, &fmt);     

    //申请缓冲区
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    ioctl (fd, VIDIOC_REQBUFS, &req);

    //申请4块结构体大小的堆内存，用于记录待申请的缓冲区信息
    buffers = calloc (req.count, sizeof (*buffers));

    //循环申请4块用于存储图像像素的缓冲区，并记录每块缓冲区的信息
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        struct v4l2_buffer buf;

        CLEAR (buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        ioctl (fd, VIDIOC_QUERYBUF, &buf);


        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = mmap (
                                            NULL /* start anywhere */,
                                            buf.length,
                                            PROT_READ | PROT_WRITE /* required */,
                                            MAP_SHARED /* recommended */,
                                            fd, buf.m.offset);
    }
    
}

//开始采集
static void
start_capturing                 (void)
{
    unsigned int i;
    enum v4l2_buf_type type;

    //缓冲区入队
    for (i = 0; i < n_buffers; ++i) 
    {
        struct v4l2_buffer buf;

        CLEAR (buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        ioctl (fd, VIDIOC_QBUF, &buf);
    }
		
    //发送开始采集的请求码
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl (fd, VIDIOC_STREAMON, &type);
}

int yuyv_to_rgb_pix(int Y,int Cb,int Cr)
{
	unsigned int pix_point = 0;//存储一个像素点的数据
	unsigned int R,G,B;      //用来存储RGB颜色分量

	//通过V4L2官方帮助文档中的公式进行计算
	R = (255/219)*(Y - 16) + 1.402 * (127/112)*(Cr - 128);
	G = (255/219)*(Y - 16) - 0.344 * (127/112)*(Cb - 128) - 0.714*(127/112)*(Cr - 128);
	B = (255/219)*(Y - 16) + 1.772 * (127/112)*(Cb - 128);

	//防止越界  RGB分别为占用一个字节 0~255，所以需要进行越界处理
	if(R > 255) R = 255;
	if(G > 255) G = 255;
	if(B > 255) B = 255;
	if(R < 0) R = 0;
	if(G < 0) G = 0;
	if(B < 0) B = 0;

	pix_point = (R<<16)|(G<<8)|B;//把RGB合成一个像素点 
	return pix_point;
}



void yuv2_rgb(unsigned int* read_buf,  char *write_buf)
{
        int j,i;
        for(i=0,j=0;j<480*640;i+=4,j+=2)
        {
            //合成rgb
            //read多申请了一半的内存
            read_buf[j]= yuyv_to_rgb_pix(write_buf[i],write_buf[i+1],write_buf[i+3]);
            read_buf[j+1]=yuyv_to_rgb_pix(write_buf[i+2],write_buf[i+1],write_buf[i+3]);
        }


        // for(i=0;i<480;i++)
        // {
        //         for(j=0;j<640;j++)
        //         {
        //                buffer[j+i*WIDTH+80]=read_buf[j+i*640];
        //         }

        // }       
      

}
    //处理图像
static void
mainloop()
{
	struct v4l2_buffer buf;

      
    for (;;) 
    {
        //定义了一个文件描述符的集合
        fd_set fds;             
        struct timeval tv; 
        int r;

        FD_ZERO (&fds);         //清空集合 
        FD_SET (fd, &fds);      //添加句柄 

        /* Timeout. */
        //设置超时时间
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        select (fd + 1, &fds, NULL, NULL, &tv);
       
        char *car_video[200];           //////这个还没清空，要申请堆空间
       
        //循环 出队->处理->入队
        for (buf_i = 0; buf_i  < 4;  ++buf_i ) 
        {
            //读取图像帧并进行颜色转码，刷新到LCD
            CLEAR (buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;

            //缓冲区出队，尝试看一下摄像有没有数据，用标志位即可
            //且要利用缓冲区，改
            ioctl (fd, VIDIOC_DQBUF, &buf);//采集的数据已经放入buffer里
          //每次都保存到
            pthread_mutex_lock(&mutex_reading_flgs);
            p_buffer[buf_i]=buffers[buf.index].start;
             read_index=buf_i;
            s_recorder.new_Camera_frame_flgs=1;
            //拷贝一份给推流服务器（避免 QBUF 后这块缓冲被摄像头覆盖）
            memcpy(g_stream_yuyv, buffers[buf.index].start, IMAGE_WIDTH*IMAGE_HEIGHT*2);
          pthread_mutex_unlock(&mutex_reading_flgs);

         
               //点击关机
            if(s_settings.shutdown_flgs==1)
            {
                //等待行车记录仪备份
                    sem_wait(&sem_recorder_complete);

                    stop_capturing ();
                
                   uninit_device ();

                     close_device ();

                        return ;
            }   
            //缓冲区入队
             ioctl (fd, VIDIOC_QBUF, &buf);

    }
  

    }
    
}













static void
stop_capturing                  (void)
{
        enum v4l2_buf_type type;
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		ioctl (fd, VIDIOC_STREAMOFF, &type);

}



static void
uninit_device                   (void)
{
        unsigned int i;

		for (i = 0; i < n_buffers; ++i)
		munmap (buffers[i].start, buffers[i].length);
        	free (buffers);
}


static void
close_device                    (void)
{
              close (fd);
        fd = -1;
        // exit (EXIT_SUCCESS);
}





//在另一个进程完成，转成jpg图片,想写覆盖的尝试一下链表,时间还有问题。。(代码没问题，环境不适配)
        // if(num<200)
        // {
        //     //获取时间
        //     File_time(filetime);
        //     sprintf(filename,"%s-car%d.jpg",filetime,num++);   
        //     car_video[num]=filename;
        //     write_JPEG_file (filename, 80,(char *)buffers[buf_i ].start);
        // }