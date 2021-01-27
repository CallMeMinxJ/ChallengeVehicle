/**
 * @brief		�������İ��ϰ�����غ���
 * @author	������̨��С��
 * @version v0.0.0
 * @date		2021/01/24
 */
 
 /*ͷ�ļ�����*/
 #include "system.h"
 #include "hw_key.h"
 #include "hw_led.h"
 
/**
 * @brief		������ʼ���ĺ���
 * @param		��
 * @return	��
 */
void Key_Init(void)
{
	//�����ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�������ⲿʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	/*GPIOD*/
	//��ʼ���ṹ����ȱʡֵ
	GPIO_StructInit(&GPIO_InitStructure);
	//���Ų������ýṹ��
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;//�������
	//����������ýṹ��
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

/**
 * @brief		������⺯��
 * @param		uint8_t	mode��
 *						Mode_cnt	1 - �������
 *						Mode_Ucnt	0 - ���������
 * @return	uint8_t Key_Num:
 *						Key_0_Num 	1 - ����0
 *						Key_1_Num 	2 - ����1
 *						Key_2_Num 	3 - ����2
 *						No_Key_Num 	1 - �ް���
 */
uint8_t Key_Scan(uint8_t mode)
{
	static int8_t KEY_STATE = 1;//��¼����״̬
    int8_t cnt   = 0,//��¼�������¸�����ֹ��
           test1 = 0,//��¼����
           test2 = 0;//��֤����
    if (mode == Mode_cnt)   KEY_STATE = 1;
    
    /*����һ�δ���*/
    if (Key_0)	test1 = Key_0_Num	,cnt++;
    if (Key_1)	test1 = Key_1_Num	,cnt++;
    if (Key_2)	test1 = Key_2_Num	,cnt++;
    
    if (cnt==1 && KEY_STATE)
    {
        cnt       = 0;
        KEY_STATE = 0;//������ʱ����״̬�ȴ���1
        Delay_ms(Key_Debounce_Time);//���������ȴ�
        
        /*���ڶ��δ���*/
        if (Key_0)  test2 = Key_0_Num	,cnt++;
        if (Key_1)	test2 = Key_1_Num	,cnt++;
        if (Key_2)	test2 = Key_2_Num	,cnt++;
        if (cnt==1 && test1 == test2) return test2;
    }
    else
        if (!cnt)  KEY_STATE = 1;//������ʱ�޴�����1�����´δ���
    return  No_Key_Num;
}

/**
 * @brief		����ʵ�ֹ��ܺ���
 * @param		uint8_t Key_Num	-	��Ӧ��ͬ�����ı���
 *						Key_0_Num - 
 *						Key_1_Num - 
 *						Key_2_Num - 
 * @return	��
 */
void Key_Function(uint8_t Key_Num)
{
	switch(Key_Num)
	{
		case Key_0_Num:
		{
			Led_Display(Led_Turn_On,Led_Turn_Off);
			break;
		}
		
		case Key_1_Num:
		{
			Led_Display(Led_Turn_Off,Led_Turn_On);
			break;
		}
		
		case Key_2_Num:
		{
			Led_Display(Led_Flash,Led_Turn_Off);
			break;
		}
	}
}























 
 

