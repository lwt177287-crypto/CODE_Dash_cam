#include "video_car.h"


void Settings()
{
    Interface=0;
  read_JPEG_file (SETTINGS_IMAGE, mp);
    int set_state=SET_STATE_SET;
    for(;;)
    {   
        switch(set_state)
        {  
             case SET_STATE_SET:
            {    
                if(SET_RETURN &&Interface==0)//退出
                {
                    printf("退出设置\n");
                    set_state=SET_STATE_RETURN;
                }
                else if(SET_SHUTDOWN&&Interface==0 )//关机键
                {
                    printf("关机！\n");
                    //保存录像
                    set_state=SET_STATE_SHUTDOWN;
                 
                }
                else if(SET_LOGIN&&Interface==0 )
                {
                    printf("进入登录界面\n");
                    set_state=SET_STATE_LOGIN;
            
                }
                // else if(SET_WIFI&&Interface==0 )
                // {
                //     printf("进入wifi界面\n");
                //     set_state=SET_STATE_WIFI;
                //     read_JPEG_file (WIFI_IMAGE, mp);
                // }
                // else if(SET_SYSTEM_INFO&&Interface==0 )
                // {
                //     printf("进入系统信息界面\n");
                //     set_state=SET_STATE_SYSTEM_INFO;
                //     read_JPEG_file (, mp);
                // }
                // else if(SET_LANGUAGE&&Interface==0 )
                // {
                //     printf("进入语言界面\n");
                //     set_state=SET_STATE_SHUTDOWN;
                //     read_JPEG_file (LANGUAGE_IMAGE, mp);
                // }
                  break;
             }    
            case SET_STATE_LOGIN:
                {
                    Login();
                    set_state=SET_STATE_SET;
                    break;
                }  
            case   SET_STATE_WIFI:
                {
                    set_state=SET_STATE_SET;
                    break;
                }  
            case SET_STATE_LANGUAGE:
                {
                set_state=SET_STATE_SET;
                    break;
                }  
            case SET_STATE_SYSTEM_INFO:
                {
                set_state=SET_STATE_SET;

                    break;
                }  
            case SET_STATE_SHUTDOWN:
            {
                   Shut_Down();
                // set_state=SET_STATE_SET;
                //     break;
                }  
            case SET_STATE_RETURN:
            {
                return ;

            }
      }
    }
}

