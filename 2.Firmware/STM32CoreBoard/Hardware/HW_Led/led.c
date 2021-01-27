/**
 * @brief		核心板上LED相关函数
 * @author	武术擂台车小组
 * @global	bool Led_0_Flash_Door - 控制Led_0闪烁
 *					bool Led_1_Flash_Door - 控制Led_1闪烁
 * @version v0.0.0
 * @date		2021/01/24
 */

/*头文件部分*/
#include "system.h"
#include "led.h"

/*全局变量部分*/
bool Led_0_Flash_Door = false;
bool Led_1_Flash_Door = false;

/**
 * @brief		LED初始化函数
 * @param		无
 * @return	无
 */
void Led_Init(void)
{
	//定义初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能引脚外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/*GPIOB*/
	//初始化结构体置缺省值
	GPIO_StructInit(&GPIO_InitStructure);
	//引脚参数配置结构体
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//推挽输出
	//载入参数配置结构体
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//LED状态初始化
	Led_0 = Led_Turn_Off;
	Led_1 = Led_Turn_Off;
}

/**
 * @brief		LED显示函数
 * @param		uint8_t Led_0_Mode
 *						Led_Turn_On		-	0	常亮
 *						Led_Turn_Off	-	1	常灭
 *						Led_Flash			-	2	闪烁
 * @return	无
 */
void Led_Display(uint8_t Led_0_Mode,uint8_t Led_1_Mode)
{
	//点亮和熄灭
	if 			(Led_0_Mode == Led_Turn_On)		Led_0 = Led_Turn_On;
	else if (Led_0_Mode == Led_Turn_Off)	Led_0 = Led_Turn_Off;
	if 			(Led_1_Mode == Led_Turn_On)		Led_1 = Led_Turn_On;
	else if (Led_1_Mode == Led_Turn_Off)	Led_1 = Led_Turn_Off;
	
	//LED闪烁
	if (Led_0_Mode == Led_Flash)Led_0_Flash_Door = true;
	else	Led_0_Flash_Door = false;
	if (Led_1_Mode == Led_Flash)Led_1_Flash_Door = true;
	else	Led_1_Flash_Door = false;
}

 




