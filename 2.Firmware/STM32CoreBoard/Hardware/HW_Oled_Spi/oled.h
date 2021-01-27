#ifndef _OLED_H_
#define _OLED_H_

/*头文件部分*/
#include "system.h"

/*宏定义部分*/
//OLED相关参数
#define OLED_CMD    0
#define OLED_DATA   1
//OLED占用的IO口
#define OLED_DO  	PBout(3)
#define OLED_D1  	PBout(4)
#define OLED_RS	  PBout(5)
#define OLED_DC		PBout(6)   
#define OLED_CS  	PBout(7)

/*函数声明部分*/
void OLED_Init(void);
void OLED_WR_Byte( uint8_t dat, uint8_t cmd);                  //写指令
void OLED_Refresh_Gram(void);//OLED初始化
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(uint8_t mode);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,u8 size);
void OLED_ShowNumber(uint8_t x,uint8_t y,int32_t number,uint8_t size);
void OLED_Show_Logo(void);
void OLED_Show_Icon(uint8_t list,uint8_t x,uint8_t y,uint8_t size,uint8_t mode);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawRectangular(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t size,uint8_t num,uint8_t mode);


#endif
