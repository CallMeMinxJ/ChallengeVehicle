/**
 * @brief		ϵͳTIMʱ����غ���
 * @author	������̨��С��
 * @global	bool Led_0_Flash_Door - ����Led_0��˸
 *					bool Led_1_Flash_Door - ����Led_1��˸
 * @version v0.0.0
 * @date		2021/01/25
 */
 
/*ͷ�ļ�����*/
#include "system.h"
#include "hw_timer.h"
#include "hw_led.h"

/*ȫ�ֱ�������*/
extern bool Led_0_Flash_Door;
extern bool Led_1_Flash_Door;

/**
 * @brief		ʱ��3��ʼ������
 * @param		uint16_t ARR	-	Ԥװ��ֵ
 *					uint16_t PSC	-	ʱ��Ԥ��Ƶϵ��
 * @return	��
 */
void TIM3_Init(uint16_t ARR,uint16_t PSC)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = ARR; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,���������ж�
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

/**
 * @brief		ʱ��3�жϺ���
 * @param		��
 * @return	��
 */
void TIM3_IRQHandler(void)
{
	static uint16_t Led_Flash_Time_ms = 0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		//Led��˸����
		if (Led_0_Flash_Door || Led_1_Flash_Door)
		{
			//flash ���� 500ms
			if(Led_Flash_Time_ms<500)Led_Flash_Time_ms++;
			else 
			{
				Led_Flash_Time_ms=0;
				
				//Led��ת
				if(Led_0_Flash_Door)Led_0 = !Led_0;
				if(Led_1_Flash_Door)Led_1 = !Led_1;
			}
		}
	}
}









