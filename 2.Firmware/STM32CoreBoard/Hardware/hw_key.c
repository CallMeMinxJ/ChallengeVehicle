/**
 * @file      hw_key.c
 * @brief     按键功能实现相关函数
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
 #include "hw_key.h"
 #include "hw_led.h"
 
/**
 * @brief     按键GPIO初始化
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	/*GPIOD*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

/**
 * @brief     按键检测函数
 * @param     mode - 检测模式
 * 					 Mode_cnt	- 连续监测
 * 					 Mode_Ucnt	- 非连续监测
 * @return    uint8_t 返回按下按键编码
 */
uint8_t Key_Scan(uint8_t mode)
{
	static int8_t KEY_STATE = 1;//状态标志
    int8_t cnt   = 0,//防止误触
           test1 = 0,//检测1
           test2 = 0;//检测2
    if (mode == Mode_cnt)   KEY_STATE = 1;
    
    //初次检测
    if (Key_0)	test1 = Key_0_Num	,cnt++;
    if (Key_1)	test1 = Key_1_Num	,cnt++;
    if (Key_2)	test1 = Key_2_Num	,cnt++;
    
    if (cnt==1 && KEY_STATE)
    {
        cnt       = 0;
        KEY_STATE = 0;//判断状态
        Delay_ms(Key_Debounce_Time);//延迟消抖
        
        //第二次检测
        if (Key_0)  test2 = Key_0_Num	,cnt++;
        if (Key_1)	test2 = Key_1_Num	,cnt++;
        if (Key_2)	test2 = Key_2_Num	,cnt++;
        if (cnt==1 && test1 == test2) return test2;
    }
    else
        if (!cnt)  KEY_STATE = 1;//更新状态
    return  No_Key_Num;
}

/**
 * @brief     按键功能实现
 * @param     Key_Num - 按下的按键编码
 */
void Key_Function(uint8_t Key_Num)
{
	switch(Key_Num)
	{
		case Key_0_Num:
		{
			Led_Display(Led_Turn_On,Led_Turn_Off);
			break;
		}
		
		case Key_1_Num:
		{
			Led_Display(Led_Turn_Off,Led_Turn_On);
			break;
		}
		
		case Key_2_Num:
		{
			Led_Display(Led_Flash,Led_Turn_Off);
			break;
		}
	}
}























 
 

