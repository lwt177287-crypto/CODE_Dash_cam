#include "video_car.h"
extern int sms_time;    
 
void Login()    //密码登录
{
    s_v_c_main.set_flgs=0;
    LOGIN_STATE login_state=LOGIN_STATE_PASSWORD;
    read_JPEG_file (SETTINGS_LOGIN, mp);
    for(;;)
    {
        if(s_keyboard.Keyboard_flgs!=1)
            switch(login_state)
            {
                //注意使用完要切回来
                case LOGIN_STATE_PASSWORD://在密码登录模式
        
                    if(s_keyboard.Keyboard_flgs==0)//退出键盘
                    {
                        login_state=LOGIN_KEY_BOARD_STATE_TURETN;
                        s_keyboard.Keyboard_flgs=-1;
                    }
                    else if(LOGIN_USERNAME&&s_admin.user_input==0) //点击手机号
                    {
                        login_state=LOGIN_STATE_INPUT_USER;
                    }
                    else if(LOGIN_PASSWORD&&s_admin.password_input==0)    //点击密码
                    {
                        login_state=LOGIN_STATE_INPUT_PASSWORD;
                    }
                    else if(LOGIN_SMS)//进入验证码
                    {
                        login_state=LOGIN_STATE_IN_SMS;
                    }
                 else if(LOGIN_IFICATION)//注册
                    {
                      login_state=LOGIN_STATE_ISTRATION;
                    }
                
                    else if( LOGIN_YES) //进入登录
                    {
                        login_state=LOGIN_STATE_YES;
                    }
                     else if( LOGIN_RETURN) //退出登录
                    {
                        login_state=LOGIN_STATE_EXIT;
                    }
                    break;
                case LOGIN_STATE_INPUT_USER://输入账号
                {
                    s_admin.user_input=1;
                    printf("输入账号\n");
                     
                    Input_Info(s_keyboard.phone_number,
                        s_keyboard.keyboard_out,
                        sizeof(s_keyboard.phone_number)-1);
                        
                    login_state=LOGIN_STATE_PASSWORD;
                    break;
                }
                case LOGIN_STATE_INPUT_PASSWORD://输入密码
                {   
                    s_admin.password_input=1;
                    printf("输入密码\n");
                     Input_Info(s_keyboard.password,
                        s_keyboard.keyboard_out,
                        sizeof(s_keyboard.password)-1);
                        
                    login_state=LOGIN_STATE_PASSWORD;
                    break;
                }
                case LOGIN_STATE_IN_SMS://切换短信验证
                {
                    en0_clear();
                    printf("进入验证码登录\n");
                    memset(&s_keyboard,0,sizeof(s_keyboard));
                    read_JPEG_file (SETTINGS_SMS_LOGIN, mp);
                    SMS_Login();   //进入验证码登录页面
                    //退出页面判断是返回密码验证还是返回设置 
                    printf("返回密码登录\n");
                    if(s_v_c_main.set_flgs==0)
                    {
                        read_JPEG_file (SETTINGS_LOGIN, mp);
                        login_state=LOGIN_STATE_PASSWORD;
                        break;
                    }
                    else if(s_v_c_main.set_flgs==1)
                    {
                        printf("返回设置\n");
                        return;
                    }
                
                }
                case LOGIN_STATE_ISTRATION://切换注册
                {
                    en0_clear();
                    printf("进入\n");
                    memset(&s_keyboard,0,sizeof(s_keyboard));
                    read_JPEG_file (SETTINGS_ISTRATION, mp);
                    Login_Istration();   //进入注册页面
                    // 要判断哪种验证进来的
                    printf("返回密码登录\n");
                    if(s_v_c_main.set_flgs==0)
                    {
                        read_JPEG_file (SETTINGS_LOGIN, mp);
                        login_state=LOGIN_STATE_PASSWORD;
                        break;
                    }
                        else if(s_v_c_main.set_flgs==1)
                    {
                        return;
                    }
                
                }
                
                case LOGIN_STATE_FORGOT_PASSWORD://忘记密码，本质切换短信
                    break;
                
                case LOGIN_STATE_YES://确认
                 {
                      printf("点击确认\n");
                        //判断，可以设置三种登录注册状态，进入SMS_YES函数以不同状态判断不同内容
                       if(strlen(s_keyboard.phone_number)!=11)
                        {
                            printf("手机号格式错误\n");
                            en0_clear();
                            login_state=LOGIN_STATE_PASSWORD;
                            break;
                        }
    
                                        
                        FILE * admin_fp;
                    if(NULL== (admin_fp=fopen(ADMIN_DATA_PATH,"a+")) )    //没有则创建,追加
                    {
                        printf("打开账号文件失败，登录失败！\n");
                        en0_clear();
                        login_state=INSTRATTON_STATE;
                        break;
                    }
                    char admin_info_buf[100]={0};
                    while(fgets(admin_info_buf,sizeof(admin_info_buf),admin_fp)!=NULL)//判断文件是否有信息
                    {
                        char *phone_number=strtok(admin_info_buf,",");
                        if(strcmp(phone_number,s_keyboard.phone_number)==0)//判断手机号
                        {
                            char *password=strtok(NULL,",");
                            if(strcmp(password,s_keyboard.password)==0)//判断密码
                            {
                                //找到啦
                                //拷贝账号信息
                                memcpy(s_admin.phone_number,
                                        s_keyboard.phone_number,
                                        strlen(s_keyboard.phone_number));
                            
                                memcpy(s_admin.password,
                                        s_keyboard.password,
                                        strlen(s_keyboard.password));
                                    
                                char *vip=strtok(NULL,"\r\n");
                                if(strcmp(vip,"vip=1")==0)         
                                {
                                    s_admin.vip=1;
                                }
                                printf("登录成功\n");
                                s_admin.admin_login_flgs=1;
                                en0_clear();

                                login_state=LOGIN_STATE_PASSWORD;
                                fclose(admin_fp);
                                //返回设置界面
                                Return_Set();
                               return ;
                            }
                        }
                    }
                    printf("手机号或密码错误！\n");
                
                
                    en0_clear();
                    login_state=LOGIN_STATE_PASSWORD;
                    break;
                    
                }

                case LOGIN_KEY_BOARD_STATE_TURETN://键盘退出
                {
                    //退出键盘后
                    if(s_admin.user_input==1)
                    {
                        s_admin.user_input=0;
                        
                        Clear_Keyboard(s_keyboard.phone_number,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.phone_number)-1);
                    }
                    if(s_admin.password_input==1)
                    {
                         printf("密码是%s\n",s_keyboard.keyboard_out);
                         s_admin.password_input=0;
                        
                           Clear_Keyboard(s_keyboard.password,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.password)-1);
               
                    }
                        en0_clear();
                    login_state=LOGIN_STATE_PASSWORD;
                    break;
                }  
                case LOGIN_STATE_EXIT://退出
                {
                    en0_clear();
                    Return_Set();
                    return ;    
                }
            }
    }
}






