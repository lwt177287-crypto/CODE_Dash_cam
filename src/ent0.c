#include "video_car.h"
int temp_x=-1;
int temp_y=-1;
//触摸屏代码
void Abs_Cat( int event_fd, struct input_event * ev0)
{
    
    int len;
    len= read(event_fd,ev0,sizeof(*ev0)*64); //可以设置宏或者传参
//    //发生错误或者缺失
//     if(len<=0 || len % sizeof(struct input_event)!=0 )
//     {
//         fprintf(stderr,"input event size error\n");
//         return ;
//     }
    int num=len/sizeof(struct input_event);

    for(int i=0;i<num;i++)
    { 
        if(ev0[i].type==EV_ABS)
        {
            if(ev0[i].code== ABS_X)
            {
                temp_x=ev0[i].value*800/1024;
            }
            if(ev0[i].code== ABS_Y)
            {
                temp_y=ev0[i].value*480/600;
            } 
        }
        if( ev0[i].type==EV_SYN&&ev0[i].code==SYN_REPORT)
        {
            if(temp_x!=-1&&temp_y!=-1)
            { 
                pthread_mutex_lock(&mutex_en0_ABS_flgs);
                en0_x = temp_x;
                en0_y = temp_y;
                pthread_cond_broadcast(&sem_en0_ABS);
                pthread_mutex_unlock(&mutex_en0_ABS_flgs);
                printf("x=%d,y=%d\n",en0_x,en0_y);
                
            }
            temp_x=-1;
            temp_y=-1;
        }
    }
}


void en0_clear()
{
     pthread_mutex_lock(&mutex_en0_ABS_flgs);
        en0_x=0;
        en0_y=0;
        pthread_mutex_unlock( &mutex_en0_ABS_flgs);
}