/**
 * @file      hw_adc.c
 * @brief     ADC外设相关函数
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
#include "hw_adc.h"

/**
 * @brief     ADC1初始化函数
 */
void Adc1_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|
							RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_GPIOC|
							RCC_APB2Periph_ADC1,ENABLE );
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//6分频 72/6<14

	//GPIOA
	GPIO_InitStructure.GPIO_Pin	 = 0XFF;//GPIO_Pin_x(x:0~7)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIOB
	GPIO_InitStructure.GPIO_Pin	 = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIOC
	GPIO_InitStructure.GPIO_Pin	 = 0X3F;//GPIO_Pin_x(x:0~5)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//配置ADC初始化结构体
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode 			= ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel			= 16;
	ADC_Init(ADC1, &ADC_InitStructure);
		
	//ADC1通道配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,	1,	ADC_SampleTime_239Cycles5);	//PA0
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,	2,	ADC_SampleTime_239Cycles5);	//PA1
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,	3,	ADC_SampleTime_239Cycles5);	//PA2
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,	4,	ADC_SampleTime_239Cycles5);	//PA3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,	5,	ADC_SampleTime_239Cycles5);	//PA4
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,	6,	ADC_SampleTime_239Cycles5);	//PA5
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,	7,	ADC_SampleTime_239Cycles5);	//PA6
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,	8,	ADC_SampleTime_239Cycles5);	//PA7
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,	9,	ADC_SampleTime_239Cycles5);	//PB0
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,	10,	ADC_SampleTime_239Cycles5);	//PB1
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,	11,	ADC_SampleTime_239Cycles5);	//PC0
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,	12,	ADC_SampleTime_239Cycles5);	//PC1
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,	13,	ADC_SampleTime_239Cycles5);	//PC2
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,	14,	ADC_SampleTime_239Cycles5);	//PC3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,	15,	ADC_SampleTime_239Cycles5);	//PC4
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,	16,	ADC_SampleTime_239Cycles5);	//PC5
		
	ADC_DMACmd(ADC1,ENABLE);//使能DMA
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/**
 * @brief     获取ADC1某通道的数值
 * @param     Channel - 选择通道（Channelx：x 1~16）
 * @return    uint16_t 
 */
uint16_t Adc1_Get_Value(uint8_t Channel)
{
	ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_239Cycles5 );				    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);
}









