    //转成jpg图片,想写覆盖的尝试一下链表,时间还有问题。。(代码没问题，环境不适配)
            
                // if(num<200)
                // {
                //     File_time(filetime);
                //     printf("%s\n",filetime);
                //     sprintf(filename,"%s-car%d.jpg",filetime,num++);   
                //         car_video[num]=filename;
                //     write_JPEG_file (filename, 80,(char *)buffers[i].start);
                // }
void Album(char *  buf)
{
    typedef struct LIST
    {
       struct LIST* next;
       struct LIST* prev;
       char * miage_name[20];//文件名字
    }* Alb;
    //初始化
    //双向循环
    //头指针转移（删除），增加
    //生成图像
}