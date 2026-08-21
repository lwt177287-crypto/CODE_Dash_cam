#include "LVGL_key.h"
#include "video_car.h"

// struct str_in_login_bool 
// {
//     int pd_login;
//     int SMS_login;
//     int instration;
//     struct str_btn s_login_btn;

// }s_in_login_bool;

void Re_Keyboard(lv_event_t *event)
{   
    struct str_re_keyboard *page = lv_event_get_user_data(event);
    if(page->info!=NULL)
    {
       page->info->btn=1;
    }
    lv_async_call(Delete_Keyboard_Page, page->keyboard_page);
}
//键盘
void Delete_Keyboard_Page(void *data)
{
    printf("键盘lvgl退出\n");
    lv_obj_del((lv_obj_t *)data);
}



//进入键盘
 void Enter_Keyboard(lv_event_t *event)
{
    //需要用键盘的按钮
    if(event != NULL)
    {
        struct str_btn* info;
        s_re_keyboard.info = lv_event_get_user_data(event);

        if( s_re_keyboard.info!=NULL)
        {
            s_re_keyboard.info->btn=1;
        }
    }

    //将按钮成退出键盘按钮
    s_re_keyboard.info=&s_login_btn[KEY_BOARD_STATE_TURETN_BTN];
    printf("进入lvgl键盘画面\n");

        //键盘
       s_re_keyboard.keyboard_page = lv_obj_create(lv_scr_act());
     lv_obj_clear_flag(s_re_keyboard.keyboard_page, LV_OBJ_FLAG_SCROLLABLE);
       lv_obj_set_width(s_re_keyboard.keyboard_page , 800);      
        lv_obj_set_height(s_re_keyboard.keyboard_page , 480);
        lv_obj_set_pos(s_re_keyboard.keyboard_page ,0,0);
        // lv_obj_remove_style_all(s_re_keyboard.keyboard_page );
        //设置不透明度，里面的子不受影响
     lv_obj_set_style_bg_opa(s_re_keyboard.keyboard_page, LV_OPA_TRANSP, 0);
        //设置不拖动
     lv_obj_set_style_pad_all(s_re_keyboard.keyboard_page,0,0);
    
    
    lv_obj_t *img = lv_img_create(s_re_keyboard.keyboard_page);
        lv_obj_set_pos(img, 0, 240);
        lv_img_set_src(img, KEYBOARD_IMAGE);
        

    //返回按钮
    lv_obj_t *re_btn = lv_obj_create(s_re_keyboard.keyboard_page);
    lv_obj_remove_style_all(re_btn);
    lv_obj_set_width(re_btn, 800);      
    lv_obj_set_height(re_btn, 260);
    lv_obj_set_pos(re_btn,0,0);

lv_obj_add_event_cb(re_btn,
                    Re_Keyboard,
                    LV_EVENT_CLICKED,
                    &s_re_keyboard);
}



