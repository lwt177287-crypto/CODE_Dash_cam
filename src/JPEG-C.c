#include <stdio.h>
//将数据转成图片
/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */
#include <stdlib.h>
#include "jpeglib.h"
#include "video_car_exe.h"

/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */

#include <setjmp.h>

#define PATHNAME ""
 unsigned char * image_buffer;	/* Points to large array of R,G,B-order data *///typedef unsigned char 
 int image_height;	/* Number of rows in image */
 int image_width;		/* Number of columns in image */
char * filename;


/*
 * Sample routine for JPEG compression.  We assume that the target file name
 * and a compression quality factor are passed in.
 */

GLOBAL(void)
write_JPEG_file (char * filename, int quality,  char *  buf)
{
    /*该结构体包含JPEG压缩参数以及指向工作空间的指针（工作空间由JPEG库按需分配）。
    可以同时存在多个此类结构体，代表多个压缩/解压缩过程。我们将任何一个结构体及其
    相关联的工作数据称为一个“JPEG对象”*/
//定义JPEG对象
  struct jpeg_compress_struct cinfo;
  //结构体表示JPEG错误处理程序

  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file *///目标文件的句柄
  unsigned char * row_pointer[1];	/* pointer to unsigned char row[s] *///typedef unsigned char *;指针数组
  int row_stride;		/* physical row width in image buffer */

  /* Step 1: 第一步：分配并初始化*/

  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);


  //第二步：打开文件
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: 设置压缩参数 */
 image_height=IMAGE_HEIGHT;
  image_width=IMAGE_WIDTH;
  cinfo.image_width = image_width; 	/* image width and height, in pixels */
  cinfo.image_height = image_height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

  jpeg_set_defaults(&cinfo);
  
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: 开始压缩 */

  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5:循环传入数据 */
 //buf
  image_buffer=(unsigned char*)calloc(1,image_width*image_height*3);



    //被合成一个像素点，超级拆解
    // pix_point = (R<<16)|(G<<8)|B;//把RGB合成一个像素点 
  
    int j,i,x;
    int argb1,argb2;
      for( x=0,i=0,j=0;j<480*640;i+=4,j+=2,x+=6)
        {
               argb1= yuyv_to_rgb_pix(buf[i],buf[i+1],buf[i+3]);
               argb2=yuyv_to_rgb_pix(buf[i+2],buf[i+1],buf[i+3]);
              image_buffer[x]=argb1>>16;
              image_buffer[x+1]=argb1>>8;
              image_buffer[x+2]=argb1;    
              image_buffer[x+3]=argb2>>16;
              image_buffer[x+4]=argb2>>8;
              image_buffer[x+5]=argb2;
              }
  


    /* cinfo.next_scanline循环计数器
   */
  row_stride = image_width * 3;	/* unsigned chars per row in image_buffer *///像素点

  while (cinfo.next_scanline < cinfo.image_height) {    //行未到底
//看来cinfo.next_scanline自动++
    row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];  //指针数组，获取某一行首地址
  
   
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);    //写入当前行的数据到cinfo

  }

  /* Step 6: Finish compression */
//压缩
  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
   free(image_buffer);
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
}


  void File_time(char * filetime)
  {
      setenv("TZ", "CST-8", 1); 
    tzset();

  struct tm *tm;
  time_t tim=time(NULL);
  tm=localtime(&tim);
  sprintf(filetime,"%d-%d-%d-%.2d:%.2d:%.2d-",tm->tm_year+1900,(tm->tm_mon)+1,
                    tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);

  }