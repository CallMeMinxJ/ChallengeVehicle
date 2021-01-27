/**
 * @file      menu.c
 * @brief     包含菜单显示相关函数
 * @author    MINXJ (CallMeMinxJ@outlook.com)
 * @version   0.1
 * @date      2021-01-27
 * 
 * @copyright Copyright (c) CAUC
 * 
 */
 
/*头文件部分*/
 #include "system.h"
 #include "menu.h"
 #include "hw_oled.h"
 #include "hw_adc.h"
  
/*全局变量部分*/
extern uint16_t Adc1_Buff[16];

/**
 * @brief     Adc1菜单部分
 * @todo      只是为了测试模块，需要改进
 */
void Menu_Adc1_Page(void)
{
	//����
	OLED_ShowString(0,0,"ADC:",12);
	
	//��һ��
	OLED_ShowString(0,12,"1-4:",12);
	OLED_ShowNumber(24,12,(int32_t) Adc1_Buff[0],12);
	OLED_ShowNumber(50,12,(int32_t) Adc1_Buff[1],12);
	OLED_ShowNumber(76,12,(int32_t) Adc1_Buff[2],12);
	OLED_ShowNumber(100,12,(int32_t) Adc1_Buff[3],12);
	
	//��һ��
	OLED_ShowString(0,24,"5-8:",12);
	OLED_ShowNumber(24,24,(int32_t) Adc1_Buff[4],12);
	OLED_ShowNumber(50,24,(int32_t) Adc1_Buff[5],12);
	OLED_ShowNumber(76,24,(int32_t) Adc1_Buff[6],12);
	OLED_ShowNumber(100,24,(int32_t) Adc1_Buff[7],12);
	
	//��һ��
	OLED_ShowString(0,36,"9- :",12);
	OLED_ShowNumber(24,36,(int32_t) Adc1_Buff[8],12);
	OLED_ShowNumber(50,36,(int32_t) Adc1_Buff[9],12);
	OLED_ShowNumber(76,36,(int32_t) Adc1_Buff[10],12);
	OLED_ShowNumber(100,36,(int32_t) Adc1_Buff[11],12);
	
	//��һ��
	OLED_ShowString(0,48,"-16:",12);
	OLED_ShowNumber(24,48,(int32_t) Adc1_Buff[12],12);
	OLED_ShowNumber(50,48,(int32_t) Adc1_Buff[13],12);
	OLED_ShowNumber(76,48,(int32_t) Adc1_Buff[14],12);
	OLED_ShowNumber(100,48,(int32_t) Adc1_Buff[15],12);
	
	OLED_Refresh_Gram();
}
 


