#include "../lv_port_linux-release-v8.3/lvgl/lvgl.h"
#include "../lv_port_linux-release-v8.3/lvgl/demos/lv_demos.h"
#include "../lv_port_linux-release-v8.3/lv_drivers/display/fbdev.h"
#include "../lv_port_linux-release-v8.3/lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "video_car.h"


#define DISP_BUF_SIZE (128 * 1024)

void * child_lvgl_key(void *a)
{

    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    //界面缓冲区尺寸，可以多缓冲
    static lv_color_t buf[DISP_BUF_SIZE*10];
    /*Initialize a descriptor for the buffer*/
    //主画布
    static lv_disp_draw_buf_t disp_buf;
    //初始化
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE*10);

    /*Initialize and register a display driver*/
    //显示驱动程序
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);


    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;


    
    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    //设置光标
    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon)
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/


    lv_obj_t * img = lv_img_create(lv_scr_act());

if(s_v_c_main.Interface==1)
{
 
    // lv_img_set_src(img, "S:Main.jpg");

}

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
                usleep(5000);

    }

    return 0;
}

// /*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
// uint32_t custom_tick_get(void)
// {
//     static uint64_t start_ms = 0;
//     if(start_ms == 0) {
//         struct timeval tv_start;
//         gettimeofday(&tv_start, NULL);
//         start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
//     }

//     struct timeval tv_now;
//     gettimeofday(&tv_now, NULL);
//     uint64_t now_ms;
//     now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

//     uint32_t time_ms = now_ms - start_ms;
//     return time_ms;
// }


