/**
 * @file      hw_usart.c
 * @brief     串口模块及通信相关函数
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
#include "system.h"
#include "hw_usart.h"

/**
 * @brief     串口3初始化函数
 * @param     BaudRate - 设置波特率
 */
void Usart3_Init(uint32_t BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //GPIO初始化
    //使能RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//使能TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    //串口初始化参配置
    //配置结构体
	USART_InitStructure.USART_BaudRate      = BaudRate;             //串口波特率
	USART_InitStructure.USART_WordLength    = USART_WordLength_8b;  //一个字节长度
	USART_InitStructure.USART_StopBits      = USART_StopBits_1;     //一个停止位
	USART_InitStructure.USART_Parity        = USART_Parity_No;      //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode          = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_Init(USART3, &USART_InitStructure);               //初始化串口3

    //使能
    DMA_Cmd(DMA1_Channel3,ENABLE);//DMA模式
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);  
	USART_Cmd(USART3, ENABLE);//使能串口3  
}

/**
 * @brief     向串口Usartx发送一个uint16_td的数据
 * @param     UsartX - 串口地址(USART1~8)
 * @param     Data - 需要发送的数据
 */
void Usart_Send_Data (USART_TypeDef * UsartX, uint16_t Data)
{
	USART_SendData(UsartX, Data);                   //向串口3发送数据
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);  //等待发送结束
}

/**
 * @brief     向串口Usartx发送一组uint16_td的数据
 * @param     UsartX - 串口地址(USART1~8)
 * @param     Data_Buff - 数据首地址
 * @param     Data_Count - 数据个数
 */
void Usart_Send_Data_Buff (USART_TypeDef * UsartX, uint16_t * Data_Buff, uint8_t Data_Count)
{
	uint8_t cnt;
	for (cnt = 0; cnt < Data_Count; cnt++)
		Usart_Send_Data(UsartX, Data_Buff[cnt]);
}

/**
 * @brief     串口3中断函数
 */
void USART3_IRQHandler (void)
{
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        USART_ClearFlag(USART3, USART_IT_IDLE);

    }
}





