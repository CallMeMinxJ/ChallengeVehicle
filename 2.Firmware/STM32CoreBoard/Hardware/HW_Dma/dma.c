/**
 * @brief		包含核心板用到的DMA相关函数
 * @author	武术擂台车小组
 * @global	uint16_t Adc1_Buff[16]	-	存放ADC1 16个端口转换结果
 * @version v0.0.0
 * @date		2021/01/27
 */
 
/*头文件部分*/
#include "system.h"
#include "dma.h"

/*全局变量部分*/
uint16_t Adc1_Buff[16]={0};

/**
 * @brief		ADC->[全局变量]DMA通道初始化
 * @param		无
 * @return	无
 */
void Dma_Adc1_To_GlobalVar(void)
{
	//定义初始化结构体
	DMA_InitTypeDef DMA_Struct;
	
	//使能DMA片内时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//配置结构体
	DMA_StructInit(&DMA_Struct);
	DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; 
	DMA_Struct.DMA_MemoryBaseAddr     = (uint32_t)Adc1_Buff;
	DMA_Struct.DMA_BufferSize         = 16;
	DMA_Struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
	DMA_Struct.DMA_Mode               = DMA_Mode_Circular;
	DMA_Struct.DMA_Priority           = DMA_Priority_High;
	
	//提交配置结构体
	DMA_Init(DMA1_Channel1, &DMA_Struct);
	//中断使能
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	//使能DMA通道1
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

