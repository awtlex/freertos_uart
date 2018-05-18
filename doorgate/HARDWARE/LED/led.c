#include "led.h"
#include "delay.h"
#include "param.h"	
#include "mp_osal_rtos.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void Led_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	//ʹ��PB,PE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_8); 					 //PB.5 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOE, &GPIO_InitStructure);				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_0); 					 //PE.5 ����� 
}



void Led_Test(void)
{	
	u8 i;
	u8 Blink_Num = 8;//��������
	
	for(i=0;i<Blink_Num;i++)	
	{		
		delay_ms(1000);	
		LED0=!LED0;
		LED1=!LED1;			
	}
}	


static u8	current_blink_led;
static u8	current_led_state;
static u8   current_blink_frq;
static u16  current_timer_cnt;	
static u8 	led_running;

static tls_os_timer_t 	led_timer;	
#define TIMER_LED_PERIOD 	M2T(500) 		

	
/**
* @brief	LED���ƽӿ�
* @param[in]  unsigned int led	 LED_RED or  LED_GREEN  or LED_YELLOW
* @param[in]  unsigned int ctrl  0: close   1:open
* @return   none                 
*/
void hw_platform_led_ctrl(u16 led,u16 ctrl)
{
	if (led == LED0)
	{
		if (ctrl)
		{
			GPIO_ResetBits(GPIOF, GPIO_Pin_6);
		}
		else
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_6);
		}
	}
	else if (led == LED1)	
	{	
		if (ctrl)
		{
			GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		}
		else
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_7);
		}
	}
}



static void led_blink_timer_hook(tls_os_timer_t timer)	
{	
	current_timer_cnt++;
	if (current_timer_cnt == current_blink_frq*2*10)
	{	
		current_led_state ^= 0x01;
		hw_platform_led_ctrl(current_blink_led,current_led_state);
		current_timer_cnt = 0;
	}
}

/**
 * @brief ��ʼLED��˸ָʾ
 * @param[in] unsigned int		led		
 * @param[in] unsigned short	delay	��˸��ʱ������Ҳ������˸Ƶ��,��λ10ms
 * @note ע��˽ӿ�ֻ����һ��LED����˸������ʵ�ּ���LEDͬʱ��˸
*/
int hw_platform_start_led_blink(u16 led,u16 delay)
{		
	if(!led_running)
	{
		current_timer_cnt = 0;
		current_blink_led = led;
		current_blink_frq = delay;
		current_led_state = 1;
		led_running = 1;
		hw_platform_led_ctrl(current_blink_led,current_led_state);
		
		tls_os_timer_create(&led_timer,  
							TIMER_LED_PERIOD , 
							pdTRUE, 		
							NULL, 
							led_blink_timer_hook); 
		
		tls_os_timer_start(led_timer);

		return TRUE;

	}
	else
	{
		return FALSE;	
	}

}

/**
 * @brief �ر�������˸��LED
*/
int hw_platform_stop_led_blink(void)
{		
	if(led_running)	
	{
		tls_os_timer_stop(led_timer);
		tls_os_timer_delete(led_timer);	
		hw_platform_led_ctrl(current_blink_led,0);
		return TRUE;
	}
	else
	{
		return FALSE;		
	}
}


 
