#include "LVGL_key.h"
#include "video_car.h"


//回调函数
 void Back_Main(lv_event_t *event)
{
    lv_scr_load(main_page);
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


 void Enter_Realtime_Video(lv_event_t *event)
{
    lv_scr_load(realtime_video_page);
        //圆形区域
    // lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);

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

void Enter_Main_Settings(lv_event_t *event)
{
    struct str_btn* info;
    lv_scr_load(main_settings_page);
    if(event != NULL)
    {
        struct str_btn* info;
        info = lv_event_get_user_data(event);

        if(info != NULL)
        {
            info->btn=1;
        }
    }
    printf("进入设置lvgl画面\n");
}


// static void Enter_Main_Srceenshot(lv_event_t *event)
// {
//     printf("进入音乐lvgl画面\n");
//     lv_scr_load(main_srceenshot_page);
// }

// static void Enter_Main_Dir(lv_event_t *event)
// {
//         printf("进入文件夹lvgl画面\n");
//         lv_scr_load(main_dir_page);
//     }

// static void Enter_Record_Video(lv_event_t *event)
// {
//     printf("进入行车记录仪lvgl画面\n");
//     lv_scr_load(main_record_video_page);
// }



//主界面按钮
void Main_Page_Create()
{
    
    main_page = lv_obj_create(NULL);   //创建一个新的screen


    //背景
    lv_obj_t *img = lv_img_create(main_page);
    lv_img_set_src(img,CAT_STAT_IMAGE);


    //摄像按钮
   Btn_Create(main_page,REALTIME_VIDEO_BTN_X,REALTIME_VIDEO_BTN_Y,
            REALTIME_VIDEO_BTN_W,REALTIME_VIDEO_BTN_H,
            Enter_Realtime_Video,&(s_main_btn[REALTIME_VIDEO_BTN]));
   
    //设置按钮
       Btn_Create(main_page,MAIN_SETTINGS_BTN_X,MAIN_SETTINGS_BTN_Y,
            MAIN_SETTINGS_BTN_W,MAIN_SETTINGS_BTN_H,
            Enter_Main_Settings,&(s_main_btn[SETTINGS_BTN]));

            //记录仪按钮
   Btn_Create(main_page,MAIN_RECORD_VIDEO_SAVE_BTN_X,MAIN_RECORD_VIDEO_SAVE_BTN_Y,
            MAIN_RECORD_VIDEO_SAVE_BTN_W,MAIN_RECORD_VIDEO_SAVE_BTN_H,
            NULL_BTN,&(s_main_btn[MAIN_REALTIME_RECOER_BTN]));

 

   //麦克风按钮
   Btn_Create(main_page,MAIN_MIC_BTN_X,MAIN_MIC_BTN_Y,
            MAIN_MIC_BTN_W,MAIN_MIC_BTN_H,
            NULL_BTN,&(s_main_btn[MAIN_MIC_BTN]));



    //保存记录仪按钮
    // lv_obj_t *main_record_video_btn = lv_btn_create(main_page);
   
    // // lv_obj_remove_style_all(realtime_video_btn);

    // lv_obj_set_width(main_record_video_btn, MAIN_RECORD_VIDEO_SAVE_BTN_W);      
    // lv_obj_set_height(main_record_video_btn, MAIN_RECORD_VIDEO_SAVE_BTN_H);
    // lv_obj_set_pos(main_record_video_btn,MAIN_RECORD_VIDEO_SAVE_BTN_X,MAIN_RECORD_VIDEO_SAVE_BTN_Y);

    // lv_obj_add_event_cb(main_record_video_btn,
    //                      Enter_Record_Video,
    //                      LV_EVENT_CLICKED,
    //                      NULL);





                         
}