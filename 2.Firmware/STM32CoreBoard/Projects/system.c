/**
 * @brief		����ϵͳ����ʵ��
 * @author	������̨��С��
 * @version v0.0.0
 * @date		2021/01/23
 */
 
/*ͷ�ļ�����*/
#include <stm32f10x.h>
#include "system.h"

/**
 * @brief		�ж����ú���
 * @param		��
 * @return	��
 */
void Nvic_Init(void)
{
	//�����ж����ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�����жϷ���2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	
	//TIM3�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
}

/**
 * @brief		����JTAG���ͷ�PA15��PB3��PB4
 * @dateils	һ��Ҫ���ã�����OLED����Ҫ�� PB3 �� PB4 ��ռ���޷�������ʾ
 * @param		��
 * @return	��
 */
void JTAGDisable(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/**
 * @brief		������ת�����ַ���
 * @param		int num		- ��Ҫת��������
 *					int radix	- ת������
 * @return	��
 */
char* Num_To_String(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
	unsigned unum;
	int i=0,j,k;
	char temp;

	//��ȡҪת���������ľ���ֵ
	if(radix==10&&num<0)
	{
		unum=(unsigned)-num;
		str[i++]='-';
	}
	else unum=(unsigned)num;
	//ת�����֣�ע��ת�����������
	do
	{
		str[i++]=index[unum%(unsigned)radix];
		unum/=radix;

	}while(unum);
	str[i]='\0';
	//��˳���������
	if(str[0]=='-') k=1;
	else k=0;
	
	for(j=k;j<=(i-1)/2;j++)
	{
		temp=str[j];
		str[j]=str[i-1+k-j];
		str[i-1+k-j]=temp;
	}

	return str;//����ת������ַ���
}


/**
 * @brirf		���´�������ʵ�־�׼�ӳٹ̶�ʱ��
 *
 * �ò��ִ�����������ԭ��
 */
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//��̬��������
static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
	
	
#if SYSTEM_SUPPORT_OS							//���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

/**
 * @brief		us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
 * @param		��
 * @return	��
 */
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else											//����UCOSII
	OSSchedLock();								//UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}


/**
 * @brief		us����ʱʱ,�ָ��������
 * @param		��
 * @return	��
 */
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII�ķ�ʽ,�ָ�����
#else											//����UCOSII
	OSSchedUnlock();							//UCOSII�ķ�ʽ,�ָ�����
#endif
}


/**
 * @brief		����OS�Դ�����ʱ������ʱ
 * @param		ticks:��ʱ�Ľ�����
 * @return	��
 */
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII��ʱ��������ģʽ
#else
	OSTimeDly(ticks);							//UCOSII��ʱ
#endif 
}
 

/**
 * @brief		systick�жϷ�����,ʹ��ucosʱ�õ�
 * @param		��
 * @return	��
 */
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();							//�����ж�
		OSTimeTick();       					//����ucos��ʱ�ӷ������               
		OSIntExit();       	 					//���������л����ж�
	}
}
#endif


/**
 * @brief		��ʼ���ӳٺ���
 * @detalis	��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
 *					SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
 *					SYSCLK:ϵͳʱ��
 * @param		��
 * @return	��
 */
void Delay_Init()
{
SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
	u32 reload;
#endif
	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  
#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
	reload=SystemCoreClock/8000000;				//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/delay_ostickspersec;		//����delay_ostickspersec�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/delay_ostickspersec;			//����OS������ʱ�����ٵ�λ	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 						//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.


/**
 * @brief		��ʱnus����
 * @param		nus��Ҫ��ʱ��us��
 * @return	��
 */
void Delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=nus*fac_us; 							//��Ҫ�Ľ�����	  		 
	tcnt=0;
	delay_osschedlock();						//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	delay_osschedunlock();						//�ָ�OS����									    
}
/**
 * @brief		��ʱnms����
 * @param		nms��Ҫ��ʱ��ms��
 * @return	��
 */
void Delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(nms>=fac_ms)							//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS��ʱ
		}
		nms%=fac_ms;							//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	Delay_us((u32)(nms*1000));					//��ͨ��ʽ��ʱ  
}
#else //����OSʱ
/**
 * @brief		��ʱnus����
 * @param		nus��Ҫ��ʱ��us��
 * @return	��
 */		    								   
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}
/**
 * @brief		��ʱnms����
 * @param		nms��Ҫ��ʱ��ms��
 * @return	��
 */
void Delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	    
} 
#endif 


