/**
 * @file      control.c
 * @brief     车辆控制相关函数
 * @author    MINXJ (CallMeMinxJ@outlook.com)
 * @version   0.1
 * @date      2021-01-27
 * 
 * @copyright Copyright (c) CAUC
 * 
 */
 
 /*ͷ�ļ�����*/
#include "system.h"
#include "control.h"
#include "key.h"
#include "led.h"
#include "oled.h"
#include "timer.h"
#include "adc.h"
#include "dma.h"

/**
 * @brief     车辆总体初始化函数
 */
void Car_Init(void)
{
	OLED_Init();//OLED模块初始化
	Key_Init();	//按键模块初始化
	Led_Init();	//LED模块初始化
	TIM3_Init(1000-1,72-1);	//时钟模块初始化
	Dma_Adc1_To_GlobalVar();//DMA模块初始化
	Adc1_Init();//ADC1模块初始化
}
 


