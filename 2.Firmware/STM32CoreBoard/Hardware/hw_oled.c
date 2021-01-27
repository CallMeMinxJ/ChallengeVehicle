/**
 * @brief		OLED��غ���
 * @author	������̨��С��
 * @version v0.0.0
 * @date		2021/01/23
 * @attention
 *					���ļ�ʹ��������ת�ַ�������������������system.c��
 */
 
/*ͷ�ļ�����*/
#include "stdlib.h"
#include <math.h>
#include "system.h"
#include "hw_oled.h"
#include "hw_oledfont.h"

/*ȫ�ֱ�������*/
uint8_t OLED_GRAM[128][8];	//�Դ�

/**
 * @brief		��OLEDд���������
 * @param		dat��ָ������ cmd��д����/д����
 * @return	��
 */
void OLED_WR_Byte( uint8_t dat, uint8_t cmd)
{
  uint8_t i;
  if(cmd)
    OLED_DC=1;
  else
    OLED_DC=0;
  for(i=0;i<8;i++)
  {
    OLED_DO=0;
    if(dat&0x80)
       OLED_D1=1;
    else
       OLED_D1=0;
    OLED_DO=1;
    dat<<=1;
  }
  OLED_DC=1;
}

/**
 * @brief		��������ˢ���Դ�����
 * @param		��
 * @return	��
 */
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

/**
 * @brief		��OLED��ʾ
 * @param		��
 * @return	��
 */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
 * @brief		�ر�OLED��ʾ
 * @param		��
 * @return	��
 */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 


/**
 * @brief		OLED����������������ȫ��
 * @param		mode��1/0 ������ˢ��/��ˢ��
 * @return	��
 */
void OLED_Clear(uint8_t mode)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	if (mode)
		OLED_Refresh_Gram();//������ʾ
}

/**
 * @brief		���ܣ�����Ļһ��λ���ϻ���
 * @param		x:������� y���������� t����ʾ��ʽ ��/��
 * @return	��
 */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

/**
 * @brief		���ܣ���һ��ֱ��
 * @param		x1,y1:��һ�������� x2,y2:�ڶ��������� mode��1/0 ����/����
 * @return	��
 */
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	int16_t 
		delta_x = x2-x1,		//��ֵ
		delta_y = y2-y1,
		cpr_x = 0,					//�Ƚ�ֵ
		cpr_y = 0;
	
	uint8_t 
		n = 0,
		x = x1,							//��ǰ����
		y = y1,
												//��¼����
		distance = abs(delta_x)>=abs(delta_y)?abs(delta_x):abs(delta_y);	
	
	//����
	if(delta_x == 0)
	{
		for(n=0;n<abs(y2-y1)+1;n++)
		{
			OLED_DrawPoint(x,y,mode);
			y+=(y2>=y1)?1:-1;
		}
	}
	//�Ǵ��� ���
	else
	{
		for(n=0;n<=distance+1;n++)
		{
			OLED_DrawPoint(x,y,mode);
			
			cpr_x+=delta_x;
			cpr_y+=delta_y;
			
			if(abs(cpr_x)>distance)
			{
				cpr_x=(delta_x>=0)?(cpr_x-distance):(cpr_x+distance);
				x=(delta_x>=0)?(x+1):(x-1);
			}
			
			if(abs(cpr_y)>distance)
			{
				cpr_y=(delta_y>=0)?(cpr_y-distance):(cpr_y+distance);
				y=(delta_y>=0)?(y+1):(y-1);
			}
		}
	}
}

/**
 * @brief		���ܣ���һ������
 * @param		x1,y1:������� y1,y2���յ�����
 * @return	��
 */
void OLED_DrawRectangular(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	OLED_DrawLine(x1,y1,x2,y1,mode);
	OLED_DrawLine(x2,y1,x2,y2,mode);
	OLED_DrawLine(x2,y2,x1,y2,mode);
	OLED_DrawLine(x1,y2,x1,y1,mode);
}

/**
 * @brief		���ܣ�����Ļ��һ��λ����ʾһ���ַ�
 * @param		x:������ y�������� size���ֺ� 12/16/24 mode��0������ʾ 1������ʾ
 * @return	��
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
			if(size==12)temp=asc2_1206[chr][t]; 	 		//����1206����
			else if(size==16)temp=asc2_1608[chr][t];	//����1608����
			else if(size==24)temp=asc2_2412[chr][t];	//����2412����
			else return;															//û�е��ֿ�
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
    }          
}

/**
 * @brief		���ܣ��ڹ̶�λ����ʾ�ַ���
 * @param		x:������ y�������� p����Ҫ��ʾ���ַ��� size:�ֺ� 12/16/32
 * @return	��
 */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear(1);}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
}

/**
 * @brief		���ܣ��ڹ̶�λ����ʾ����
 * @param		x:������ y�������� p����Ҫ��ʾ���ַ��� size:�ֺ� 12/16/32
 * @return	��
 */
void OLED_ShowNumber(uint8_t x,uint8_t y,int32_t number,uint8_t size)
{	
	char *p;
	p = Num_To_String(number,p,10);
	while((*p<='9')&&(*p>='0'))//�ж��ǲ��ǷǷ��ַ�
	{       
			if(x>(128-(size/2))){x=0;y+=size;}
			if(y>(64-size)){y=x=0;OLED_Clear(1);}
			OLED_ShowChar(x,y,*p,size,1);	 
			x+=size/2;
			p++;
	}  
}

/**
 * @brief		���ܣ���ʼ��OLED
 * @param		��
 * @return	��
 */
void OLED_Init(void)
{
	//����GPIO��ʼ���ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//ʹ�ܶ�Ӧ�����ⲿʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*GPIOB*/
	//Ϊ��ʼ���ṹ�帳ȱʡֵ
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;//�������
	//��ȡ��ʼ���ṹ���ֵ
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//IO��ʼ����ɺ�����Ĭ�ϵ�ƽ
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);

	/*��������OLEDоƬͨ�Ŵ��룬��������*/
	OLED_RS=1;
	Delay_ms(100);
	OLED_RS=0;
	Delay_ms(100);
	OLED_RS=1;
	
	//ָ��������ʾģʽ
	OLED_WR_Byte(0xAE,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x81,OLED_CMD);
	OLED_WR_Byte(0xCF,OLED_CMD);
	OLED_WR_Byte(0xA1,OLED_CMD);
	OLED_WR_Byte(0xC0,OLED_CMD);//0xc8 0xc0 ��Ļ�ߵ�������һ��
	OLED_WR_Byte(0xA8,OLED_CMD);
	OLED_WR_Byte(0x3f,OLED_CMD);
	OLED_WR_Byte(0xD3,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xd5,OLED_CMD);
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD);
	OLED_WR_Byte(0xF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD);
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x20,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xA4,OLED_CMD);
	OLED_WR_Byte(0xA6,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD);

	OLED_Clear(1);
}





