#ifndef _KEY_H_
#define _KEY_H_

/*ͷ�ļ�����*/
#include "system.h"

/*�궨�岿��*/
//��������
#define No_Key_Num	0
#define Key_0_Num		1
#define Key_1_Num		2
#define Key_2_Num		3
//�����˿� �͵�ƽ��Ч
#define Key_0	!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)
#define Key_1	!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)
#define Key_2	!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)
//�������ģʽ
#define Mode_Ucnt		0//������
#define Mode_cnt		1//����
//����ʱ��
#define Key_Debounce_Time 5

/*������������*/
void Key_Init(void);
uint8_t Key_Scan(uint8_t mode);
void Key_Function(uint8_t Key_Num);

#endif
