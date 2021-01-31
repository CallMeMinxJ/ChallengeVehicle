/**
 * @file      hw_oled.c
 * @brief     OLED模块初始化部分
 * @author    MINXJ (CallMeMinxJ@outlook.com)
 * @version   0.1
 * @date      2021-01-28
 * 
 * @copyright Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 *            Everyone is permitted to copy and distribute verbatim copies
 *            of this license document, but changing it is not allowed.
 * 
 */
 
/*头文件部分*/
#include <stdlib.h>
#include <math.h>
#include "system.h"
#include "hw_oled.h"
#include "hw_oledfont.h"

/*全局变量部分*/
uint8_t OLED_GRAM[128][8];	//�Դ�

/**
 * @brief     写 命令/数据 函数
 * @param     dat - 写入的 命令/数据
 * @param     cmd - 写入模式
 * 					cmd = 1:命令
 * 					cmd = 0:数据
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
 * @brief     OLED软刷新
 */
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

/**
 * @brief     OLED开启显示
 */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
 * @brief     OLED关闭显示
 */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 


/**
 * @brief     OLED清屏函数
 * @param     mode - 清屏方式
 * 					 mode：1 正常
 * 					 mode: 0 反白
 */
void OLED_Clear(uint8_t mode)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	if (mode)
		OLED_Refresh_Gram();
}

/**
 * @brief     OLED画点函数
 * @param     x - x轴坐标
 * @param     y - y轴坐标
 * @param     t - 1：正常 0：反白
 */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

/**
 * @brief     OLED画直线函数
 * @param     x1 - A点x坐标
 * @param     y1 - A点y坐标
 * @param     x2 - B点x坐标
 * @param     y2 - B点y坐标
 * @param     mode - 画线模式 1:正常 0：反白
 */
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	int16_t 
		delta_x = x2-x1,
		delta_y = y2-y1,
		cpr_x = 0,
		cpr_y = 0;
	
	uint8_t 
		n = 0,
		x = x1,	
		y = y1,
		
		distance = abs(delta_x)>=abs(delta_y)?abs(delta_x):abs(delta_y);	
	
	if(delta_x == 0)
	{
		for(n=0;n<abs(y2-y1)+1;n++)
		{
			OLED_DrawPoint(x,y,mode);
			y+=(y2>=y1)?1:-1;
		}
	}
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
 * @brief     OLED画矩形函数
 * @param     x1 - A点x坐标
 * @param     y1 - A点y坐标
 * @param     x2 - B点x坐标
 * @param     y2 - B点y坐标
 * @param     mode - 画线模式 1:正常 0：反白
 */
void OLED_DrawRectangular(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	OLED_DrawLine(x1,y1,x2,y1,mode);
	OLED_DrawLine(x2,y1,x2,y2,mode);
	OLED_DrawLine(x2,y2,x1,y2,mode);
	OLED_DrawLine(x1,y2,x1,y1,mode);
}

/**
 * @brief     OLED显示字符函数
 * @param     x - x轴坐标
 * @param     y - y轴坐标
 * @param     chr - 字符常量
 * @param     size - 字体大小 12/16/24
 * @param     mode - 模式 1:正常 0：反白
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);	
	chr=chr-' '; 
    for(t=0;t<csize;t++)
    {   
			if(size==12)temp=asc2_1206[chr][t]; 	 	//1206
			else if(size==16)temp=asc2_1608[chr][t];	//1608
			else if(size==24)temp=asc2_2412[chr][t];	//2412
			else return;
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
 * @brief     显示字符串函数
 * @param     x - x坐标
 * @param     y - y坐标
 * @param     p - 字符串常量
 * @param     size - 字号 12/16/24
 */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear(1);}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
}

/**
 * @brief     显示数字函数
 * @param     x - x坐标
 * @param     y - y坐标
 * @param     number - 显示的数字
 * @param     size - 字号 12/16/24
 */
void OLED_ShowNumber(uint8_t x,uint8_t y,int32_t number,uint8_t size)
{	
	char *p;
	//数字转换成字符串
	p = Num_To_String(number,p,10);
	while((*p<='9')&&(*p>='0'))//判断是否合法
	{       
			if(x>(128-(size/2))){x=0;y+=size;}
			if(y>(64-size)){y=x=0;OLED_Clear(1);}
			OLED_ShowChar(x,y,*p,size,1);	 
			x+=size/2;
			p++;
	}  
}

/**
 * @brief     OLED初始化函数
 */
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*GPIOB*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//IO口初始状态
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);

	/*OLED准备开启*/
	OLED_RS=1;
	Delay_ms(100);
	OLED_RS=0;
	Delay_ms(100);
	OLED_RS=1;
	
	//用通信代码配置OLED显示模式
	OLED_WR_Byte(0xAE,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x81,OLED_CMD);
	OLED_WR_Byte(0xCF,OLED_CMD);
	OLED_WR_Byte(0xA1,OLED_CMD);
	OLED_WR_Byte(0xC0,OLED_CMD);//0xc8 0xc0 屏幕颠倒选另一个
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





