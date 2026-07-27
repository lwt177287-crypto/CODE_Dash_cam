#include "video_car_exe.h"

void Line_Car(unsigned int *mp)
{  
            if(!(LINE_H>=SIZE&&SIZE>=ROW_H&& LINE_H>=SIZE_H && IMAGE_WIDTH>=ROW_W&&ROW_W>=LINE_W && WIDTH>=IMAGE_WIDTH&&HEIGHT>=IMAGE_HEIGHT))
    {
        perror("Requirement:(LINE_H>SIZE>ROH_H,LINE_H>SIZE_H,IMAGE_WIDTH>ROW_W>LINE_W)set error!");
    }
    //线宽度、线长度、颜色、中断量自定义
    //固定前站1/4，中占3/8，尾占3/8，每一阶段开始的第一条线结尾前是长横线，每一段线有间隔
    unsigned int *line_l=mp+(IMAGE_HEIGHT-1)*IMAGE_WIDTH;   //左线
    unsigned int *line_r=mp+IMAGE_HEIGHT*IMAGE_WIDTH-1;       //右线
    unsigned int color=LINE_COLOR_STAT;
    unsigned color_tmp=LINE_COLOR_STAT;
    int num=0;

    int size_color=color;   //每一条线段的颜色（起点是什么颜色就是什么颜色）
// LINE_H>SIZE>ROH_H,LINE_H>SIZE_H,IMAGE_WIDTH>ROW_W>LINE_W
  
    for(int j=0;j<LINE_H;j++)
 {  
    if(j==LINE_H*STAT/(STAT+END+MID))    //中间颜色开始
    {
        color=LINE_COLOR_MID;
        if(color!=color_tmp)
        num=0;
        color_tmp=color;
    }
    else if(j==LINE_H*(STAT+MID)/(STAT+END+MID))      //末尾颜色开始
   { 
        color=LINE_COLOR_END;
        if(color!=color_tmp)
        num=0;
        color_tmp=color;
    }

    //中断不画线，不与末端长横冲突
    if(j%(SIZE+SIZE_H)>=SIZE&&j<LINE_H-SIZE_H)
    {
        if(j%(SIZE+SIZE_H)==LINE_H-SIZE_H-1)    //可改到线段起点第一个像素,这段有问题
        size_color=color;
    }
    
    // 画长横线,不得与末端冲突
  else if((j%(SIZE+SIZE_H))>=SIZE-ROW_H && j<LINE_H-ROW_H&&num<ROW_H&&color!=LINE_COLOR_END&&size_color==color)
    {       //  横线起点               不能与末端长横线冲突   一种颜色只能画一次    最后一种颜色不要长横线   防止长横线被截断,一条线段只有同颜色才能画长横线
        for(int i=0;i<ROW_W;i++)
     {
        line_l[i]=color;
        line_r[-i]=color;
     }
     num++;
    }
    //画末尾长横线
    else if(j>=LINE_H-ROW_H)
    {
        for(int i=0;i<ROW_W*2;i++)
     {
        line_l[i]=color;
        line_r[-i]=color;
     }
     
    }
  
    //画正常线
    else  
    {
     for(int i=0;i<LINE_W;i++)
     {
        line_l[i]=color;
        line_r[-i]=color;
     }
    }
     //倾斜度
     if(j%2==0)
     {
        line_l+=1;
        line_r-=1;
     }
     //换行
        line_l-=IMAGE_WIDTH;
        line_r-=IMAGE_WIDTH;
 }
}



//画标志录像的小圆点（双圆环）
void round1()
{
   int r1=4;      //半径内圆
   int r2=6;        //半径外圆
   //坐标
   int x=10;        
   int y=8;
   //做边界保护
   int stat_x=x-r2;
   int stat_y=y-r2;
   int end_x=x+r2;
   int end_y=y+r2;
   if(stat_x<0)
   {stat_x=0;}
   if(end_x>IMAGE_WIDTH)
   {end_x=IMAGE_WIDTH;}
   if(stat_y<0)
   {stat_y=0;}
   if(end_y>IMAGE_HEIGHT)
   {end_y=IMAGE_HEIGHT;}
   for(int i=stat_y;i<=end_y;i++)
   for(int j=stat_x;j<=end_x;j++)
   {
      if((j-x)*(j-x)+(i-y)*(i-y)-r1*r1>=0&&(j-x)*(j-x)+(i-y)*(i-y)-r1*r1<5)
      {
         read_buf[i*IMAGE_WIDTH+j]=0xffff0000;
      }
      //   else if((j-x)*(j-x)+(i-y)*(i-y)<r2*r2)
      if((j-x)*(j-x)+(i-y)*(i-y)-r2*r2>=0&&(j-x)*(j-x)+(i-y)*(i-y)-r2*r2<10)
      {
          read_buf[i*IMAGE_WIDTH+j]=0xffff0000;
      }
   }
}

