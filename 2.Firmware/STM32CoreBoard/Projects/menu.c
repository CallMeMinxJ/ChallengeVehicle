/**
 * @file      menu.c
 * @brief     菜单相关函数
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
 #include "menu.h"
 #include "hw_oled.h"
 #include "hw_adc.h"
 #include "hw_rplidar.h"
  
/*全局变量部分*/
//菜单页码 临时
uint8_t G_Menu_Page = 0;
//DMA Buff
extern uint16_t Adc1_Buff[16];
extern uint16_t Adc1_Real[16];
extern uint8_t 	OLED_GRAM[128][8];
//激光雷达相关
extern uint16_t G_Rplidar_Angle;
extern uint16_t G_Rplidar_Distance;
extern uint16_t G_Rplidar_Collect[361];

void Menu_Display (uint8_t Menu_Page)
{
	OLED_Clear(0);
	switch(Menu_Page)
	{
		case 0:Menu_Adc1_Page();break;
		case 1:Menu_Rplidar_Page();break;
	}
}

/**
 * @brief     Adc1菜单部分
 * @todo      只是为了测试模块，需要改进
 */
void Menu_Adc1_Page(void)
{
	//显示标题
	OLED_ShowString(0,0,"ADC:",12);
	
	//Channel 1-4
	OLED_ShowString(0,12,"1-4:",12);
	OLED_ShowNumber(24,12,(int32_t) Adc1_Real[0],12);
	OLED_ShowNumber(50,12,(int32_t) Adc1_Real[1],12);
	OLED_ShowNumber(76,12,(int32_t) Adc1_Real[2],12);
	OLED_ShowNumber(100,12,(int32_t) Adc1_Real[3],12);
	
	//Channel 5-8
	OLED_ShowString(0,24,"5-8:",12);
	OLED_ShowNumber(24,24,(int32_t) Adc1_Real[4],12);
	OLED_ShowNumber(50,24,(int32_t) Adc1_Real[5],12);
	OLED_ShowNumber(76,24,(int32_t) Adc1_Real[6],12);
	OLED_ShowNumber(100,24,(int32_t) Adc1_Real[7],12);
	
	//Channel 9-12
	OLED_ShowString(0,36,"9- :",12);
	OLED_ShowNumber(24,36,(int32_t) Adc1_Real[8],12);
	OLED_ShowNumber(50,36,(int32_t) Adc1_Real[9],12);
	OLED_ShowNumber(76,36,(int32_t) Adc1_Real[10],12);
	OLED_ShowNumber(100,36,(int32_t) Adc1_Real[11],12);
	
	//Channel 13-16
	OLED_ShowString(0,48,"-16:",12);
	OLED_ShowNumber(24,48,(int32_t) Adc1_Real[12],12);
	OLED_ShowNumber(50,48,(int32_t) Adc1_Real[13],12);
	OLED_ShowNumber(76,48,(int32_t) Adc1_Real[14],12);
	OLED_ShowNumber(100,48,(int32_t) Adc1_Real[15],12);
	
	OLED_Refresh_Gram();
}

/**
 * @brief     雷达数据界面
 * @todo      只是为了测试模块，需要改进
 */
void Menu_Rplidar_Page(void)
{
	//绘制捕获目标信息
	Rplidar_Display_Capture_To_Oled(G_Rplidar_Distance, G_Rplidar_Angle);
	//绘制激光雷达扫描结果
	Rplidar_Display_Map_To_Oled(G_Rplidar_Collect);

	OLED_Refresh_Gram();
}


