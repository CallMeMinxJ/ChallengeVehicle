#ifndef _OLED_H_
#define _OLED_H_

/*ͷ�ļ�����*/
#include "system.h"

/*�궨�岿��*/
//OLED��ز���
#define OLED_CMD    0
#define OLED_DATA   1
//OLEDռ�õ�IO��
#define OLED_DO  	PBout(3)
#define OLED_D1  	PBout(4)
#define OLED_RS	  PBout(5)
#define OLED_DC		PBout(6)   
#define OLED_CS  	PBout(7)

/*������������*/
void OLED_Init(void);
void OLED_WR_Byte( uint8_t dat, uint8_t cmd);                  //дָ��
void OLED_Refresh_Gram(void);//OLED��ʼ��
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
