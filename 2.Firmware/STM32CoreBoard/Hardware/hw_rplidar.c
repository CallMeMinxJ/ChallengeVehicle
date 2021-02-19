/**
 * @file      hw_rplidar.c
 * @brief     红外测距雷达相关函数
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
#include <math.h>
#include <stdlib.h>
#include "system.h"
#include "hw_rplidar.h"
#include "hw_dma.h"
#include "hw_usart.h"
#include "hw_oled.h"


/*全局变量部分*/
//雷达开启扫描命令
uint16_t G_Rplidar_Start_Up[9] = {0xA5, 0x82, 0x05, 0, 0, 0, 0, 0, 0x22};
//雷达处理后
uint16_t G_Rplidar_Angle = 0;
uint16_t G_Rplidar_Distance = 0;
uint16_t G_Rplidar_Collect[361] = {0};
//捕获目标的信息
uint16_t G_Rplidar_Target_Distance = 0;
uint16_t G_Rplidar_Target_Angle = 0;
bool     G_Rplidar_Target_Is_Catched = false;
//显示比例尺
uint16_t dmax = 6000;

/**
 * @brief     红外测距雷达初始化函数
 */
void Rplidar_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	/*GPIOD*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//上拉输入
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	Dma_Usart2_To_GlobalVar();
	Usart2_Init(115200);
}

/**
 * @brief     雷达启动扫描
 */
void Rplidar_Start_Scanning(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
    Usart_Send_Data_Buff(USART2, G_Rplidar_Start_Up,9);
}

/**
 * @brief     接收雷达发送的一帧数据的分析函数
 * @param     Usart3_Buff - 得到的一帧数据包
 */
void Rplidar_Data_Processing (uint8_t * Usart2_Buff)
{
    uint8_t     i = 0, ChkSum = 0, count = 0;
    float       AngleDiff = 0, dsita[32] = {0};
    uint16_t    distance[32] = {0};
    float       real_angle = 0, start_angle_q6 = 0, temp[32] = {0};
    static float last_real_angle = 0;

    ChkSum = Usart2_Buff[0] & 0x0F;
    ChkSum += (Usart2_Buff[1] & 0x0F) << 4;

    start_angle_q6 = Usart2_Buff[2];
    start_angle_q6 += ((uint16_t)Usart2_Buff[3] & 0x7F) << 8;

    for(i = 4;i < 84;i += 5)
    {
        distance[count] = Usart2_Buff[i] >> 2;
        distance[count] += ((uint16_t)Usart2_Buff[i + 1]) << 6;
        dsita[count] = (Usart2_Buff[i] & 0x01) << 4;
        dsita[count] += Usart2_Buff[i + 4] & 0x0F;

        if ((Usart2_Buff[i] & 0x02) == 0x02) dsita[count] *= -1;

        dsita[count] /= 8;
        count ++;
        distance[count] = Usart2_Buff[i + 2] >> 2;
        distance[count] += ((uint16_t)Usart2_Buff[i + 3]) << 6;
        dsita[count] = (Usart2_Buff[i + 2] & 0x01) << 4;
        dsita[count] += Usart2_Buff[i + 4] & 0xF0;

        if ((Usart2_Buff[i + 2] & 0x02) == 0x02) dsita[count] *= -1;

        dsita[count] /= 8;
        count ++;
    }

    real_angle = start_angle_q6 / 64.0;

    AngleDiff = ((last_real_angle <= real_angle)?
            (real_angle - last_real_angle):
            (360 + real_angle - last_real_angle));

    for(i = 1; i <= 32; i++)
    {
        temp[i] = last_real_angle + AngleDiff / 32.0 * i;
        G_Rplidar_Collect[(int)temp[i]] = distance[i];
    }

    last_real_angle = real_angle;
    G_Rplidar_Distance = G_Rplidar_Collect[G_Rplidar_Angle];
}

/**
 * @brief     利用OLED绘制激光雷达扫描结果
 * @param     collect - 激光雷达得到的极坐标信息
 */
