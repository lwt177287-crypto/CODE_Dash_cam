#include "video_car_exe.h"

//触摸屏代码
void Abs_Cat( int event_fd, struct input_event * ev0)
{


        int a= read(event_fd,ev0,sizeof(struct input_event));
        if(ev0->type==EV_ABS)
        {
            if(ev0->code== ABS_X)
            {
                 pthread_mutex_lock(&mutex_en0_ABS_flgs);
                en0_x=ev0->value*800/1024;
                pthread_mutex_unlock( &mutex_en0_ABS_flgs);
            }
            if(ev0->code== ABS_Y)
            {
                 pthread_mutex_lock(&mutex_en0_ABS_flgs);
                en0_y=ev0->value*480/600;
                pthread_mutex_unlock( &mutex_en0_ABS_flgs);
                printf("x=%d,y=%d\n",en0_x,en0_y);
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