void SMS_Login()
{
    printf("进入短信验证\n");
   s_v_c_main.set_flgs=0;
     LOGIN_STATE login_state=LOGIN_STATE_SMS;
    for(;;)
    {
        //如果键盘没有打开
        if(s_keyboard.Keyboard_flgs!=1)
            switch(login_state)
            {
                //注意使用完要切回来
                case LOGIN_STATE_SMS://在密码登录模式
                    if(s_keyboard.Keyboard_flgs==0)//退出键盘
                    {
                        login_state=LOGIN_KEY_BOARD_STATE_TURETN;
                        s_keyboard.Keyboard_flgs=-1;
                    }
                    else if(LOGIN_USERNAME&&s_admin.user_input==0) //输入手机号
                    {
                        login_state=LOGIN_STATE_INPUT_USER;
                    }
                    else if(LOGIN_PASSWORD&&s_admin.password_input==0)    //输入验证码
                    {
                        login_state=LOGIN_STATE_INPUT_PASSWORD;
                    }
                    else if(LOGIN_SMS)//进入密码登录模式
                    {
                        login_state=LOGIN_STATE_IN_PASSWORD;
                    }
                
                    else if(LOGIN_GET_SMS)//获取验证码
                    {
                        login_state=LOGIN_STATE_GET_SMS;
                    }
                
                    else if(LOGIN_IFICATION)//注册
                    {
                        login_state=LOGIN_STATE_ISTRATION;
                    }
                
                    else if( LOGIN_RETURN) //退出登录
                    {
                        login_state=LOGIN_STATE_EXIT;
                    }

                    else if(LOGIN_YES ) //登录
                    {
                        login_state=LOGIN_STATE_YES;
                    }
                    
                    break;
                        
                case LOGIN_STATE_INPUT_USER://输入手机号
                {
                    s_admin.user_input=1;
                    printf("输入手机号\n");
                 
                    Input_Info(s_keyboard.phone_number,
                                s_keyboard.keyboard_out,
                                sizeof(s_keyboard.phone_number)-1);
                    login_state=LOGIN_STATE_SMS;
                    break;
                }
                case LOGIN_STATE_INPUT_PASSWORD://输入验证码
                {   
                    s_admin.sms_input=1;
                    printf("输入验证码\n");
                       Input_Info(s_keyboard.sms_input,
                                s_keyboard.keyboard_out,
                                sizeof(s_keyboard.sms_input)-1);
                    login_state=LOGIN_STATE_SMS;
                    break;
                }
                case LOGIN_STATE_IN_PASSWORD://切换密码验证
                {
                    en0_clear();
                    printf("切换密码登录\n");
                    memset(&s_keyboard,0,sizeof(s_keyboard));
                    login_state=LOGIN_STATE_SMS;
                    return ;   
                }
                case LOGIN_STATE_GET_SMS://获取验证码
                {
                    SMS_Get();
                    en0_clear();
                    login_state=LOGIN_STATE_SMS;
                    break;
                }
                case LOGIN_STATE_ISTRATION://切换注册 一模一样
                   {
                        en0_clear();
                        printf("进入\n");
                        //清空键盘
                        memset(&s_keyboard,0,sizeof(s_keyboard));
                        read_JPEG_file (SETTINGS_ISTRATION, mp);
                        Login_Istration();   //进入注册页面
                        // 要判断哪种验证进来的
                        printf("返回登录\n");
                        if(s_v_c_main.set_flgs==0)
                        {
                            read_JPEG_file (SETTINGS_SMS_LOGIN, mp);
                            login_state=LOGIN_STATE_SMS;
                        }
                        else if(s_v_c_main.set_flgs==1)
                        {
                            return;
                        }
                
                }

                case LOGIN_STATE_YES://确认
                {
                    printf("点击确认\n");
                         int start =SMS_YES();
                    if(start==INSTRATTON_STATE)
                    {
                        login_state=INSTRATTON_STATE;
                        printf("检测到错误\n");
                        break;
                    }
                 
                        FILE * admin_fp;
                    if(NULL== (admin_fp=fopen(ADMIN_DATA_PATH,"a+")) )    //没有则创建,追加
                    {
                        printf("打开账号文件失败，登录失败！\n");
                        en0_clear();
                        login_state=INSTRATTON_STATE;
                        break;
                    }
                    char admin_info_buf[100]={0};
                    while(fgets(admin_info_buf,sizeof(admin_info_buf),admin_fp)!=NULL)//判断文件是否有信息
                    {
                        char *phone_number=strtok(admin_info_buf,",");
                        if(strcmp(phone_number,s_keyboard.phone_number)==0)//判断手机号
                        {
                            //找到啦
                            //拷贝账号信息
                            memcpy(s_admin.phone_number,
                                    s_keyboard.phone_number,
                                    strlen(s_keyboard.phone_number));
                        
                            memcpy(s_admin.password,
                                    s_keyboard.password,
                                    strlen(s_keyboard.password));
                            
                            strtok(NULL,",");
                            char *vip=strtok(NULL,"\r\n");
                                    
                            if(strcmp(vip,"vip=1")==0)         
                            {
                                s_admin.vip=1;
                            }
                            printf("登录成功\n");
                            s_admin.admin_login_flgs=1;
                            en0_clear();
                            login_state=LOGIN_STATE_SMS;
                            fclose(admin_fp);
                            Return_Set();
                            return ;
                        }
                    }
                        printf("手机号或密码错误！\n");
                        //打开文件查找账号
                        //将账号信息拷贝
                        //清除键盘
                        //返回设置界面
                    
                        en0_clear();
                        login_state=LOGIN_STATE_SMS;
                        break;
                    
                }
                case LOGIN_KEY_BOARD_STATE_TURETN://键盘退出
                {
                    //退出键盘后
                    if(s_admin.user_input==1)
                    {
                        s_admin.user_input=0;
                        printf("手机号是%s\n",s_keyboard.keyboard_out);
                             Clear_Keyboard(s_keyboard.phone_number,
                                    s_keyboard.keyboard_out,
                                    sizeof(s_keyboard.phone_number)-1);

                    }
                    if(s_admin.sms_input==1)
                    {
                        printf("验证码是%s\n",s_keyboard.keyboard_out);
                        //截取键盘输入的验证码
                        s_admin.sms_input=0;
                         Clear_Keyboard(s_keyboard.sms_input,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.sms_input)-1);
                    }
                        en0_clear();
                        login_state=LOGIN_STATE_SMS;
                        break;
                }  
                case LOGIN_STATE_EXIT://退出
                {
                    en0_clear();
                    Return_Set();
                    return ;    
                }
            }
        }
}

