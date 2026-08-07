#include "video_car.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype_lcd.h"
#define FONT_PATH "/STSONG.TTF"

void showfont(int x,int y,int size,int color,const char * font )
{
    //初始化
    freetype_init(FONT_PATH);
    //设置大小
    freetype_set_size(size);

    freetype_set_lcd(mp);
    //显示内容
    lcd_draw_text(x,y,font,color);


}