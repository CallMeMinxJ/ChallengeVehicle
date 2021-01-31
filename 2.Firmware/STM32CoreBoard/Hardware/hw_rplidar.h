#ifndef _RPLIDAR_H_
#define _RPLIDAR_H_

/*头文件部分*/
#include "system.h"

/*宏定义部分*/
//雷达捕获目标阈值
#define TARGET_DISTANCE_THRESHOLD   45      //mm
#define TARGET_DISTANCE_MAX         2000    //mm
//取绝对值方法
#define sabs(x) (x>0?x:-x)

/*函数声明部分*/
void Rplidar_Init(void);
void Rplidar_Start_Scanning(void);
void Rplidar_Data_Processing (uint8_t * Usart3_Buff);
void Rplidar_Display_Map_To_Oled(uint16_t *collect);
void Rplidar_Capture_Target(uint16_t * collect, uint16_t Target_Distance_Threshold, uint16_t Target_Distance_Max);
void Rplidar_Display_Capture_To_Oled(uint16_t Rplidar_Distance, uint16_t Rplidar_Angle);

#endif

