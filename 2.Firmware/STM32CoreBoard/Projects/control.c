/**
 * @file      control.c
 * @brief     车辆控制相关函数
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
#include "control.h"
#include "hw_key.h"
#include "hw_led.h"
#include "hw_oled.h"
#include "hw_timer.h"
#include "hw_adc.h"
#include "hw_dma.h"
#include "hw_rplidar.h"
#include "hw_inf&gray.h"

/*全局变量部分*/
extern uint16_t Adc1_Buff[ADC1_BUFF_LEN];
extern uint16_t Adc1_Real[ADC1_BUFF_LEN];

/**
 * @brief     车辆总体初始化函数
 */
void Car_Init(void)
{
	OLED_Init();//OLED模块初始化
	Key_Init();	//按键模块初始化
	Led_Init();	//LED模块初始化
	TIM1_PWM_Init(1050-1,72-1);	//时钟模块初始化
	TIM3_Init(1000-1,72-1);		//时钟模块初始化
	TIM2_Init(1000-1,720-1);	//时钟模块初始化
	Inf_And_Gray_Init();		//红外灰度模块初始化
	Rplidar_Init();				//激光雷达模块初始化
}

/**
 * @brief     车辆整体中断 数据处理函数
 */
void Car_Data_Processing(void)
{
	//对ADC数据进行均值滤波
	First_Order_Complementary_Filtering(Adc1_Buff , Adc1_Real ,  0.88, ADC1_BUFF_LEN);
	
}

/**
 * @brief     一阶互补滤波函数
 * @param     Pending_Data - 待处理数据首地址
 * @param     Processed_Data - 处理后数据首地址
 * @param     a - 滤波系数 : 0~1 需要调试
 * @param     Data_Count - 数据总数
 */
void First_Order_Complementary_Filtering (uint16_t * Pending_Data , uint16_t * Processed_Data , double a , uint8_t Data_Count)
{
	uint8_t cnt;
	for(cnt = 0; cnt < Data_Count; cnt++)
		Processed_Data[cnt] =(uint16_t) (a * Processed_Data[cnt] + (1 - a) * Pending_Data[cnt]);
}


