#ifndef _LED_H_
#define _LED_H_

/*ͷ�ļ�����*/
#include "system.h"

/*�궨�岿��*/
//LED״̬ �͵�ƽ��Ч
#define Led_Turn_On		0
#define Led_Turn_Off	1
#define Led_Flash			2
//LED�˿ڶ���
#define Led_0 PBout(13)
#define Led_1 PBout(12)

/*������������*/
void Led_Init(void);
void Led_Display(uint8_t Led_0_Mode,uint8_t Led_1_Mode);

#endif





