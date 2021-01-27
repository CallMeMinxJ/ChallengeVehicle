/**
 * @brief		常用系统函数实现
 * @author	武术擂台车小组
 * @version v0.0.0
 * @date		2021/01/23
 */
 
/*头文件部分*/
#include <stm32f10x.h>
#include "system.h"

/**
 * @brief		中断配置函数
 * @param		无
 * @return	无
 */
void Nvic_Init(void)
{
	//声明中断配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//设置中断分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	
	//TIM3中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
}

/**
 * @brief		禁用JTAG，释放PA15、PB3、PB4
 * @dateils	一定要禁用，否则OLED所需要的 PB3 和 PB4 被占用无法正常显示
 * @param		无
 * @return	无
 */
void JTAGDisable(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/**
 * @brief		将数字转换成字符串
 * @param		int num		- 需要转换的数字
 *					int radix	- 转换基数
 * @return	无
 */
char* Num_To_String(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
	unsigned unum;
	int i=0,j,k;
	char temp;

	//获取要转换的整数的绝对值
	if(radix==10&&num<0)
	{
		unum=(unsigned)-num;
		str[i++]='-';
	}
	else unum=(unsigned)num;
	//转换部分，注意转换后是逆序的
	do
	{
		str[i++]=index[unum%(unsigned)radix];
		unum/=radix;

	}while(unum);
	str[i]='\0';
	//将顺序调整过来
	if(str[0]=='-') k=1;
	else k=0;
	
	for(j=k;j<=(i-1)/2;j++)
	{
		temp=str[j];
		str[j]=str[i-1+k-j];
		str[i-1+k-j]=temp;
	}

	return str;//返回转换后的字符串
}


/**
 * @brirf		以下代码用于实现精准延迟固定时间
 *
 * 该部分代码来自正点原子
 */
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//静态变量定义
static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
	
	
#if SYSTEM_SUPPORT_OS							//如果SYSTEM_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS).
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
#endif

//支持UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII	
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数
#endif

/**
 * @brief		us级延时时,关闭任务调度(防止打断us级延迟)
 * @param		无
 * @return	无
 */
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIII的方式,禁止调度，防止打断us延时
#else											//否则UCOSII
	OSSchedLock();								//UCOSII的方式,禁止调度，防止打断us延时
#endif
}


/**
 * @brief		us级延时时,恢复任务调度
 * @param		无
 * @return	无
 */
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII的方式,恢复调度
#else											//否则UCOSII
	OSSchedUnlock();							//UCOSII的方式,恢复调度
#endif
}


/**
 * @brief		调用OS自带的延时函数延时
 * @param		ticks:延时的节拍数
 * @return	无
 */
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII延时采用周期模式
#else
	OSTimeDly(ticks);							//UCOSII延时
#endif 
}
 

/**
 * @brief		systick中断服务函数,使用ucos时用到
 * @param		无
 * @return	无
 */
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS开始跑了,才执行正常的调度处理
	{
		OSIntEnter();							//进入中断
		OSTimeTick();       					//调用ucos的时钟服务程序               
		OSIntExit();       	 					//触发任务切换软中断
	}
}
#endif


/**
 * @brief		初始化延迟函数
 * @detalis	当使用OS的时候,此函数会初始化OS的时钟节拍
 *					SYSTICK的时钟固定为HCLK时钟的1/8
 *					SYSCLK:系统时钟
 * @param		无
 * @return	无
 */
void Delay_Init()
{
SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	u32 reload;
#endif
	fac_us=SystemCoreClock/8000000;				//为系统时钟的1/8  
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	reload=SystemCoreClock/8000000;				//每秒钟的计数次数 单位为K	   
	reload*=1000000/delay_ostickspersec;		//根据delay_ostickspersec设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	fac_ms=1000/delay_ostickspersec;			//代表OS可以延时的最少单位	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//非OS下,代表每个ms需要的systick时钟数   
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.


/**
 * @brief		延时nus函数
 * @param		nus：要延时的us数
 * @return	无
 */
void Delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							//需要的节拍数	  		 
	tcnt=0;
	delay_osschedlock();						//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};
	delay_osschedunlock();						//恢复OS调度									    
}
/**
 * @brief		延时nms函数
 * @param		nms：要延时的ms数
 * @return	无
 */
void Delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
	{		 
		if(nms>=fac_ms)							//延时的时间大于OS的最少时间周期 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS延时
		}
		nms%=fac_ms;							//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	Delay_us((u32)(nms*1000));					//普通方式延时  
}
#else //不用OS时
/**
 * @brief		延时nus函数
 * @param		nus：要延时的us数
 * @return	无
 */		    								   
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}
/**
 * @brief		延时nms函数
 * @param		nms：要延时的ms数
 * @return	无
 */
void Delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	  	    
} 
#endif 


