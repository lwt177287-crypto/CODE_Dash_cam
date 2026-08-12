#include "video_car.h"

void Account_Info()
{
    s_v_c_main.set_flgs=0;
    read_JPEG_file (SETTINGS_ACCOUNT, mp);
    enum Account_start E_account_start=ACCOUNT_START;
    en0_clear();
    
    for(;;)
    {
        switch (E_account_start)
        {
            case ACCOUNT_START:
                if(ACCOUNT_VIP_RECHARGE)
                {
                    E_account_start=ACCOUNT_START_VIP_RECHARGE;
                    break;
                }
                else  if(ACCOUNT_SAFE)
                {
                    E_account_start=ACCOUNT_START_SAFE;
                    break;
                }
                else  if(ACCOUNT_DEVICE_MANAGEMENT)
                {
                    E_account_start=ACCOUNT_START_DEVICE_MANAGEMENT;
                    break;
                }
                else if(ACCOUNT_EXIT)
                {
                    E_account_start=ACCOUNT_START_EXIT;
                    break;
                }
                else if(ACCOUNT_RETURN)
                {
                    E_account_start=ACCOUNT_START_RETURN;
                    break;
                }
                break;
            case ACCOUNT_START_VIP_RECHARGE://vip
            {
                en0_clear();
                //......确认订单状态后
                 FILE * admin_fp;
                if(NULL== (admin_fp=fopen(ADMIN_DATA_PATH,"r+")) )    //没有则创建,追加
                {
                    printf("打开账号文件失败，注册失败！\n");
                    E_account_start=ACCOUNT_START;
                    break;
                }
                char admin_info_buf[1000];
                char * end;
                int search_account=1;
                char buf_p[1000];
                while(fgets(admin_info_buf,sizeof(admin_info_buf),admin_fp)!=NULL)
                {
                    memset(buf_p,0,strlen(buf_p));
                    memcpy(buf_p,admin_info_buf,strlen(admin_info_buf));
                    printf("admin_info_buf=%s",admin_info_buf);
                    char *phone_number=strtok(admin_info_buf,",");
                    
                    if(strcmp(phone_number,s_admin.phone_number)==0)//判断手机号
                    {
                        char *password=strtok(NULL,",");
                        if(strcmp(password,s_admin.password)==0)//判断密码
                        {
                          char *vip=strtok(NULL,"\r\n");

                            if(strcmp(vip,"vip=1")==0)  
                            {
                                fclose(admin_fp);
                                search_account=0;
                                E_account_start=ACCOUNT_START;
                                break;
                            }
                            else
                            {
                                //已经被切割，没有vip
                                end=strstr(buf_p,"vip=0");
                                int sk=strlen(buf_p)-strlen(end);
                                //回退到当前行首
                                fseek(admin_fp, -strlen(buf_p), SEEK_CUR);
                                fseek(admin_fp,sk,SEEK_CUR);
                                fprintf(admin_fp,"vip=1");
                                s_admin.vip=1;
                                printf("恭喜您成为了尊贵的VIP\n");
                                break;
                            }
                        }
                    }
                }
                if(search_account==0)
                {
                    printf("您已经是vip\n");
                    break;
                }
                fclose(admin_fp);
                E_account_start=ACCOUNT_START;
                //改lvgl图形
                break;
            }
            case ACCOUNT_START_SAFE:    //改密码,之后再实现
            {
                    en0_clear();
                E_account_start=ACCOUNT_START;
                break;
            }
            case ACCOUNT_START_DEVICE_MANAGEMENT:
            {
                    en0_clear();

                E_account_start=ACCOUNT_START;
                break;
            }
            case ACCOUNT_START_EXIT: //退出登录
            {
                if(s_admin.admin_login_flgs==1)
               { 
                memset(&s_admin,0,sizeof(s_admin));
                printf("退出登录！\n");
                en0_clear();
                read_JPEG_file (SETTINGS_IMAGE, mp);
                s_v_c_main.set_flgs=1;
                return ;
                }
                else
                {
                    printf("您还未登录\n");
                }

                E_account_start=ACCOUNT_START;
                break;
            }
            case ACCOUNT_START_RETURN:  //返回
            {
                printf("返回\n");
                    en0_clear();
                E_account_start=ACCOUNT_START;
                read_JPEG_file (SETTINGS_IMAGE, mp);
                s_v_c_main.set_flgs=1;
                return ;
            }
           
        }



    }




}

