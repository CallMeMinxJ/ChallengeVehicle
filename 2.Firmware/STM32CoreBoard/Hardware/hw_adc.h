#ifndef _ADC_H_
#define _ADC_H_

/*头文件部分*/
#include "system.h"

/*函数初始化部分*/
void Adc1_Init(void);
uint16_t Adc1_Get_Value(uint8_t Channel);

#endif
