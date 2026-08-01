
#include "video_car.h"


void *child_keyboard(void * a)
{

             //获取被键盘覆盖的区域
    //这里键盘大小为800*240
    printf("进入键盘线程\n");
    unsigned int * keyboard_mp=(unsigned int *)calloc(240*800,sizeof(unsigned int));
    char *p=keyboard_out;
        p+=strlen(keyboard_out);
    memcpy(
        keyboard_mp,
        mp+(HEIGHT-240)*WIDTH,
        240*WIDTH*sizeof(unsigned int)
            );
    read_JPEG_file (KEYBOARD_IMAGE, mp+(HEIGHT-240)*WIDTH);

    for(;;)
    {
         p = Key_Board(keyboard_mp,p);
        if((KEYBOARD_CLOSE||KEYBOARD_RETURN)&& Keyboard_flgs==1 )//关闭
        {   
            printf("退出键盘\n");
            memcpy(mp+(HEIGHT-240)*WIDTH,keyboard_mp,240*WIDTH*sizeof(unsigned int));
            free(keyboard_mp);
            Keyboard_return=1;
            pthread_exit(NULL);
        }
    }
}


char* Key_Board(unsigned int * keyboard_mp,char *p)
{
   

    //显示键盘
    //   if(strlen(keyboard_out)==99)
    // {
    //     printf("键盘输出已满\n");
    // }
    if(Keyboard_flgs==1&&KEYBOARD_1)
    {
        en0_clear();
        strcat(keyboard_out,"1");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_2)
    {
        en0_clear();
        strcat(keyboard_out,"2");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_3)
    {
        en0_clear();
        strcat(keyboard_out,"3");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_4)
    {
        en0_clear();
        strcat(keyboard_out,"4");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_5)
    {
        en0_clear();
        strcat(keyboard_out,"5");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_6)
    {
        en0_clear();
        strcat(keyboard_out,"6");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_7)
    {
        en0_clear();
        strcat(keyboard_out,"7");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_8)
    {
        en0_clear();
        strcat(keyboard_out,"8");
        p++;    
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_9)
    {
        en0_clear();
        strcat(keyboard_out,"9");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_0)
    {
        en0_clear();
        strcat(keyboard_out,"0");
        p++;
        printf("%s\n",keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_LEFT)//左移
    {
        en0_clear();
        if(p>keyboard_out)
        {
            p--;
            printf("p=%s\n",p);
        }
    }
    else if(Keyboard_flgs==1&&KEYBOARD_RIGHT)//右移
    {
        en0_clear();
        if(p<keyboard_out+strlen(keyboard_out))
        {
            p++;
            printf("p=%s\n",p);
        }
    }
    else if(Keyboard_flgs==1&&KEYBOARD_END)//移到末尾
    {
        en0_clear();
        p=keyboard_out;
        p+=strlen(keyboard_out);
    }
    else if(Keyboard_flgs==1&&KEYBOARD_REMOVE)//删除
    {
        en0_clear();
        if(p!=keyboard_out)
        {
            printf("%p\n",keyboard_out);
            printf("%p\n",p);
            p--;
            *p='\0';
        printf("%s\n",keyboard_out);
        }

    }
    else if(Keyboard_flgs==1&&KEYBOARD_YES)//确定
    {
        en0_clear();

    }
    else if(Keyboard_flgs==1&&KEYBOARD_DECIMAL_POINT)//小数点
    {
        en0_clear();
        strcat(keyboard_out,".");
        p++;
        printf("%s\n",keyboard_out);
    }
    return p;
  

}