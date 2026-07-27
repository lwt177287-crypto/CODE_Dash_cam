#include "video_car_exe.h"
struct buffer *         buffers         = NULL;

//打开摄像头全流程
void Open_Video()
{
            mp=lcd_fp;
        //打开摄像头
 open_device ();
//初始化
 init_device ();
//入队
start_capturing();

     mainloop ();

        // stop_capturing ();

        // uninit_device ();

        // close_device ();

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
        buffers[n_buffers].start =
                                    mmap (
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
void yuv2_rgb(char* buf,  unsigned int *mp)
{
        int j,i;
        unsigned int lcdbuf[480*640]={0};
        for(i=0,j=0;j<480*640;i+=4,j+=2)
        {
                lcdbuf[j]= yuyv_to_rgb_pix(buf[i],buf[i+1],buf[i+3]);
                lcdbuf[j+2]=yuyv_to_rgb_pix(buf[i+2],buf[i+1],buf[i+3]);
        }
        for(i=0;i<480;i++)
        {
                for(j=0;j<640;j++)
                {
                        mp[j+i*WIDTH+80]=lcdbuf[j+i*640];
                }
        }

}


    //处理图像
static void
mainloop                        ()
{
	struct v4l2_buffer buf;
	unsigned int i;

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
        
        //循环 出队->处理->入队
        for (i = 0; i < 4; ++i) 
        {
            //读取图像帧并进行颜色转码，刷新到LCD
            CLEAR (buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;

            //缓冲区出队
            ioctl (fd, VIDIOC_DQBUF, &buf);

            //处理图像帧并显示到LCD
            yuv2_rgb(buffers[i].start,mp);
 
            //缓冲区入队
            ioctl (fd, VIDIOC_QBUF, &buf);
        }
        
    }
       
}
