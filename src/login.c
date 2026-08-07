#include "video_car.h"
static int user_input=0;
static int password_input=0;
static char sms_out[5]={0};
static int sms_time=0;

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
                        login_state=KEY_BOARD_STATE_TURETN;
                        s_keyboard.Keyboard_flgs=-1;
                    }
                    else if(LOGIN_USERNAME&&user_input==0) //点击手机号
                    {
                        login_state=LOGIN_STATE_INPUT_USER;
                    }
                    else if(LOGIN_PASSWORD&&password_input==0)    //点击密码
                    {
                        login_state=LOGIN_STATE_INPUT_PASSWORD;
                    }
                    else if(LOGIN_SMS)//进入验证码
                    {
                        login_state=LOGIN_STATE_IN_SMS;
                    }
                
                    else if( LOGIN_RETURN) //退出登录
                    {
                        login_state=LOGIN_STATE_EXIT;
                    }
                    break;
                case LOGIN_STATE_INPUT_USER://输入账号
                {
                    user_input=1;
                    printf("输入账号\n");
                    if(strlen(s_keyboard.phone_number)>=11)
                    {
                        printf("用户名已到达上限!\n");
                    }
                    memcpy(s_keyboard.keyboard_out,s_keyboard.phone_number,strlen(s_keyboard.phone_number));  //将没写完的手号接着写
                    Key_Board_Open();
                    login_state=LOGIN_STATE_PASSWORD;
                    break;
                }
                case LOGIN_STATE_INPUT_PASSWORD://输入密码
                {   
                    password_input=1;
                    printf("输入密码\n");
                    if(strlen(s_keyboard.password)>=19)
                    {
                        printf("密码已到达上限!\n");
                    }
                    memcpy(s_keyboard.keyboard_out,s_keyboard.password,strlen(s_keyboard.password));             //将没写完的密码接着写
                    Key_Board_Open(); //打开键盘
                    login_state=LOGIN_STATE_PASSWORD;
                    break;
                }
                case LOGIN_STATE_IN_SMS://切换短信验证
                {
                    en0_clear();
                    printf("进入验证码登录\n");
                    read_JPEG_file (SETTINGS_SMS_LOGIN, mp);
                    SMS_Login();   //进入验证码登录页面
                    if(s_v_c_main.set_flgs==0)
                    {
                        read_JPEG_file (SETTINGS_LOGIN, mp);
                        login_state=LOGIN_STATE_PASSWORD;
                    }
                    else if(s_v_c_main.set_flgs==1)
                    {
                        return;
                    }
                
                }
                case LOGIN_STATE_ISTRATION://切换注册
                    break;
                
                case LOGIN_STATE_FORGOT_PASSWORD://忘记密码，本质切换短信
                    break;
                
                case LOGIN_STATE_YES://确认
                    break;

                case KEY_BOARD_STATE_TURETN://键盘退出
                {
                    //退出键盘后
                    if(user_input==1)
                    {
                        printf("手机号是%s\n",s_keyboard.keyboard_out);
                        user_input=0;
                        if(strlen(s_keyboard.keyboard_out)>11)
                        {
                            printf("手机号过长,截取到前11字符\n");
                            memcpy(s_keyboard.phone_number,s_keyboard.keyboard_out,11);
                            s_keyboard.phone_number[11]='\0';
                            printf("手机号是%s\n",s_keyboard.phone_number);
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                        else if(strlen(s_keyboard.keyboard_out)>0)
                        {
                            memcpy(s_keyboard.phone_number,s_keyboard.keyboard_out,strlen(s_keyboard.keyboard_out));
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                    }
                    if(password_input==1)
                    {
                    printf("密码是%s\n",s_keyboard.keyboard_out);
                        password_input=0;
                        if(strlen(s_keyboard.keyboard_out)>=19)
                        {
                            printf("密码过长,截取到前19字符\n");
                            memcpy(s_keyboard.password,s_keyboard.keyboard_out,19);
                            s_keyboard.password[19]='\0';
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                        else if(strlen(s_keyboard.keyboard_out)>0)
                        {
                            memcpy(s_keyboard.password,s_keyboard.keyboard_out,strlen(s_keyboard.keyboard_out));
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                    }
                        en0_clear();
                    login_state=LOGIN_STATE_PASSWORD;

                }  
                
                break;
                
                case LOGIN_STATE_EXIT://退出
                {
                    en0_clear();
                    memset(s_keyboard.keyboard_out,0,sizeof(s_keyboard.keyboard_out));
                    memset(s_keyboard.phone_number,0,sizeof(s_keyboard.phone_number));
                    memset(s_keyboard.password,0,sizeof(s_keyboard.password));
                    s_v_c_main.set_flgs=1;
                    read_JPEG_file (SETTINGS_IMAGE, mp);
                    return ;    
                }
            }
    }
}


void Key_Board_Open() //输入账号
{
      en0_clear();
   if(-1== pthread_create(&thread_keyboard, NULL,child_keyboard,NULL))//打开键盘
   {
        fprintf(stderr,"create pthread_key_board error:%s",strerror(errno));
        exit(-1);
    }
    pthread_detach(thread_keyboard);
    s_keyboard.Keyboard_flgs=1;
}



void SMS_Login()
{
    printf("进入短信验证\n");
   s_v_c_main.set_flgs=0;
    int login_state=LOGIN_STATE_SMS;
    for(;;)
    {
        if(s_keyboard.Keyboard_flgs!=1)
            switch(login_state)
            {
                //注意使用完要切回来
                case LOGIN_STATE_SMS://在密码登录模式
                    if(s_keyboard.Keyboard_flgs==0)//退出键盘
                    {
                        login_state=KEY_BOARD_STATE_TURETN;
                        s_keyboard.Keyboard_flgs=-1;
                    }
                    else if(LOGIN_USERNAME&&user_input==0) //输入手机号
                    {
                        login_state=LOGIN_STATE_INPUT_USER;
                    }
                    else if(LOGIN_PASSWORD&&password_input==0)    //输入验证码
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
                    user_input=1;
                    printf("输入手机号\n");
                    if(strlen(s_keyboard.phone_number)>=11)
                    {
                        printf("手机号已到达上限!\n");
                    }
                    memcpy(s_keyboard.keyboard_out,s_keyboard.phone_number,strlen(s_keyboard.phone_number));  //将没写完的手号接着写
                    Key_Board_Open();
                    login_state=LOGIN_STATE_SMS;
                    break;
                }
                case LOGIN_STATE_INPUT_PASSWORD://输入密码
                {   
                    password_input=1;
                    printf("输入验证码\n");
                    if(strlen(s_keyboard.sms_input)>=4)
                    {
                        printf("验证码已到达上限!\n");
                    }
                    memcpy(s_keyboard.keyboard_out,s_keyboard.sms_input,strlen(s_keyboard.sms_input));             //将没写完的密码接着写
                    Key_Board_Open(); //打开键盘
                    login_state=LOGIN_STATE_SMS;
                    break;
                }
                case LOGIN_STATE_IN_PASSWORD://切换密码验证
                {
                        en0_clear();
                    login_state=LOGIN_STATE_SMS;
                    return ;   
                }
                case LOGIN_STATE_GET_SMS://获取验证码
                {
                    if(strlen(s_keyboard.phone_number)==11)
                    {
                        if(time(NULL)-sms_time<=60)
                        {
                            printf("等待重新获取验证码（%d）\n",60-time(NULL)+sms_time);
                            
                        }
                        else
                        {
                            sms_time=time(NULL);
                            printf("%d\n",sms_time);
                            int a=0;
                            if((a=SMS(s_keyboard.phone_number))==0)
                            {
                                fprintf(stderr,"get ssm error %s",strerror(errno));
                            }
                            else
                            {
                                sprintf(sms_out,"%d",a);
                            }
                        }
                    }
                    else
                    {
                        printf("请输入手机号！\n");
                    }
                        en0_clear();
                    login_state=LOGIN_STATE_SMS;
                    break;
                }
                case LOGIN_STATE_ISTRATION://切换注册
                    break;

                case LOGIN_STATE_YES://确认
                {
                    if(strlen(sms_out)!=4)
                    {
                        printf("系统验证码错误~");
                    }
                    if(strcmp(sms_out,s_keyboard.sms_input)!=0)
                    {
                        printf("验证码输入错误！请重新输入\n");
                    }
                    else
                    {
                        printf("登录成功\n");
                        //返回设置界面
                    }
                        en0_clear();
                        login_state=LOGIN_STATE_SMS;
                        break;
                    
                }
                case KEY_BOARD_STATE_TURETN://键盘退出
                {
                    //退出键盘后
                    if(user_input==1)
                    {
                        user_input=0;
                        if(strlen(s_keyboard.keyboard_out)>11)
                        {
                            printf("手机号过长,截取到前11字符\n");
                            memcpy(s_keyboard.phone_number,s_keyboard.keyboard_out,11);
                            s_keyboard.phone_number[11]='\0';
                            printf("手机号是%s\n",s_keyboard.phone_number);
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                        else if(strlen(s_keyboard.keyboard_out)>0)
                        {
                            memcpy(s_keyboard.phone_number,s_keyboard.keyboard_out,strlen(s_keyboard.keyboard_out));
                            printf("手机号是%s\n",s_keyboard.phone_number);
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                    }
                    if(password_input==1)
                    {
                        printf("验证码是%s\n",s_keyboard.keyboard_out);
                        password_input=0;
                        if(strlen(s_keyboard.keyboard_out)>=4)
                        {
                            printf("验证码过长,截取到前4个字符\n");
                            memcpy(s_keyboard.sms_input,s_keyboard.keyboard_out,4);
                            s_keyboard.sms_input[4]='\0';
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                        else if(strlen(s_keyboard.keyboard_out)>0)
                        {
                            memcpy(s_keyboard.sms_input,s_keyboard.keyboard_out,strlen(s_keyboard.keyboard_out));
                            memset(s_keyboard.keyboard_out,0,strlen(s_keyboard.keyboard_out));
                        }
                    }
                        en0_clear();
                        login_state=LOGIN_STATE_SMS;
                        break;
                }  
                case LOGIN_STATE_EXIT://退出
                {
                    en0_clear();
                    memset(s_keyboard.keyboard_out,0,sizeof(s_keyboard.keyboard_out));
                    memset(s_keyboard.phone_number,0,sizeof(s_keyboard.phone_number));
                    memset(s_keyboard.password,0,sizeof(s_keyboard.password));
                    s_v_c_main.set_flgs=1;
                    en0_clear();
                    login_state=LOGIN_STATE_SMS;
                    read_JPEG_file (SETTINGS_IMAGE, mp);
                    return ;    
                }
            }
        }
}
