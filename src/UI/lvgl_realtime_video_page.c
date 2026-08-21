#include "LVGL_key.h"
#include "video_car.h"

//退出时需要等待最后一帧采集完成再退出
//摄像界面
void Realtime_Video_Page_Create()
{
    // lv_indev_wait_release(lv_indev_get_act());
    realtime_video_page = lv_obj_create(NULL);

    //视频背景
    lv_obj_t *img = lv_img_create(realtime_video_page);
    lv_img_set_src(img,TRALTIME_IMAGE);
    
    //增加按钮
    //摄像
 Btn_Create(realtime_video_page,REALTIME_VIDEO_LIFT_CHILD_BTN_X,REALTIME_RECOER_BTN_Y,
            REALTIME_RECOER_BTN_W,REALTIME_RECOER_BTN_H,
            NULL_BTN,&(s_realtime_video_btn[RECOER_BTN]));
    //辅助线
 Btn_Create(realtime_video_page,REALTIME_VIDEO_RIGHT_CHILD_BTN_X,GUIDELINE_BTN_Y,
            GUIDELINE_BTN_W,GUIDELINE_BTN_H,
            NULL_BTN,&(s_realtime_video_btn[GUIDELINE_BTN]));
    //返回
    Btn_Create(realtime_video_page,REALTIME_VIDEO_RIGHT_CHILD_BTN_X, REALTIME_RETURN_BTN_Y,
        REALTIME_RETURN_BTN_W,REALTIME_RETURN_BTN_H,
        Back_Main,&(s_realtime_video_btn[REALTIME_RETURN_BNT]));
    //关闭摄像机
    Btn_Create(realtime_video_page,REALTIME_VIDEO_LIFT_CHILD_BTN_X, EIXT_FILM_BTN_Y,
        EIXT_FILM_BTN_W,EIXT_FILM_BTN_H,
        Back_Main,&(s_realtime_video_btn[EXIT_FILM_BNT]));


}


