#include "card.h"
#include "delay.h"	

static void GPIO_WiegandPower_INIT(void);
static void GPIO_Wiegand_INIT(void);			


void Card_Init(void)	
{	
	GPIO_WiegandPower_INIT();
	
	GPIO_Wiegand_INIT();
	
	//��ʼ��һ����ʱ��:�����жϽ����Ƿ�ʱ
	//GPIO_Wiegand_Timer();
}


void GPIO_Wiegand_Timer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;                       //����ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;               //����TIM�ṹ�����

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);          //ʹ��TIM2����
	TIM_DeInit(TIM2);	      

	//��λʱ��TIM2���ָ�����ʼ״̬
	TIM_TimeBaseStructure.TIM_Period = 200;
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //TIM2ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //������ʽ
	//  ��ʱʱ��T���㹫ʽ��

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);               //��ʼ��
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                         //�����־

	//  �жϷ�ʽ�£�ʹ���ж�Դ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                     //ʹ���ж�Դ
	TIM_Cmd(TIM2,ENABLE);                                        //ʹ��TIM2
	
	//  �������ȷּ���
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //ѡ���ж�ͨ������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //��Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //������ͨ�����ж�
	NVIC_Init(&NVIC_InitStructure);                            //�ṹ���ʼ��
}	


	
static void GPIO_WiegandPower_INIT(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	

	//����ͷ��Դ����		
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_12; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	

	CardRead1_PowerOn;	
	CardRead2_PowerOn;
}	


// ��������  : ΰ����ʼ��
static void GPIO_Wiegand_INIT(void)	
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
		
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);


	/****************����ͷ1***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//����GPIO���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2|EXTI_Line3;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	


	
	/****************����ͷ2***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//����GPIO���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line3|EXTI_Line4;		
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	


	
	/***************** �ⲿ�ж�ʹ������ ***********************/		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  		
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 	
}






void EXTI2_IRQHandler(void)      
{
	if(WGReader_H1D0_Reader==0)	
	{			
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,0,0);		
	}
	else
	{
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,0,1);	
	}
	
	EXTI_ClearITPendingBit(EXTI_Line2);
}  



void EXTI3_IRQHandler(void)      
{
	if(WGReader_H1D1_Reader==0)	
	{				
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,1,0);
	}
	else
	{
		wiegand_reader_data_pin_changed(CARD_READ_DEV1,1,1);	
	}

	
	if(WGReader_H2D0_Reader == 0)
	{	
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,0,0);		
	}
	else
	{
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,0,1);		
	}

	EXTI_ClearITPendingBit(EXTI_Line3);		
}  



void EXTI4_IRQHandler(void)      
{	
	if(WGReader_H2D1_Reader==0)		
	{		
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,1,0);	
	}
	else
	{	
		wiegand_reader_data_pin_changed(CARD_READ_DEV2,1,1);			
	}	

	EXTI_ClearITPendingBit(EXTI_Line4);
}  



	
