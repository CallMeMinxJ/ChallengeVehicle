/**
 * @file      hw_led.c
 * @brief     LED相关函数
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
#include "hw_led.h"

/*全局变量部分*/
bool Led_0_Flash_Door = false;
bool Led_1_Flash_Door = false;

/**
 * @brief     LED初始化函数
 */
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/*GPIOB*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//LED初始状态
	Led_0 = Led_Turn_Off;
	Led_1 = Led_Turn_Off;
}

/**
 * @brief     LED显示函数
 * @param     Led_0_Mode - My Param doc
 * @param     Led_1_Mode - My Param doc
 */
void Led_Display(uint8_t Led_0_Mode,uint8_t Led_1_Mode)
{
	//点亮或熄灭
	if 	(Led_0_Mode == Led_Turn_On)			Led_0 = Led_Turn_On;
	else if (Led_0_Mode == Led_Turn_Off)	Led_0 = Led_Turn_Off;
	if	(Led_1_Mode == Led_Turn_On)			Led_1 = Led_Turn_On;
	else if (Led_1_Mode == Led_Turn_Off)	Led_1 = Led_Turn_Off;
	
	//LED闪烁
	if (Led_0_Mode == Led_Flash)Led_0_Flash_Door = true;
	else	Led_0_Flash_Door = false;
	if (Led_1_Mode == Led_Flash)Led_1_Flash_Door = true;
	else	Led_1_Flash_Door = false;
}

/**
 * @brief     LED闪烁控制函数
 * @param     Half_Cycle - My Param doc
 */
void Led_Flashing(const uint16_t Half_Cycle)
{
	static uint16_t Led_Flash_Time_ms = 0;

	//Led闪烁
	if (Led_0_Flash_Door || Led_1_Flash_Door)
	{
		//flash 半周期 Half_Cycle ms
		if(Led_Flash_Time_ms<Half_Cycle)Led_Flash_Time_ms++;
		else 
		{
			Led_Flash_Time_ms=0;
			if(Led_0_Flash_Door)Led_0 = !Led_0;
			if(Led_1_Flash_Door)Led_1 = !Led_1;
		}
	}

}
 




