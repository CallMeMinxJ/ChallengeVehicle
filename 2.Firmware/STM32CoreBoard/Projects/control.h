#ifndef _CONTROL_H_
#define _CONTROL_H_

/*宏定义部分*/
#define INFRARED_DISTANCE_THRESHOLD 1000

/*函数声明部分*/
void Car_Init(void);
void Car_Data_Processing(void);
void First_Order_Complementary_Filtering (uint16_t * Pending_Data , uint16_t * Processed_Data , double a , uint8_t Data_Count);

#endif

