/**
 * @brief		OLED相关函数
 * @author	武术擂台车小组
 * @version v0.0.0
 * @date		2021/01/23
 * @attention
 *					该文件使用了数字转字符串函数，函数本体在system.c中
 */
 
/*头文件部分*/
#include "stdlib.h"
#include <math.h>
#include "system.h"
#include "oled.h"
#include "oledfont.h"

/*全局变量部分*/
uint8_t OLED_GRAM[128][8];	//显存

/**
 * @brief		向OLED写命令或数据
 * @param		dat：指令内容 cmd：写命令/写数据
 * @return	无
 */
void OLED_WR_Byte( uint8_t dat, uint8_t cmd)
{
  uint8_t i;
  if(cmd)
    OLED_DC=1;
  else
    OLED_DC=0;
  for(i=0;i<8;i++)
  {
    OLED_DO=0;
    if(dat&0x80)
       OLED_D1=1;
    else
       OLED_D1=0;
    OLED_DO=1;
    dat<<=1;
  }
  OLED_DC=1;
}

/**
 * @brief		软清屏，刷新显存内容
 * @param		无
 * @return	无
 */
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

/**
 * @brief		打开OLED显示
 * @param		无
 * @return	无
 */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
 * @brief		关闭OLED显示
 * @param		无
 * @return	无
 */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 


/**
 * @brief		OLED清屏函数，清屏后全黑
 * @param		mode：1/0 清屏后刷新/不刷新
 * @return	无
 */
void OLED_Clear(uint8_t mode)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	if (mode)
		OLED_Refresh_Gram();//更新显示
}

/**
 * @brief		功能：在屏幕一个位置上画点
 * @param		x:点横坐标 y：点纵坐标 t：显示方式 白/黑
 * @return	无
 */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

/**
 * @brief		功能：画一条直线
 * @param		x1,y1:第一个点坐标 x2,y2:第二个点坐标 mode：1/0 正常/消除
 * @return	无
 */
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	int16_t 
		delta_x = x2-x1,		//差值
		delta_y = y2-y1,
		cpr_x = 0,					//比较值
		cpr_y = 0;
	
	uint8_t 
		n = 0,
		x = x1,							//当前坐标
		y = y1,
												//记录长边
		distance = abs(delta_x)>=abs(delta_y)?abs(delta_x):abs(delta_y);	
	
	//垂线
	if(delta_x == 0)
	{
		for(n=0;n<abs(y2-y1)+1;n++)
		{
			OLED_DrawPoint(x,y,mode);
			y+=(y2>=y1)?1:-1;
		}
	}
	//非垂线 描点
	else
	{
		for(n=0;n<=distance+1;n++)
		{
			OLED_DrawPoint(x,y,mode);
			
			cpr_x+=delta_x;
			cpr_y+=delta_y;
			
			if(abs(cpr_x)>distance)
			{
				cpr_x=(delta_x>=0)?(cpr_x-distance):(cpr_x+distance);
				x=(delta_x>=0)?(x+1):(x-1);
			}
			
			if(abs(cpr_y)>distance)
			{
				cpr_y=(delta_y>=0)?(cpr_y-distance):(cpr_y+distance);
				y=(delta_y>=0)?(y+1):(y-1);
			}
		}
	}
}

/**
 * @brief		功能：画一个矩形
 * @param		x1,y1:起点坐标 y1,y2：终点坐标
 * @return	无
 */
void OLED_DrawRectangular(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	OLED_DrawLine(x1,y1,x2,y1,mode);
	OLED_DrawLine(x2,y1,x2,y2,mode);
	OLED_DrawLine(x2,y2,x1,y2,mode);
	OLED_DrawLine(x1,y2,x1,y1,mode);
}

/**
 * @brief		功能：在屏幕上一个位置显示一个字符
 * @param		x:横坐标 y：纵坐标 size：字号 12/16/24 mode：0反白显示 1正常显示
 * @return	无
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
			if(size==12)temp=asc2_1206[chr][t]; 	 		//调用1206字体
			else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
			else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
			else return;															//没有的字库
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
    }          
}

/**
 * @brief		功能：在固定位置显示字符串
 * @param		x:横坐标 y：纵坐标 p：需要显示的字符串 size:字号 12/16/32
 * @return	无
 */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear(1);}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
}

/**
 * @brief		功能：在固定位置显示数字
 * @param		x:横坐标 y：纵坐标 p：需要显示的字符串 size:字号 12/16/32
 * @return	无
 */
void OLED_ShowNumber(uint8_t x,uint8_t y,int32_t number,uint8_t size)
{	
	char *p;
	p = Num_To_String(number,p,10);
	while((*p<='9')&&(*p>='0'))//判断是不是非法字符
	{       
			if(x>(128-(size/2))){x=0;y+=size;}
			if(y>(64-size)){y=x=0;OLED_Clear(1);}
			OLED_ShowChar(x,y,*p,size,1);	 
			x+=size/2;
			p++;
	}  
}

/**
 * @brief		功能：初始化OLED
 * @param		无
 * @return	无
 */
void OLED_Init(void)
{
	//定义GPIO初始化结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//使能对应引脚外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*GPIOB*/
	//为初始化结构体赋缺省值
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;//推挽输出
	//读取初始化结构体的值
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//IO初始化完成后设置默认电平
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);

	/*以下是与OLED芯片通信代码，不必理解*/
	OLED_RS=1;
	Delay_ms(100);
	OLED_RS=0;
	Delay_ms(100);
	OLED_RS=1;
	
	//指令设置显示模式
	OLED_WR_Byte(0xAE,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x81,OLED_CMD);
	OLED_WR_Byte(0xCF,OLED_CMD);
	OLED_WR_Byte(0xA1,OLED_CMD);
	OLED_WR_Byte(0xC0,OLED_CMD);//0xc8 0xc0 屏幕颠倒换成另一个
	OLED_WR_Byte(0xA8,OLED_CMD);
	OLED_WR_Byte(0x3f,OLED_CMD);
	OLED_WR_Byte(0xD3,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xd5,OLED_CMD);
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD);
	OLED_WR_Byte(0xF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD);
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x20,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xA4,OLED_CMD);
	OLED_WR_Byte(0xA6,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD);

	OLED_Clear(1);
}





