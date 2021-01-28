/**
 * @file      hw_timer.c
 * @brief     计时器模块相关函数
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
#include "hw_timer.h"
#include "hw_led.h"

/*全局变量部分*/
extern bool Led_0_Flash_Door;
extern bool Led_1_Flash_Door;

/**
 * @brief     TIM3时钟初始化函数
 * @param     ARR - 装载值
 * @param     PSC - 分频系数
 */
void TIM3_Init(uint16_t ARR,uint16_t PSC)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//初始化时中配置
	TIM_TimeBaseStructure.TIM_Period = ARR; 
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 
	TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief     TIM3中断函数
 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
		
		//Led闪烁
		Led_Flashing(500);
	}
}

/**
 * @brief     TIM2时钟初始化函数
 * @param     ARR - 装载值
 * @param     PSC - 分频系数
 * @detail	  利用该时钟中断进行车辆的数据处理
 */
void TIM2_Init(uint16_t ARR,uint16_t PSC)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//初始化时中配置
	TIM_TimeBaseStructure.TIM_Period = ARR; 
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 
	TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief     TIM2中断函数
 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );
		
		//车辆中断 数据处理
		Car_Data_Processing();
	}
}









