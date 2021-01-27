#ifndef _ADC_H_
#define _ADC_H_

/*头文件部分*/
#include "system.h"

/*函数声明部分*/
void Adc1_Init(void);
uint16_t Adc1_Get_Value(uint8_t Channel);
uint16_t Adc_Average (uint8_t ADC_Channel_Num,uint16_t Times);

#endif
