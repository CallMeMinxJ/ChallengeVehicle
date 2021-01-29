/**
 * @file      hw_inf&gray.c
 * @brief     红外测距和灰度模块的相关函数
 * @author    MINXJ (CallMeMinxJ@outlook.com)
 * @version   0.1
 * @date      2021-01-29
 * 
 * @copyright Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 *            Everyone is permitted to copy and distribute verbatim copies
 *            of this license document, but changing it is not allowed.
 * 
 */

/*头文件部分*/
#include "system.h"
#include "hw_inf&gray.h"
#include "hw_adc.h"
#include "hw_dma.h"

/**
 * @brief     红外模块 和 灰度模块 初始化函数
 */
void Inf_And_Gray_Init(void)
{
    Dma_Adc1_To_GlobalVar();	//DMA模块初始化
	Adc1_Init();//ADC1模块初始化
}





