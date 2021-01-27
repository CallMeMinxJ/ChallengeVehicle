/**
 * @brief		���İ���LED��غ���
 * @author	������̨��С��
 * @global	bool Led_0_Flash_Door - ����Led_0��˸
 *					bool Led_1_Flash_Door - ����Led_1��˸
 * @version v0.0.0
 * @date		2021/01/24
 */

/*ͷ�ļ�����*/
#include "system.h"
#include "hw_led.h"

/*ȫ�ֱ�������*/
bool Led_0_Flash_Door = false;
bool Led_1_Flash_Door = false;

/**
 * @brief		LED��ʼ������
 * @param		��
 * @return	��
 */
void Led_Init(void)
{
	//�����ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�������ⲿʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/*GPIOB*/
	//��ʼ���ṹ����ȱʡֵ
	GPIO_StructInit(&GPIO_InitStructure);
	//���Ų������ýṹ��
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//�������
	//����������ýṹ��
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//LED״̬��ʼ��
	Led_0 = Led_Turn_Off;
	Led_1 = Led_Turn_Off;
}

/**
 * @brief		LED��ʾ����
 * @param		uint8_t Led_0_Mode
 *						Led_Turn_On		-	0	����
 *						Led_Turn_Off	-	1	����
 *						Led_Flash			-	2	��˸
 * @return	��
 */
void Led_Display(uint8_t Led_0_Mode,uint8_t Led_1_Mode)
{
	//������Ϩ��
	if 			(Led_0_Mode == Led_Turn_On)		Led_0 = Led_Turn_On;
	else if (Led_0_Mode == Led_Turn_Off)	Led_0 = Led_Turn_Off;
	if 			(Led_1_Mode == Led_Turn_On)		Led_1 = Led_Turn_On;
	else if (Led_1_Mode == Led_Turn_Off)	Led_1 = Led_Turn_Off;
	
	//LED��˸
	if (Led_0_Mode == Led_Flash)Led_0_Flash_Door = true;
	else	Led_0_Flash_Door = false;
	if (Led_1_Mode == Led_Flash)Led_1_Flash_Door = true;
	else	Led_1_Flash_Door = false;
}

 




