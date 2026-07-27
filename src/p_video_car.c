#include "video_car_exe.h"
struct buffer *         buffers         = NULL;

//打开摄像头全流程
void Open_Video()
{
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
int open_device                     (void)
{
       

        fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd) {
              perror("open fd falied");
              exit(1);
            return -1;
            }
            return 0;
}

static void
init_device                     (void)
{
       
        struct v4l2_format fmt;
	unsigned int min;       //删

        CLEAR (fmt);

        fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = 640; 
        fmt.fmt.pix.height      = 480;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;//删

        ioctl (fd, VIDIOC_REQBUFS, &fmt);//改后               
        struct v4l2_requestbuffers req;
        // ioctl (fd, VIDIOC_REQBUFS, &req);               ///？
        CLEAR (req);

        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_USERPTR;

        ioctl (fd, VIDIOC_REQBUFS, &req);

  buffers = calloc (req.count, sizeof (*buffers));//分配内存，本身大小
        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR (buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;
                        //映射摄像头
                ioctl (fd, VIDIOC_QUERYBUF, &buf);

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap (NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd,
                               buf.m.offset);

              
        }
	
}

static void
start_capturing                 (void)
{
        unsigned int i;
        enum v4l2_buf_type type;

		for (i = 0; i < n_buffers; ++i) {
            		struct v4l2_buffer buf;

        		CLEAR (buf);

        		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        		buf.memory      = V4L2_MEMORY_MMAP;
        		buf.index       = i;

        		 ioctl (fd, VIDIOC_QBUF, &buf);
                    	
		}
		
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		ioctl (fd, VIDIOC_STREAMON, &type);

}
unsigned int yuyv_to_rgb_pix(char Y,char Cb,char Cr)
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
        unsigned int *lcdbuf=(unsigned int *)calloc(1,480*640);
        for(i=0,j=0;j<480*640;i+=4,j+=2)
        {
                lcdbuf[j]= yuyv_to_rgb_pix(buf[i],buf[i+1],buf[i+3]);
                lcdbuf[j+1]=yuyv_to_rgb_pix(buf[i+2],buf[i+1],buf[i+3]);
        }
        for(i=0;i<480;i++)
        {
                for(j=0;j<640;j++)
                {
                        mp[j+i*WIDTH+80]=lcdbuf[j+i*640];
                }
        }

}


static void
mainloop                        (void)
{
	unsigned int count;

        count = 100;

                for (;;) {
                        fd_set fds;
                        struct timeval tv;
                        int r;

                        FD_ZERO (&fds);
                        FD_SET (fd, &fds);

                        /* Timeout. */
                        tv.tv_sec = 2;
                        tv.tv_usec = 0;

                       select (fd + 1, &fds, NULL, NULL, &tv);
		        	
                    	 struct v4l2_buffer buf;
	                unsigned int i;
                           //添加循环
                        for(i=0;i<4;++i)
                        {
                                CLEAR (buf);

                         buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                         buf.memory = V4L2_MEMORY_MMAP;

                                    //出队
                        ioctl (fd, VIDIOC_DQBUF, &buf);

                           //处理图像，转换
                        yuv2_rgb( buffers[i].start,  mp);

                          //将yuv转换成rgb后打开摄像头

                          //入队
                        ioctl (fd, VIDIOC_QBUF, &buf);
                }	
			/* EAGAIN - continue select loop. */
                }
}