void Rplidar_Display_Map_To_Oled(uint16_t *collect)
{
	float k1 = 3.14 / 180;//常系数
	int i;//计数
	uint32_t x, y;
	
	for(i = 0; i < 360; i ++)
	{
		//转化坐标
		x = 94 + (30.0 * collect[i] * cos(k1 * i - (3.1416 / 2))) / dmax;
		y = 32 - (30.0 * collect[i] * sin(k1 * i - (3.1416 / 2))) / dmax;

		//判断点是否在边界内
		if(abs((int)x-94)<30 && abs((int)y-32)<30)
		{			
			OLED_DrawPoint(x,y,1);
		}
	}	
}

/**
 * @brief     分析激光雷达扫描结果利用均值滤波捕获目标
 * @param     collect - 激光雷达得到的极坐标信息
 * @param     Target_Distance_Threshold - 判定为目标的距离阈值mm
 * @param     Target_Distance_Max - 加入判定的最大距离mm
 */
void Rplidar_Capture_Target(uint16_t * collect, uint16_t Target_Distance_Threshold, uint16_t Target_Distance_Max)
{
    int n=0,door=0,i,scan1[5]={0},scan2[5]={0};
	int a1=0,a2=0;//角度1 和 角度2
	int d1=0,d2=0;//距离1 和 距离2

	//未捕获目标
	G_Rplidar_Target_Is_Catched = false;
	
	//计算整体数据的均值
	for (i=0;i<361;i++)
	{
		if (collect[i]<Target_Distance_Max)
		{
			if (door == 0)
			{
				//取样
				for (n = 0; n < 5; n ++)
				{
					if ((i + 1 + n) < 361)
						scan1[n] = collect[i + 1 + n];
					else scan1[n] = collect[i + 1 + n - 360];
				}
				//排除单个点的干扰
				if 
				(
					(collect[i] - scan1[0]) > Target_Distance_Threshold
					&& abs(scan1[0] - scan1[1]) < Target_Distance_Threshold
					&& abs(scan1[1] - scan1[2]) < Target_Distance_Threshold
					&& abs(scan1[2] - scan1[3]) < Target_Distance_Threshold
					&& abs(scan1[3] - scan1[4]) < Target_Distance_Threshold
				)
				//第一个异常点
				{
					d1 = collect[i];
					a1 = i;
					
					door = 1;
				}
			}
			else 
			{
				//取样
				for (n = 0; n < 5; n ++)
				{
					if ((i - n - 1) >= 0)
						scan2[n] = collect[i - n - 1];
					else scan2[n] = collect[i - 1 - n + 360];
				}
				
				//排除单个点的干扰
				if 
				(
					(collect[i] - scan2[0]) > Target_Distance_Threshold
					&& abs(scan2[0] - scan2[1]) < Target_Distance_Threshold
					&& abs(scan2[1] - scan2[2]) < Target_Distance_Threshold
					&& abs(scan2[2] - scan2[3]) < Target_Distance_Threshold
					&& abs(scan2[3] - scan2[4]) < Target_Distance_Threshold
				)
				//第二个异常点
				{
					d2 = collect[i];
					a2 = i;
					
					G_Rplidar_Angle = (a1 + a2) / 2;
					G_Rplidar_Target_Distance = (d1 + d2) / 2;
					
                    //已经捕获到目标
					G_Rplidar_Target_Is_Catched = true;
					break;
				}
			}
		}

	}
}

/**
 * @brief     激光雷达捕获目标信息显示
 * @param     Rplidar_Distance - 目标距离
 * @param     Rplidar_Angle - 目标角度
 */
void Rplidar_Display_Capture_To_Oled(uint16_t Rplidar_Distance, uint16_t Rplidar_Angle)
{
	//地图边界
	OLED_DrawRectangular(64, 2, 124, 62, 1);		
	
	//显示数据
    OLED_ShowString(0, 0, "Rplidar", 12);
    OLED_ShowString(0, 12, "A:", 12);
    OLED_ShowNumber(12, 12, Rplidar_Angle, 12);
    OLED_ShowString(0, 24, "D:", 12);
    OLED_ShowNumber(12, 24, Rplidar_Distance, 12);
	
	//十字盘和方向线
    OLED_DrawLine(0, 64-15, 30,64-15, 1);
    OLED_DrawLine(15, 64, 15, 64-30, 1);

	//画出目标方向
	if(G_Rplidar_Target_Is_Catched)
		OLED_DrawLine(15,64-15,15+(int)(15*cos((Rplidar_Angle*3.1416)/180)),64-15-(int)(15*sin((Rplidar_Angle*3.1416)/180)),1);
}
