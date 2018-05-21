#include "beep.h"
#include "delay.h"	

static void GPIO_BEEP_INIT(void);

static void beep_delay_us(unsigned int time)
{ 	   
	unsigned int i=0;  
	while(time--)
	{
		i=8;  
		while(i--) ;    
	}
}



void Beep_Init(void)
{	
	GPIO_BEEP_INIT();
	
	BEEP_OFF;
}

static void GPIO_BEEP_INIT(void)
{	
	//������
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clocks ,����ʹ�õ���GPIO�ڵ�RCCʱ��
	RCC_APB2PeriphClockCmd( GPIO_BEEP_RCC_CLK, ENABLE);
	
	//PE_15�˿ڳ�ʼ��Ϊ��©���	RUN
	GPIO_InitStructure.GPIO_Pin =  GPIO_BEEP_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(GPIO_BEEP_PORT, &GPIO_InitStructure);
}


//���������Գ���
void Beep_Test(void)
{		
	hw_platform_beep_ctrl(250,1000);		
	delay_ms(100);		
	
	hw_platform_beep_ctrl(250,1000);	
	delay_ms(50);
	
	hw_platform_beep_ctrl(250,1000);		
	
}



void hw_platform_beep_ctrl(unsigned short delay,unsigned int beep_freq)
{
	int i;
	for (i = 0; i < (delay*beep_freq)/2000;i++)
	{		
		BEEP_ON;
		beep_delay_us(1000000/beep_freq);	
		BEEP_OFF;			
		beep_delay_us(1000000/beep_freq);
	}
}



