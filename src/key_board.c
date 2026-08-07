
#include "video_car.h"


void *child_keyboard(void * a)
{
     //获取被键盘覆盖的区域
    //这里键盘大小为800*240
    printf("进入键盘线程\n");
    unsigned int * keyboard_mp=(unsigned int *)calloc(240*800,sizeof(unsigned int));
    char *p=s_keyboard.keyboard_out;
    p+=strlen(s_keyboard.keyboard_out);
    memcpy(
            keyboard_mp,
            mp+(HEIGHT-240)*WIDTH,
            240*WIDTH*sizeof(unsigned int)
            );
    read_JPEG_file (KEYBOARD_IMAGE, mp+(HEIGHT-240)*WIDTH);

    for(;;)
    {
        if(strlen(s_keyboard.keyboard_out)==99)
        {   
            printf("键盘输出已满!!\n");
        }
         p = Key_Board(keyboard_mp,p);
            usleep(10000);
        if((KEYBOARD_CLOSE||KEYBOARD_RETURN)&& s_keyboard.Keyboard_flgs==1)//关闭
        {   
            // printf("eno_x=%d,eno_y=%d\n",en0_x,en0_y);
            printf("%d\n",KEYBOARD_CLOSE);
            printf("%d\n",KEYBOARD_RETURN);
            printf("退出键盘\n");
            memcpy(mp+(HEIGHT-240)*WIDTH,keyboard_mp,240*WIDTH*sizeof(unsigned int));
            free(keyboard_mp);
            s_keyboard.Keyboard_flgs=0;
            pthread_exit(NULL);
        
        }
    }
}

char* Key_Board(unsigned int * keyboard_mp,char *p)
{
    if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_1&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='1';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_2&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='2';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_3&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='3';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_4&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='4';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_5&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='5';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_6&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='6';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_7&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='7';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_8&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='8';
        p++;    
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_9&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='9';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_0&&strlen(s_keyboard.keyboard_out)<99)
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='0';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_LEFT)//左移
    {
        en0_clear();
        if(p>s_keyboard.keyboard_out)
        {
            p--;
            printf("p=%s\n",p);
        }
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_RIGHT)//右移
    {
        en0_clear();
        if(p<s_keyboard.keyboard_out+strlen(s_keyboard.keyboard_out))
        {
            p++;
            printf("p=%s\n",p);
        }
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_END)//移到末尾
    {
        en0_clear();
        p=s_keyboard.keyboard_out;
        p+=strlen(s_keyboard.keyboard_out);
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_REMOVE)//删除
    {
        en0_clear();
        if(p!=s_keyboard.keyboard_out)
        {
            printf("%p\n",s_keyboard.keyboard_out);
            printf("%p\n",p);
            memmove(p-1,p,strlen(p)+1);
            p--;
        printf("%s\n",s_keyboard.keyboard_out);
        }
    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_YES)//确定
    {
        en0_clear();

    }
    else if(s_keyboard.Keyboard_flgs==1&&KEYBOARD_DECIMAL_POINT&&strlen(s_keyboard.keyboard_out)<99)//小数点
    {
        en0_clear();
        memmove(p+1,p,strlen(p)+1);
        *p='.';
        p++;
        printf("%s\n",s_keyboard.keyboard_out);
    }
   
    
    return p;
}