void Return_Set()
{
    memset(&s_keyboard,0,sizeof(s_keyboard));
    s_v_c_main.set_flgs=1;
    read_JPEG_file (SETTINGS_IMAGE, mp);
}


















void  Login_Istration()
{
    printf("进入注册验证\n");
   s_v_c_main.set_flgs=0;
    LOGIN_STATE instratton_state=INSTRATTON_STATE;
    for(;;)
    {
        //如果键盘没有打开
        if(s_keyboard.Keyboard_flgs!=1)
            switch(instratton_state)
            {
                //注意使用完要切回来
                case INSTRATTON_STATE://在注册模式
                    if(s_keyboard.Keyboard_flgs==0)//退出键盘
                    {
                        instratton_state=INSTRATTON_KEY_BOARD_STATE_TURETN;
                        s_keyboard.Keyboard_flgs=-1;
                    }
                    else if(INSTRATTON_USERNAME&&s_admin.user_input==0) //输入手机号
                    {
                        instratton_state=INSTRATTON_STATE_INPUT_USER;
                    }
                    else if(INSTRATTON_SMS&&s_admin.sms_input==0)    //输入验证码
                    {
                        instratton_state=INSTRATTON_STATE_INPUT_SMS;
                    }
              
                    else if(INSTRATTON_PASSWORD&&s_admin.password_input==0)    //输入密码
                    {
                        instratton_state=INSTRATTON_STATE_INPUT_PASSWORD;
                    }
              
                    else if(INSTRATTON_AGAIN_PASSWORD&&s_admin.password_input==0)    //再次输入
                    {
                        instratton_state=INSTRATTON_STATE_AGAIN_INPUT_PASSWORD;
                    }
                    else if(INSTRATTON_GET_SMS)//获取验证码
                    {
                        instratton_state=INSTRATTON_STATE_GET_SMS;
                    }
                
                    else if( INSTRATTON_RETURN) //退出注册
                    {
                        instratton_state=INSTRATTON_STATE_EXIT;
                    }

                    else if(INSTRATTON_LOGIN) //返回登录
                    {
                        instratton_state=INSTRATTON_STATE_LONIG;
                    }

                    else if(INSTRATTON_YES ) //确认注册
                    {
                        instratton_state=INSTRATTON_STATE_YES;
                    }
                    break;
                        
                case INSTRATTON_STATE_INPUT_USER://输入手机号
                {
                    s_admin.user_input=1;
                    printf("输入手机号\n");

                   Input_Info(s_keyboard.phone_number,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.phone_number)-1);
                    instratton_state=INSTRATTON_STATE;
                    break;
                }
                case INSTRATTON_STATE_INPUT_SMS://输入验证码
                {   
                    s_admin.sms_input=1;
                    printf("输入验证码\n");
                     Input_Info(s_keyboard.sms_input,
                                s_keyboard.keyboard_out,
                                sizeof(s_keyboard.sms_input)-1);
                    instratton_state=INSTRATTON_STATE;
                    break;
                }
                case INSTRATTON_STATE_INPUT_PASSWORD://密码
                {   
                    s_admin.password_input=1;
                    printf("输入密码\n");
                      Input_Info(s_keyboard.password,
                        s_keyboard.keyboard_out,
                        sizeof(s_keyboard.password)-1);
                    instratton_state=INSTRATTON_STATE;
                    break;
                }
                case INSTRATTON_STATE_AGAIN_INPUT_PASSWORD://再次输入密码
                {   
                    s_admin.password_again_input=1;
                    printf("再次输入密码\n");
                      Input_Info(s_keyboard.again_password,
                        s_keyboard.keyboard_out,
                        sizeof(s_keyboard.again_password)-1);
                    instratton_state=INSTRATTON_STATE;
                    break;
                }
                case INSTRATTON_STATE_LONIG://切换登录
                {
                    en0_clear();
                    printf("注册切换登录\n");
                    memset(&s_keyboard,0,sizeof(s_keyboard));
                    instratton_state=INSTRATTON_STATE;
                    return ;   
                }
                case INSTRATTON_STATE_GET_SMS://获取验证码
                {
                    SMS_Get();

                    en0_clear();
                    instratton_state=INSTRATTON_STATE;
                    break;
                }
                case INSTRATTON_STATE_YES://确认
                {
                    printf("点击确认\n");
                    int start =SMS_YES();
                    if(start==INSTRATTON_STATE)
                    {
                        instratton_state=INSTRATTON_STATE;
                        printf("检测到错误\n");
                        break;
                    }
                    
                   
                    printf("1\n");
                    if(strcmp(s_keyboard.password,s_keyboard.again_password)!=0 ||strlen(s_keyboard.password)==0)
                    {
                          en0_clear();
                          printf("两次密码不一样/密码不能为空\n");
                        instratton_state=INSTRATTON_STATE;
                         break;
                    }
                    //判断是否注册过
                        FILE * admin_fp;
                        if(NULL== (admin_fp=fopen(ADMIN_DATA_PATH,"a+")) )    //没有则创建,追加
                        {
                          printf("打开账号文件失败，注册失败！\n");
                            en0_clear();
                            instratton_state=INSTRATTON_STATE;
                            break;
                        }
                        //  记录注册账号的信息，与文件存储的账号是否重复
                        char admin_info_buf[100]={0};
                        int search_account=1;
                        while(fgets(admin_info_buf,sizeof(admin_info_buf),admin_fp)!=NULL)
                        {
                              char *phone_number=strtok(admin_info_buf,",");
                         if(strcmp(phone_number,s_keyboard.phone_number)==0)//判断手机号
                            {

                                   printf("该账号已经注册过！\n");
                                    en0_clear();
                                  instratton_state=INSTRATTON_STATE;
                                  fclose(admin_fp);
                                  //清空该验证码，因为已经没用，也可以不清空让他用来登录
                                  memset(s_admin.sms_out,0,strlen(s_admin.sms_out));
                                  printf("验证码不可用\n");
                                   search_account=0;
                                   break;
                                }
                        }
                        //若注册过则退出确认
                        if(search_account==0)
                        {
                            instratton_state=INSTRATTON_STATE;
                            break;
                        }
                    
                        //写入数据
                          memcpy(s_admin.phone_number,
                                s_keyboard.phone_number,
                                strlen(s_keyboard.phone_number));
                                
                        memcpy(s_admin.password,
                                s_keyboard.password,
                                strlen(s_keyboard.password));
                                            
                        sprintf(admin_info_buf,"%s,%s,vip=0\r\n",s_admin.phone_number,s_admin.password);
                        fwrite(admin_info_buf,1,strlen(admin_info_buf),admin_fp);
                        printf("注册成功！\n");
                        fclose(admin_fp);
                     
                        //切到登录界面登录
                        instratton_state=INSTRATTON_STATE_LONIG;
                        break;
                    
                }
                case INSTRATTON_KEY_BOARD_STATE_TURETN://键盘退出
                {
                    //退出键盘后
                    if(s_admin.user_input==1)
                    {
                        s_admin.user_input=0;
                        printf("手机号是%s\n",s_keyboard.keyboard_out);
                        
                        Clear_Keyboard(s_keyboard.phone_number,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.phone_number)-1);
                    }
                    if(s_admin.sms_input==1)
                    {
                        printf("验证码是%s\n",s_keyboard.keyboard_out);
                        s_admin.sms_input=0;
                         Clear_Keyboard(s_keyboard.sms_input,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.sms_input)-1);
                    }
                      if(s_admin.password_input==1)
                    {
                         printf("密码是%s\n",s_keyboard.keyboard_out);
                         s_admin.password_input=0;
                        
                           Clear_Keyboard(s_keyboard.password,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.password)-1);
                    }
                     if(s_admin.password_again_input==1)
                    {
                         printf("第二次密码是%s\n",s_keyboard.keyboard_out);
                         s_admin.password_again_input=0;
                        
                           Clear_Keyboard(s_keyboard.again_password,
                            s_keyboard.keyboard_out,
                            sizeof(s_keyboard.again_password)-1);
                    }
                        en0_clear();
                        instratton_state=INSTRATTON_STATE;
                        break;
                }  
                case LOGIN_STATE_EXIT://退出
                {
                    en0_clear();
                    Return_Set();
                    return ;    
                }
            }
        }
}

