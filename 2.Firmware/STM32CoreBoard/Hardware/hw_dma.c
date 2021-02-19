/**
 * @file      hw_dma.c
 * @brief     DMA通道相关函数
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
#include "system.h"
#include "hw_dma.h"

/*全局变量部分*/
//ADC1 GlobalVar
uint16_t Adc1_Buff[16]={0};
uint16_t Adc1_Real[16]={0};
//USART3 GlobalVar
uint8_t	Usart2_Buff[USART2_RX_LEN] = {0};

/**
 * @brief     DMA ADC1->Adc1_Buff 的通道配置
 */
void Dma_Adc1_To_GlobalVar(void)
{
	DMA_InitTypeDef DMA_Struct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//配置初始化结构体
	DMA_StructInit(&DMA_Struct);
	DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; 
	DMA_Struct.DMA_MemoryBaseAddr     = (uint32_t)Adc1_Buff;
	DMA_Struct.DMA_BufferSize         = ADC1_BUFF_LEN;
	DMA_Struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
	DMA_Struct.DMA_Mode               = DMA_Mode_Circular;
	DMA_Struct.DMA_Priority           = DMA_Priority_High;
	DMA_Init(DMA1_Channel1, &DMA_Struct);
	DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1,ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

/**
 * @brief     DMA USART3->Adc1_Buff 的通道配置
 */
void Dma_Usart2_To_GlobalVar(void)
{
	DMA_InitTypeDef DMA_Struct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//配置初始化结构体
	DMA_StructInit(&DMA_Struct);
	DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR; 
	DMA_Struct.DMA_MemoryBaseAddr     = (uint32_t)Usart2_Buff;
	DMA_Struct.DMA_BufferSize         = 7;
	DMA_Struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_Struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_Struct.DMA_Mode               = DMA_Mode_Normal;
	DMA_Struct.DMA_Priority           = DMA_Priority_High;
	DMA_Init(DMA1_Channel6, &DMA_Struct);
	DMA_ITConfig(DMA1_Channel6,DMA1_IT_TC1,ENABLE);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

