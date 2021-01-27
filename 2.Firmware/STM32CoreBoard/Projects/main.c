/**
 * @file      main.c
 * @brief     工程的主函数文件
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
#include "hw_oled.h"
#include "menu.h"

/**
 * @brief 程序入口 主函数
 * @return int 
 */
int main (void)
{
	/*初始化部分*/
	Delay_Init();//延迟函数初始化
	Nvic_Init();//中断配置
	JTAGDisable();//禁用Jtag
	Car_Init();//车辆总体初始化

	while(1)
	{
		//按键检测及功能实现
		Key_Function(Key_Scan(Mode_Ucnt));
		Menu_Adc1_Page();
	}
}

