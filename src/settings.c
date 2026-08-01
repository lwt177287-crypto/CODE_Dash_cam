#include "video_car.h"
 int user_out=0;
    int password_out=0;
void Settings()
{
    Interface=0;
    set_flgs=1;
  read_JPEG_file (SETTINGS_IMAGE, mp);

    for(;;)
    {
        if(SET_RETURN &&Interface==0&&set_flgs==1)//再次点击就退出
        {
            printf("退出设置\n");
            return;
        }
        else if(SET_SHUTDOWN&&Interface==0 &&set_flgs==1)//关机键
        {
            printf("关机！\n");
            Shut_Down();
        }
        else if(SET_LOGIN&&Interface==0 &&set_flgs==1&&login_flgs==0)
        {
            login_flgs=0;
            printf("进入登录界面\n");
            Login();
        }
        // else if(SET_WIFI&&Interface==0 &&set_flgs==1)
        // {
        //     printf("进入wifi界面\n");
        //     read_JPEG_file (WIFI_IMAGE, mp);
        // }
        // else if(SET_BLUETOOTH&&Interface==0 &&set_flgs==1)
        // {
        //     printf("进入蓝牙界面\n");
        //     read_JPEG_file (BLUETOOTH_IMAGE, mp);
        // }
        // else if(SET_LANGUAGE&&Interface==0 &&set_flgs==1)
        // {
        //     printf("进入语言界面\n");
        //     read_JPEG_file (LANGUAGE_IMAGE, mp);
        // }

    }

}


void Login()    //密码登录
{
    set_flgs=0;
    login_flgs=1;
   
    read_JPEG_file (SETTINGS_LOGIN, mp);
    for(;;)
    {
        if((LOGIN_USERNAME||LOGIN_PASSWORD)&&login_flgs==1&&Keyboard_flgs==0)//进入键盘
        {
            login_flgs=0;
            if(LOGIN_USERNAME&&user_out==0)
            {
                user_out=1;
                printf("输入账号\n");
                memcpy(keyboard_out,admin,strlen(admin));               //将没写完的用户名接着写
            }
            else if(LOGIN_PASSWORD&&password_out==0)
            {
                password_out=1;
                printf("输入密码\n");
                memcpy(keyboard_out,password,strlen(password));             //将没写完的密码接着写
            }
            Login_In();
        }


        //键盘退出处理
        if(Keyboard_return==1&&Keyboard_flgs==1)
        // if((KEYBOARD_CLOSE ||KEYBOARD_RETURN) &&login_flgs==0&&Keyboard_flgs==1)
         {
            if(user_out==1)
                {
                    printf("用户是%s\n",keyboard_out);
                    memcpy(admin,keyboard_out,strlen(keyboard_out));
                    memset(keyboard_out,0,strlen(keyboard_out));
                    user_out=0;
                }
            else if(password_out==1)
                { 
                    printf("密码是%s\n",keyboard_out);
                    memcpy(password,keyboard_out,strlen(keyboard_out));
                    memset(keyboard_out,0,strlen(keyboard_out));
                    password_out=0;
               }   
                pthread_join(thread_keyboard,NULL);
                en0_clear();
                Keyboard_flgs=0;
                login_flgs=1;
        }
    


              
       
        // if(Keyboard_flgs==0&&login_flgs==0)//进入键盘
        // else if(Keyboard_flgs==1)//退出键盘
        // {
        //     pthread_join(thread_keyboard,NULL);
        //     Keyboard_flgs=0;
        // }


        // else if()//获取验证码
        // SMS();
    }
// //退出时
// set_flgs=1;
//     login_flgs=0;
}


void Login_In() //输入账号
{
      en0_clear();
    Keyboard_return=0;
    pthread_create(&thread_keyboard, NULL,child_keyboard,NULL);//打开键盘
    Keyboard_flgs=1;
}




