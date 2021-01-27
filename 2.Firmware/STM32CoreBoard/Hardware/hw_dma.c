/**
 * @brief		�������İ��õ���DMA��غ���
 * @author	������̨��С��
 * @global	uint16_t Adc1_Buff[16]	-	���ADC1 16���˿�ת�����
 * @version v0.0.0
 * @date		2021/01/27
 */
 
/*ͷ�ļ�����*/
#include "system.h"
#include "hw_dma.h"

/*ȫ�ֱ�������*/
uint16_t Adc1_Buff[16]={0};

/**
 * @brief		ADC->[ȫ�ֱ���]DMAͨ����ʼ��
 * @param		��
 * @return	��
 */
void Dma_Adc1_To_GlobalVar(void)
{
	//�����ʼ���ṹ��
	DMA_InitTypeDef DMA_Struct;
	
	//ʹ��DMAƬ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//���ýṹ��
	DMA_StructInit(&DMA_Struct);
	DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; 
	DMA_Struct.DMA_MemoryBaseAddr     = (uint32_t)Adc1_Buff;
	DMA_Struct.DMA_BufferSize         = 16;
	DMA_Struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
	DMA_Struct.DMA_Mode               = DMA_Mode_Circular;
	DMA_Struct.DMA_Priority           = DMA_Priority_High;
	
	//�ύ���ýṹ��
	DMA_Init(DMA1_Channel1, &DMA_Struct);
	//�ж�ʹ��
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	//ʹ��DMAͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

