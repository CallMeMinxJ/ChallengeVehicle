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

/**
 * @brief     TIM1 时钟PWM模式初始化
 * @param     ARR - 预装载值
 * @param     PSC - 预分频系数
 * @details	  TIM1启用完全重映射:
 * 			  	TIM1_CH1->PE9 
 * 			  	TIM1_CH2->PE11 
 * 			  	TIM1_CH3->PE13 
 * 			  	TIM1_CH4->PE14
 * 		   	  TIM1设置:
 * 				72分频，频率10KHz，向上计数,1020是为了速度为1000时，
 * 				PWM占空比不会到100%（占空比最大98%）方便电机驱动自举生压。一般PWM频率为10K。
 */
void TIM1_PWM_Init(uint16_t ARR,uint16_t PSC)
{
	//初始化结构体定义
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	//TIM1 GPIO 口初始化配置
	TIM_DeInit(TIM1);//配置初始化结构体缺省值
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); //TIM1完全重映射  TIM1_CH1->PE9 CH2->PE11 CH3->PE13 CH4->PE14 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO 

	//TIM1 初始化结构体配置
	TIM_TimeBaseStructure.TIM_Period = ARR;  //装载值
	TIM_TimeBaseStructure.TIM_Prescaler = PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	//TIM1 通道配置 CH1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能的预装载寄存器
	//CH2
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能的预装载寄存器
	//CH3
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能的预装载寄存器
	//CH4
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能的预装载寄存器

	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能定时器1

	//设置四个通道比较值
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,0);

	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); 
	//启用TIM1
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/**
 * @brief     TIM1中断函数
 */
void TIM1_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

/**
 * @brief     TIM8 时钟PWM模式初始化
 * @param     ARR - 预装载值
 * @param     PSC - 预分频系数
 * @details	  TIM8启用完全重映射:
 * 			  	TIM8_CH1->PC6 
 * 			  	TIM8_CH2->PC7
 * 			  	TIM8_CH3->PC8 
 * 			  	TIM8_CH4->PC9
 * 		   	  TIM8设置:
 * 				72分频，频率10KHz，向上计数,1020是为了速度为1000时，
 * 				PWM占空比不会到100%（占空比最大98%）方便电机驱动自举生压。一般PWM频率为10K。
 */
void TIM8_PWM_Init(uint16_t ARR,uint16_t PSC)
{
	//初始化结构体定义
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//TIM8 GPIO 口初始化配置
	TIM_DeInit(TIM8);//配置初始化结构体缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO 

	//TIM8 初始化结构体配置
	TIM_TimeBaseStructure.TIM_Period = ARR;  //装载值
	TIM_TimeBaseStructure.TIM_Prescaler = PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	//TIM8 通道配置 CH1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能的预装载寄存器
	//CH2
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能的预装载寄存器
	//CH3
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能的预装载寄存器
	//CH4
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC4Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能的预装载寄存器

	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能定时器8

	//设置四个通道比较值
	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare4(TIM8,0);

	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE); 
	//启用TIM8
	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

/**
 * @brief     TIM8中断函数
 */
void TIM8_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	}
}

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
 * @details	  利用该时钟中断进行车辆的数据处理
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









