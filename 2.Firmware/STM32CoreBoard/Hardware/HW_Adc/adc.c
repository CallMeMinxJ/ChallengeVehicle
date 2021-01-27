/**
 * @brief		ADC模数转换相关函数
 * @author	武术擂台车小组
 * @version v0.0.0
 * @date		2021/01/25
 */
 
/*头文件部分*/
#include "system.h"
#include "adc.h"

/**
 * @brief		ADC初始化函数
 * @param		无
 * @return	无
 */
void Adc1_Init(void)
{
	//定义初始化结构体
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIO和ADC1通道时钟 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|
													RCC_APB2Periph_GPIOB|
													RCC_APB2Periph_GPIOC|
													RCC_APB2Periph_ADC1,ENABLE );
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频因子6 72M/6=12(不能超过14M)

	//配置GPIO口
	//GPIOA
	GPIO_InitStructure.GPIO_Pin	 = 0XFF;//GPIO_Pin_x(x:0~7)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIOB
	GPIO_InitStructure.GPIO_Pin	 = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIOC
	GPIO_InitStructure.GPIO_Pin	 = 0X3F;//GPIO_Pin_x(x:0~5)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//配置ADC初始化结构体
	ADC_DeInit(ADC1); 																										//配置缺省值
	ADC_InitStructure.ADC_Mode 								= ADC_Mode_Independent;			//工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode 				= ENABLE;										//多通道模式
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;										//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;//转换由软件触发启动
	ADC_InitStructure.ADC_DataAlign 					= ADC_DataAlign_Right;			//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel 				= 16;												//顺序进行规则
	ADC_Init(ADC1, &ADC_InitStructure);																		//提交初始化参数
		
		//设置ADC1各通道采样周期
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
		
	ADC_DMACmd(ADC1,ENABLE);										//使能ADC的DMA通道
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);				//ADC1中断使能
	ADC_Cmd(ADC1, ENABLE);											//使能指定的ADC1
	ADC_ResetCalibration(ADC1);	 								//使能复位校准 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);									//开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));			//等待校准结束

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//使能指定的ADC1的软件转换启动功能
}
/**
 * @brief		获取ADC1某个通道的数值
 * @param		uint8_t Channel	-	通道名称
 * 					ADC_Channel_x(x: 0~15)
 * @return	无
 */
uint16_t Adc1_Get_Value(uint8_t Channel)
{
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

/**
 * @brief		对ADC采样进行均值滤波
 * @param		无
 * @return	无
 * @todo		将每个引脚的采样进行均值滤波，将结果返回
 */
uint16_t Adc_Average (uint8_t ADC_Channel_Num,uint16_t Times)
{
	return 0;
}









