#ifndef _USART_H_
#define _USART_H_

/*头文件部分*/
#include "system.h"

/*函数声明部分*/
void Usart2_Init(uint32_t BaudRate);
void Usart_Send_Data (USART_TypeDef * UsartX, uint16_t Data);
void Usart_Send_Data_Buff (USART_TypeDef * UsartX, uint16_t * Data_Buff, uint8_t Data_Count);

#endif