//回调函数
//进入登录
 void Enter_Login(lv_event_t *event)
 {
    printf("进入lvgl登录画面\n");
    if(s_admin.admin_login_flgs==0)
    {
        lv_scr_load(settings_pd_login_page);
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
    else if(s_admin.admin_login_flgs==1)
    {
        lv_scr_load(account_info_page);
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
}

//进入短信登录
 void Enter_SMS_Login(lv_event_t *event)
{
    lv_scr_load(settings_sms_login_page);
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

//进入注册
void Enter_Instration(lv_event_t *event)
{
    lv_scr_load(settings_instration_page);
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
//注册返回
void Re_Instration(lv_event_t *event)
{
    printf("注册返回\ns_instration_re.form_pd=%d\ns_instration_re.form_pd=%d\n",s_instration_re.form_pd,s_instration_re.form_pd);
    lv_scr_load(settings_instration_page);
   if(event != NULL)
    {
        struct str_btn* info;
        info = lv_event_get_user_data(event);

        if(info != NULL)
        {
            info->btn=1;
        }
    }
    if(s_instration_re.form_pd==1)
    {
        s_instration_re.form_pd=0;
        Enter_Login(NULL);
    }
    else if(s_instration_re.form_SMS==1)
    {
        s_instration_re.form_SMS=0;
        Enter_SMS_Login(NULL);
    }
}


//确认检测
void Login_Bool(lv_event_t *event)
{
    struct str_btn* info;
   if(event != NULL)
    {
        info = lv_event_get_user_data(event);

        if(info != NULL)
        {
            info->btn=1;
        }
    }
    //等待判定结果
     printf("nihao1\n");
    sem_wait(&sem_input_info);
    printf("nihao2\n");

    if(info==&(s_login_btn[LOGIN_YES_BTN]))
   { 
        printf("判断密码登录lvgl");
        if(s_admin.admin_login_flgs==1)
        {
            Enter_Main_Settings(NULL);
            printf("成功LVGL登录\n");
        }
        else if(s_admin.admin_login_flgs==0)
        {
            printf("检测到错误回去登录界面\n");
        }
    }
    else if(info==&(s_login_btn[LOGIN_SMS_YES_BTN]))
       { 
        printf("判断短信登录lvgl");
        if(s_admin.admin_login_flgs==1)
        {
            Enter_Main_Settings(NULL);
            printf("成功LVGL短信登录\n");
        }
        else if(s_admin.admin_login_flgs==0)
        {
            printf("检测到错误回去短信登录界面\n");
        }
    }
    else if(info==&(s_instration_btn[INSTRATION_YES_BTN]))
    { 
        if(s_admin.phone_number[0]!='\0')
            {
            printf("s_admin.phone_number=%s\n",s_admin.phone_number);
            Enter_Login(NULL);
            printf("成功LVGL注册\n");
            }
        else
            {
            printf("检测到错误，注册失败\n");
            }
    }



}




// //确认检测
// void Instration_Bool(lv_event_t *event)
// {
//    if(event != NULL)
//     {
//         struct str_btn* info;
//         info = lv_event_get_user_data(event);

//         if(info != NULL)
//         {
//             info->btn=1;
//         }
//     }
//     //等待判定结果
//      printf("nihao1\n");
//     sem_wait(&sem_input_info);
//     printf("nihao2\n");
 
// }





void settings_Page_Create()
{
 // lv_indev_wait_release(lv_indev_get_act());
    main_settings_page = lv_obj_create(NULL);

    //按钮函数
    pd_Login_Page_Create();
    sms_Login_Page_Create();
    instration_Page_Create();
     account_Page_Create();


    //背景
    lv_obj_t *img = lv_img_create(main_settings_page);
    lv_img_set_src(img,SETTINGS_IMAGE);

    //登录按钮
   Btn_Create(main_settings_page,SET_CHILD_BTN_X,SET_LOGIN_BTN_Y,
            SET_LOGIN_BTN_W,SET_LOGIN_BTN_H,
            Enter_Login,&(s_settings_btn[LOGIN_BTN]));//进入登录

    //关机按钮
   Btn_Create(main_settings_page,SET_CHILD_BTN_X,SET_SHUTDOWN_BTN_Y,
            SET_SHUTDOWN_BTN_W,SET_SHUTDOWN_BTN_H,
            NULL_BTN,&(s_settings_btn[SHUTDOWN_BTN]));
    //返回主界面
   Btn_Create(main_settings_page,SET_RETURN_BTN_X,SET_RETURN_BTN_Y,
            SET_RETURN_BTN_W,SET_RETURN_BTN_H,
            Back_Main,&(s_settings_btn[SET_RETURN_BTN]));

}

//密码登录
void pd_Login_Page_Create()
{
    settings_pd_login_page = lv_obj_create(NULL);
    lv_obj_t *img = lv_img_create(settings_pd_login_page);
    lv_img_set_src(img,SETTINGS_LOGIN);

/********************按钮**************************/
       //手机号按钮
   Btn_Create(settings_pd_login_page,LOGIN_USERNAME_BTN_X,LOGIN_USERNAME_BTN_Y,
            LOGIN_USERNAME_BTN_W,LOGIN_USERNAME_BTN_H,
            Enter_Keyboard,&(s_login_btn[INPUT_USER_BTN]));
        
       //密码按钮
   Btn_Create(settings_pd_login_page,LOGIN_PASSWORD_BTN_X,LOGIN_PASSWORD_BTN_Y,
            LOGIN_PASSWORD_BTN_W,LOGIN_PASSWORD_BTN_H,
            Enter_Keyboard,&(s_login_btn[INPUT_PASSWORD_BTN]));
        
       //登录按钮 要判断成不成功吧
   Btn_Create(settings_pd_login_page,LOGIN_YES_BTN_X,LOGIN_YES_BTN_Y,
            LOGIN_YES_BTN_W,LOGIN_YES_BTN_H,
            Login_Bool,&(s_login_btn[LOGIN_YES_BTN]));

       //忘记密码按钮
   Btn_Create(settings_pd_login_page,LOGIN_FORGOT_PASSWORD_BTN_X,LOGIN_FORGOT_PASSWORD_BTN_Y,
            LOGIN_FORGOT_PASSWORD_BTN_W,LOGIN_FORGOT_PASSWORD_BTN_H,
            NULL_BTN,&(s_login_btn[FORGOT_PASSWORD_BTN]));
        
        //短信按钮
       //进入短信登录页面
   Btn_Create(settings_pd_login_page,LOGIN_SMS_BTN_X,LOGIN_SMS_BTN_Y,
            LOGIN_SMS_BTN_W,LOGIN_SMS_BTN_H,
            Enter_SMS_Login,&(s_login_btn[IN_MODE_BTN]));
         
       //注册按钮
   Btn_Create(settings_pd_login_page,LOGIN_IFICATION_BTN_X,LOGIN_IFICATION_BTN_Y,
            LOGIN_IFICATION_BTN_W,LOGIN_IFICATION_BTN_H,
            Enter_Instration,&(s_login_btn[ISTRATION_BTN]));
        
        //退出
      Btn_Create(settings_pd_login_page,LOGIN_RETURN_BTN_X,LOGIN_RETURN_BTN_Y,
            LOGIN_RETURN_BTN_W,LOGIN_RETURN_BTN_H,
            Enter_Main_Settings,&(s_login_btn[LOGIN_EXIT_BTN])); 
            
/*************************************************************/

}
//短信登录按钮
void sms_Login_Page_Create()
{
    settings_sms_login_page = lv_obj_create(NULL);
  lv_obj_t *img = lv_img_create(settings_sms_login_page);
    lv_img_set_src(img,SETTINGS_SMS_LOGIN);

/********************按钮**************************/
       //手机号按钮
   Btn_Create(settings_sms_login_page,LOGIN_USERNAME_BTN_X,LOGIN_USERNAME_BTN_Y,
            LOGIN_USERNAME_BTN_W,LOGIN_USERNAME_BTN_H,
            Enter_Keyboard,&(s_login_btn[INPUT_USER_BTN]));
        
       //短信输入按钮
   Btn_Create(settings_sms_login_page,LOGIN_PASSWORD_BTN_X,LOGIN_PASSWORD_BTN_Y,
            LOGIN_PASSWORD_BTN_W,LOGIN_PASSWORD_BTN_H,
            Enter_Keyboard,&(s_login_btn[INPUT_PASSWORD_BTN]));
        
       //登录按钮
   Btn_Create(settings_sms_login_page,LOGIN_YES_BTN_X,LOGIN_YES_BTN_Y,
            LOGIN_YES_BTN_W,LOGIN_YES_BTN_H,
            Login_Bool,&(s_login_btn[LOGIN_SMS_YES_BTN]));
        
       //忘记密码按钮
   Btn_Create(settings_sms_login_page,LOGIN_FORGOT_PASSWORD_BTN_X,LOGIN_FORGOT_PASSWORD_BTN_Y,
            LOGIN_FORGOT_PASSWORD_BTN_W,LOGIN_FORGOT_PASSWORD_BTN_H,
            NULL_BTN,&(s_login_btn[FORGOT_PASSWORD_BTN]));
        
       //切换密码按钮
   Btn_Create(settings_sms_login_page,LOGIN_SMS_BTN_X,LOGIN_SMS_BTN_Y,
            LOGIN_SMS_BTN_W,LOGIN_SMS_BTN_H,
            Enter_Login,&(s_login_btn[IN_MODE_BTN]));

       //获取验证码按钮
   Btn_Create(settings_sms_login_page,LOGIN_GET_SMS_BTN_X,LOGIN_GET_SMS_BTN_Y,
            LOGIN_GET_SMS_BTN_W,LOGIN_GET_SMS_BTN_H,
            NULL_BTN,&(s_login_btn[GET_SMS_BTN]));
        
       //注册按钮
   Btn_Create(settings_sms_login_page,LOGIN_IFICATION_BTN_X,LOGIN_IFICATION_BTN_Y,
            LOGIN_IFICATION_BTN_W,LOGIN_IFICATION_BTN_H,
            Enter_Instration,&(s_login_btn[ISTRATION_BTN]));

       //退出
      Btn_Create(settings_sms_login_page,LOGIN_RETURN_BTN_X,LOGIN_RETURN_BTN_Y,
            LOGIN_RETURN_BTN_W,LOGIN_RETURN_BTN_H,
            Enter_Main_Settings,&(s_login_btn[LOGIN_EXIT_BTN])); 

/*************************************************************/

}

//注册按钮
void instration_Page_Create()
{
    //可以用一张画布
    settings_instration_page= lv_obj_create(NULL);
  lv_obj_t *img = lv_img_create(settings_instration_page);
    lv_img_set_src(img,SETTINGS_ISTRATION);

      
/********************按钮**************************/
       //手机号按钮
   Btn_Create(settings_instration_page,INSTRATTON_USERNAME_BTN_X,INSTRATTON_USERNAME_BTN_Y,
            INSTRATTON_USERNAME_BTN_W,INSTRATTON_USERNAME_BTN_H,
            Enter_Keyboard,&(s_instration_btn[INPUT_USER_BTN]));
        
               
       //获取验证码按钮
   Btn_Create(settings_instration_page,INSTRATTON_GET_SMS_BTN_X,INSTRATTON_GET_SMS_BTN_Y,
            INSTRATTON_GET_SMS_BTN_W,INSTRATTON_GET_SMS_BTN_H,
            NULL_BTN,&(s_instration_btn[GET_SMS_BTN]));   

       //输入验证码按钮
   Btn_Create(settings_instration_page,INSTRATTON_SMS_BTN_X,INSTRATTON_SMS_BTN_Y,
            INSTRATTON_SMS_BTN_W,INSTRATTON_SMS_BTN_H,
            Enter_Keyboard,&(s_instration_btn[INPUT_SMS_BTN]));
        
       //输入密码按钮
   Btn_Create(settings_instration_page,INSTRATTON_PASSWORD_BTN_X,INSTRATTON_PASSWORD_BTN_Y,
            INSTRATTON_PASSWORD_BTN_W,INSTRATTON_PASSWORD_BTN_H,
            Enter_Keyboard,&(s_instration_btn[INPUT_PASSWORD_BTN]));

        //再次输入密码
        Btn_Create(settings_instration_page,INSTRATTON_AGAIN_PASSWORD_BTN_X,INSTRATTON_AGAIN_PASSWORD_BTN_Y,
        INSTRATTON_AGAIN_PASSWORD_BTN_W,INSTRATTON_AGAIN_PASSWORD_BTN_H,
        Enter_Keyboard,&(s_instration_btn[INPUT_AGAIN_PASSWORD_BTN]));


       //立即注册按钮，回到密码登录,要成功才行吧
   Btn_Create(settings_instration_page,INSTRATTON_YES_BTN_X,INSTRATTON_YES_BTN_Y,
            INSTRATTON_YES_BTN_W,INSTRATTON_YES_BTN_H,
            Login_Bool,&(s_instration_btn[INSTRATION_YES_BTN]));
       
        
       //返回登录按钮
   Btn_Create(settings_instration_page,INSTRATTON_LOGIN_BTN_X,INSTRATTON_LOGIN_BTN_Y,
            INSTRATTON_LOGIN_BTN_W,INSTRATTON_LOGIN_BTN_H,
            Re_Instration,&(s_instration_btn[IN_MODE_BTN]));
            
       //退出按钮
   Btn_Create(settings_instration_page,INSTRATTON_RETURN_BTN_X,INSTRATTON_RETURN_BTN_Y,
            INSTRATTON_RETURN_BTN_W,INSTRATTON_RETURN_BTN_H,
            Enter_Main_Settings,&(s_instration_btn[LOGIN_EXIT_BTN]));
        
/*************************************************************/

}
//账户信息
void account_Page_Create()
{
    account_info_page= lv_obj_create(NULL);
  lv_obj_t *img = lv_img_create(account_info_page);
    lv_img_set_src(img,SETTINGS_ACCOUNT);

        Btn_Create(account_info_page,ACCOUNT_VIP_RECHARGE_BTN_X,ACCOUNT_VIP_RECHARGE_BTN_Y,
                ACCOUNT_VIP_RECHARGE_BTN_W,ACCOUNT_VIP_RECHARGE_BTN_H,
                NULL_BTN,&(account_info_btn[ACCOUNT_VIP_BTN])); 
        
        Btn_Create(account_info_page,ACCOUNT_EXIT_BTN_X,ACCOUNT_EXIT_BTN_Y,
                ACCOUNT_EXIT_BTN_W,ACCOUNT_EXIT_BTN_H,
                Enter_Main_Settings,&(account_info_btn[ACCOUNT_EXIT_BTN])); 
    
        Btn_Create(account_info_page,ACCOUNT_RETURN_BTN_X,ACCOUNT_RETURN_BTN_Y,
                ACCOUNT_RETURN_BTN_W,ACCOUNT_RETURN_BTN_H,
                Enter_Main_Settings,&(account_info_btn[ACCOUNT_RETURN_BTN])); 

}