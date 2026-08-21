#include "LVGL_key.h"
#include "video_car.h"

//逻辑是先进入主界面，为每个界面分配画布对象，并创建对应的按钮函数，里面有功能按钮，主界面点击某个按钮，
//回调函数是将活动画布设置为对应的画布对象，系统会根据你写的对应的按钮函数来刷新按钮

//回调函数，只改按钮状态


void NULL_BTN(lv_event_t *event)
{
    struct str_btn* info;
    if(event != NULL)
    {
        struct str_btn* info;
        info = lv_event_get_user_data(event);

        if(info != NULL)
        {
            info->btn=1;
        }
    }
}


lv_obj_t * Btn_Create(lv_obj_t *page,int x,int y,int w,int h, void Fun(lv_event_t *),struct str_btn* s_btn)
{
    
    //按钮
   lv_obj_t *btn = lv_btn_create(page);
  
    //隐藏
    // lv_obj_remove_style_all(btn);
    lv_obj_set_width(btn, w);      
    lv_obj_set_height(btn, h);
    lv_obj_set_pos(btn,x,y);

    if(Fun!=NULL)
  {  lv_obj_add_event_cb(btn,
                         Fun,
                         LV_EVENT_CLICKED,
                         s_btn);
  }
  return btn;
}



//lvgl按键线程
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


    //事件
    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);



    
    //进入主界面按钮
    Main_Page_Create();
    
    //摄像画面
    Realtime_Video_Page_Create();
    //文件夹画面
    // Main_Dir_Page_Create();
    // //设置画面
    settings_Page_Create();
    // //音乐画面
    // Main_Srceenshot_Page_Create();
    // //记录仪保存画面
    // Main_Record_Video_Page_Create();


    lv_scr_load(main_page);




    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) 
    {
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


