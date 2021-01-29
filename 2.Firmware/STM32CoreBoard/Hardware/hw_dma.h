#ifndef _DMA_H_
#define _DMA_H_

/*头文件部分*/
#include "system.h"

/*宏定义部分*/
#define ADC1_BUFF_LEN 16
#define USART3_RX_LEN (84*5)

/*函数声明部分*/
void Dma_Adc1_To_GlobalVar(void);
void Dma_Usart3_To_GlobalVar(void);

#endif



