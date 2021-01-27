/**
 * @brief		ADCģ��ת����غ���
 * @author	������̨��С��
 * @version v0.0.0
 * @date		2021/01/25
 */
 
/*ͷ�ļ�����*/
#include "system.h"
#include "hw_adc.h"

/**
 * @brief		ADC��ʼ������
 * @param		��
 * @return	��
 */
void Adc1_Init(void)
{
	//�����ʼ���ṹ��
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��GPIO��ADC1ͨ��ʱ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|
													RCC_APB2Periph_GPIOB|
													RCC_APB2Periph_GPIOC|
													RCC_APB2Periph_ADC1,ENABLE );
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//��Ƶ����6 72M/6=12(���ܳ���14M)

	//����GPIO��
	//GPIOA
	GPIO_InitStructure.GPIO_Pin	 = 0XFF;//GPIO_Pin_x(x:0~7)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIOB
	GPIO_InitStructure.GPIO_Pin	 = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIOC
	GPIO_InitStructure.GPIO_Pin	 = 0X3F;//GPIO_Pin_x(x:0~5)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//����ADC��ʼ���ṹ��
	ADC_DeInit(ADC1); 																										//����ȱʡֵ
	ADC_InitStructure.ADC_Mode 								= ADC_Mode_Independent;			//�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode 				= ENABLE;										//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;										//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;//ת����������������
	ADC_InitStructure.ADC_DataAlign 					= ADC_DataAlign_Right;			//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel 				= 16;												//˳����й���
	ADC_Init(ADC1, &ADC_InitStructure);																		//�ύ��ʼ������
		
		//����ADC1��ͨ����������
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
		
	ADC_DMACmd(ADC1,ENABLE);										//ʹ��ADC��DMAͨ��
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);				//ADC1�ж�ʹ��
	ADC_Cmd(ADC1, ENABLE);											//ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);	 								//ʹ�ܸ�λУ׼ 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);									//����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));			//�ȴ�У׼����

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//ʹ��ָ����ADC1������ת����������
}
/**
 * @brief		��ȡADC1ĳ��ͨ������ֵ
 * @param		uint8_t Channel	-	ͨ������
 * 					ADC_Channel_x(x: 0~15)
 * @return	��
 */
uint16_t Adc1_Get_Value(uint8_t Channel)
{
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

/**
 * @brief		��ADC�������о�ֵ�˲�
 * @param		��
 * @return	��
 * @todo		��ÿ�����ŵĲ������о�ֵ�˲������������
 */
uint16_t Adc_Average (uint8_t ADC_Channel_Num,uint16_t Times)
{
	return 0;
}









