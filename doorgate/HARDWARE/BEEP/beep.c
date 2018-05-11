#include "beep.h"

static void GPIO_BEEP_INIT(void);


void Beep_Init(void)
{	
	GPIO_BEEP_INIT();
	GPIO_BEEP_ClOSE();	
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
	
}



void hw_platform_beep_ctrl(unsigned short delay,unsigned int beep_freq)
{
	int i;
	for (i = 0; i < (delay*beep_freq)/2000;i++)
	{	
		GPIO_BEEP_OPEN();
		delay_us(1000000/beep_freq);
		GPIO_BEEP_ClOSE();
		delay_us(1000000/beep_freq);
	}
}



