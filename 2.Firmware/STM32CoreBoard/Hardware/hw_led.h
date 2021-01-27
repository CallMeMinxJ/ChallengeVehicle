#ifndef _LED_H_
#define _LED_H_

/*头文件部分*/
#include "system.h"

/*宏定义部分*/
//LED状态 低电平有效
#define Led_Turn_On		0
#define Led_Turn_Off	1
#define Led_Flash			2
//LED端口定义
#define Led_0 PBout(13)
#define Led_1 PBout(12)

/*函数声明部分*/
void Led_Init(void);
void Led_Display(uint8_t Led_0_Mode,uint8_t Led_1_Mode);

#endif





