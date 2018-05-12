#include "card.h"
#include "delay.h"
#include "wiegand.h"


static void GPIO_WiegandPower_INIT(void);
static void GPIO_Wiegand_INIT(void);			


void Card_Init(void)	
{	
	GPIO_WiegandPower_INIT();
	GPIO_Wiegand_INIT();
}


static void GPIO_WiegandPower_INIT(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʱ��ʹ��
	RCC_APB2PeriphClockCmd(GPIOReadHead_RCC_CLK, ENABLE);

	//����ͷ��Դ����	
	GPIO_InitStructure.GPIO_Pin =  GPIOReadHead_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
	GPIO_Init(GPIOReadHead_PORT, &GPIO_InitStructure);
}	


// ��������  : ΰ����ʼ��
static void GPIO_Wiegand_INIT(void)	
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

  RCC_APB2PeriphClockCmd(GPIOWGReader_OUT_RCC_CLK|RCC_APB2Periph_AFIO, ENABLE);
	/****************����ͷ1***************************************/	
	GPIO_InitStructure.GPIO_Pin = WGReader_H1D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H1D1_PORT, &GPIO_InitStructure);	//����GPIO���
	GPIO_EXTILineConfig(WGReader_H1D1_PORTSOURCE, WGReader_H1D1_PINSOURCE);//����EXTI�� ʹ�ж��ߺ�IO�����������
	
	GPIO_InitStructure.GPIO_Pin = WGReader_H1D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H1D0_PORT, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(WGReader_H1D0_PORTSOURCE, WGReader_H1D0_PINSOURCE);
	
	/****************����ͷ2***************************************/	
	GPIO_InitStructure.GPIO_Pin = WGReader_H2D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H2D1_PORT, &GPIO_InitStructure);	//����GPIO���
	GPIO_EXTILineConfig(WGReader_H2D1_PORTSOURCE, WGReader_H2D1_PINSOURCE);//����EXTI�� ʹ�ж��ߺ�IO�����������
	
	GPIO_InitStructure.GPIO_Pin = WGReader_H2D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H2D0_PORT, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(WGReader_H2D0_PORTSOURCE, WGReader_H2D0_PINSOURCE);

  /****************����ͷ3***************************************/	
	GPIO_InitStructure.GPIO_Pin = WGReader_H3D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H3D1_PORT, &GPIO_InitStructure);	//����GPIO���
	GPIO_EXTILineConfig(WGReader_H3D1_PORTSOURCE, WGReader_H3D1_PINSOURCE);//����EXTI�� ʹ�ж��ߺ�IO�����������
	
	GPIO_InitStructure.GPIO_Pin = WGReader_H3D0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WGReader_H3D0_PORT, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(WGReader_H3D0_PORTSOURCE, WGReader_H3D0_PINSOURCE);
	//**********��EXTI���ϳ����½��ز����ж�************
	EXTI_InitStructure.EXTI_Line = WGReader_H1D1_EXTLINE|WGReader_H1D0_EXTLINE|
	                               WGReader_H2D1_EXTLINE|WGReader_H2D0_EXTLINE|
                                   WGReader_H3D1_EXTLINE|WGReader_H3D0_EXTLINE;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			 //���� EXTI ��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//��·��״̬
	EXTI_Init(&EXTI_InitStructure);	
	/***************** �ⲿ�ж�ʹ������ ***********************/		
	/* Enable the EXTI8��9 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
	
	// Enable the EXTI12 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
		
	// Enable the EXTI2 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
}




	
void EXTI9_5_IRQHandler(void)   
{
	if(EXTI_GetITStatus(EXTI_Line8)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
		if(WGReader_H1D0_Reader==0)
		{	
			wiegand_reader_d0(0,0);
		}		
		else
		{
			wiegand_reader_d0(0,1);
		}
		
	}
	else if(EXTI_GetITStatus(EXTI_Line9)!= RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line9);
		if(WGReader_H1D1_Reader==0)
		{		
			wiegand_reader_d0(1,0);
		}
		else
		{
			wiegand_reader_d0(1,1);
		}
	}  	

			
	if(EXTI_GetITStatus(EXTI_Line5)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		if(WGReader_H3D0_Reader==0)
		{
			wiegand_reader_d3(0,0);
		}
		else
		{
			wiegand_reader_d3(0,1);
		}
	}	
	else if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		if(WGReader_H3D1_Reader==0)
		{
			wiegand_reader_d3(1,0);
		}
		else
		{	
			wiegand_reader_d3(1,1);	
		}
	}  	
}

void EXTI15_10_IRQHandler(void)   
{	
	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);
		if(WGReader_H2D0_Reader==0)
		{		
			wiegand_reader_d2(0,0);
		}
		else
		{
			wiegand_reader_d2(0,1);
		}
	} 
}  

void EXTI2_IRQHandler(void)      
{
	if(EXTI_GetITStatus(EXTI_Line2)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
		if(WGReader_H2D1_Reader==0)	
		{		
			wiegand_reader_d2(1,0);
		}
		else
		{
			wiegand_reader_d2(1,1);	
		}
	} 
	
}  

