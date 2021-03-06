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
//ADC数据部分
extern uint16_t Adc1_Buff[ADC1_BUFF_LEN];
extern uint16_t Adc1_Real[ADC1_BUFF_LEN];
//激光雷达数据部分
extern uint16_t G_Rplidar_Target_Distance;
extern uint16_t G_Rplidar_Target_Angle;
extern bool     G_Rplidar_Target_Is_Catched;

/**
 * @brief     车辆总体初始化函数
 */
void Car_Init(void)
{
	OLED_Init();//OLED模块初始化
	Key_Init();	//按键模块初始化
	Led_Init();	//LED模块初始化
	TIM1_PWM_Init(1050-1,72-1);	//时钟模块初始化
	TIM8_PWM_Init(1050-1,72-1);	//时钟模块初始化
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
	//对ADC数据进行互补滤波
	First_Order_Complementary_Filtering(Adc1_Buff , Adc1_Real ,  0.88, ADC1_BUFF_LEN);
	//小车驶向目标
	Car_Crush_Target(G_Rplidar_Target_Is_Catched, G_Rplidar_Target_Angle, G_Rplidar_Target_Distance);
	
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

/**
 * @brief     小车驶向目标函数
 * @param     Rplidar_Target_Is_Catched - 是否捕获到目标
 * @param     Rplidar_Target_Angle - 目标所在角度
 * @param     Rplidar_Target_Distance - 到目标的距离
 */
void Car_Crush_Target(bool Rplidar_Target_Is_Catched, uint16_t Rplidar_Target_Angle, uint16_t Rplidar_Target_Distance)
{
	if (Rplidar_Target_Is_Catched)
	{
		if(Rplidar_Target_Angle>90 && Rplidar_Target_Angle<225)
			Car_Turn_Right(100);
		else if(Rplidar_Target_Angle<90 && Rplidar_Target_Angle>315)
			Car_Turn_Left(100);
	}
}

/**
 * @brief     控制小车轮子左转
 * @param     CCR - PWM的比较值控制占空比
 */
void Car_Turn_Left(uint16_t CCR)
{
	TIM_SetCompare1(TIM1,CCR);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,CCR);
	TIM_SetCompare4(TIM1,0);

	TIM_SetCompare1(TIM8,CCR);
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,CCR);
	TIM_SetCompare4(TIM8,0);
}

/**
 * @brief     控制小车轮子右转
 * @param     CCR - PWM的比较值控制占空比
 */
void Car_Turn_Right(uint16_t CCR)
{
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare2(TIM1,CCR);
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,CCR);

	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,CCR);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare4(TIM8,CCR);
}

/**
 * @brief     控制小车轮子前进
 * @param     CCR - PWM的比较值控制占空比
 */
void Car_Running(uint16_t CCR)
{
	TIM_SetCompare1(TIM1,CCR);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,CCR);
	TIM_SetCompare4(TIM1,0);

	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,CCR);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare4(TIM8,CCR);
}

/**
 * @brief     控制小车轮子后退
 * @param     CCR - PWM的比较值控制占空比
 */
void Car_Backing(uint16_t CCR)
{
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare2(TIM1,CCR);
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,CCR);

	TIM_SetCompare1(TIM8,CCR);
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,CCR);
	TIM_SetCompare4(TIM8,0);
}

/**
 * @brief     控制小车轮子制动
 * @param     CCR - PWM的比较值控制占空比
 */
void Car_Stop(uint16_t CCR)
{
	TIM_SetCompare1(TIM1,CCR);
	TIM_SetCompare2(TIM1,CCR);
	TIM_SetCompare3(TIM1,CCR);
	TIM_SetCompare4(TIM1,CCR);

	TIM_SetCompare1(TIM8,CCR);
	TIM_SetCompare2(TIM8,CCR);
	TIM_SetCompare3(TIM8,CCR);
	TIM_SetCompare4(TIM8,CCR);
}





