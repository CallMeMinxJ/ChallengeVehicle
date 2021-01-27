/**
 * @brief		包含核心板上按键相关函数
 * @author	武术擂台车小组
 * @version v0.0.0
 * @date		2021/01/24
 */
 
 /*头文件部分*/
 #include "system.h"
 #include "key.h"
 #include "led.h"
 
/**
 * @brief		按键初始化的函数
 * @param		无
 * @return	无
 */
void Key_Init(void)
{
	//定义初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能引脚外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	/*GPIOD*/
	//初始化结构体置缺省值
	GPIO_StructInit(&GPIO_InitStructure);
	//引脚参数配置结构体
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;//上拉输出
	//载入参数配置结构体
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

/**
 * @brief		按键检测函数
 * @param		uint8_t	mode：
 *						Mode_cnt	1 - 连续检测
 *						Mode_Ucnt	0 - 非连续检测
 * @return	uint8_t Key_Num:
 *						Key_0_Num 	1 - 按键0
 *						Key_1_Num 	2 - 按键1
 *						Key_2_Num 	3 - 按键2
 *						No_Key_Num 	1 - 无按键
 */
uint8_t Key_Scan(uint8_t mode)
{
	static int8_t KEY_STATE = 1;//记录按键状态
    int8_t cnt   = 0,//记录按键按下个数防止误触
           test1 = 0,//记录按键
           test2 = 0;//验证按键
    if (mode == Mode_cnt)   KEY_STATE = 1;
    
    /*检测第一次触发*/
    if (Key_0)	test1 = Key_0_Num	,cnt++;
    if (Key_1)	test1 = Key_1_Num	,cnt++;
    if (Key_2)	test1 = Key_2_Num	,cnt++;
    
    if (cnt==1 && KEY_STATE)
    {
        cnt       = 0;
        KEY_STATE = 0;//非连续时按键状态等待置1
        Delay_ms(Key_Debounce_Time);//按键消抖等待
        
        /*检测第二次触发*/
        if (Key_0)  test2 = Key_0_Num	,cnt++;
        if (Key_1)	test2 = Key_1_Num	,cnt++;
        if (Key_2)	test2 = Key_2_Num	,cnt++;
        if (cnt==1 && test1 == test2) return test2;
    }
    else
        if (!cnt)  KEY_STATE = 1;//非连续时无触发置1允许下次触发
    return  No_Key_Num;
}

/**
 * @brief		按键实现功能函数
 * @param		uint8_t Key_Num	-	对应不同按键的编码
 *						Key_0_Num - 
 *						Key_1_Num - 
 *						Key_2_Num - 
 * @return	无
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























 
 

