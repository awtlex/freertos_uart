#include "FreeRTOS.h"
#include "task.h"

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

void My_Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				
	GPIO_SetBits(GPIOA,GPIO_Pin_8);							

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				
	GPIO_SetBits(GPIOD,GPIO_Pin_2);					
}
	 





#define		LED_RUN_MASK			(0x01<<0)
#define		LED_FUN_MASK			(0x01<<1)		
	
#define		LED_RUN_ON_MASK		(0x01<<4)
#define		LED_FUN_ON_MASK		(0x01<<5)

#define LED_RUN_PERIOD 	M2T(300)				
#define LED_FUN_PERIOD 	M2T(600)				


static u8 current_blink_led;
static u8 current_led_state;	
static u8 current_blink_frq;		

static tls_os_timer_t 	led_run_timer;	
static tls_os_timer_t 	led_fun_timer;	




void Led_Test(void)
{				
	hw_platform_start_led_blink(LED_RUN,LED_RUN_PERIOD);			
	hw_platform_start_led_blink(LED_FUN,LED_FUN_PERIOD);		
}	
	
/**
* @brief	LED���ƽӿ�
* @param[in]  unsigned int led	 LED_RED or  LED_GREEN  or LED_YELLOW
* @param[in]  unsigned int ctrl  0: close   1:open
* @return   none                 
*/
void hw_platform_led_ctrl(u16 led,u16 ctrl)
{
	if (led == LED_RUN)
	{	
		if (ctrl)
		{	
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		}
		else
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_8);			
		}
	}
	else if (led == LED_FUN)		
	{	
		if (ctrl)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
		}
		else	
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_0);	
		}
	}
}



static void led_run_blink_timer_hook(tls_os_timer_t timer)	
{			
	current_led_state ^= LED_RUN_MASK;
	hw_platform_led_ctrl(LED_RUN,current_led_state & LED_RUN_MASK);
}	

static void led_fun_blink_timer_hook(tls_os_timer_t timer)	
{	
	current_led_state ^= LED_FUN_MASK;			
	hw_platform_led_ctrl(LED_FUN,current_led_state & LED_FUN_MASK);
}


/**
 * @brief ��ʼLED��˸ָʾ
 * @param[in] unsigned int		led		
 * @param[in] unsigned short	delay	��˸��ʱ������Ҳ������˸Ƶ��,��λ10ms
 * @note ע��˽ӿ�ֻ����һ��LED����˸������ʵ�ּ���LEDͬʱ��˸
*/
int hw_platform_start_led_blink(u16 led,u16 delay)
{		
	if (led == LED_RUN)
	{	
		if (current_led_state & LED_RUN_ON_MASK)
		{	
			current_led_state |= LED_RUN_MASK;
			tls_os_timer_change(led_run_timer,0);
		}	
		else
		{
			current_led_state |= (LED_RUN_MASK | LED_RUN_ON_MASK);
			tls_os_timer_create(&led_run_timer,  
									delay,			 	
									pdTRUE, 				
									NULL, 			
									led_run_blink_timer_hook); 

			tls_os_timer_start(led_run_timer);	
		}

		return TRUE;	
	}
	else if (led == LED_FUN)	
	{	
		if (current_led_state & LED_FUN_ON_MASK)
		{		
			current_led_state |= LED_FUN_MASK;
			tls_os_timer_change(led_fun_timer,0);	
		}
		else
		{		
			current_led_state |= (LED_FUN_MASK | LED_FUN_ON_MASK);
			tls_os_timer_create(&led_fun_timer, 		 
									delay,	 		
									pdTRUE, 					
									NULL, 		
									led_fun_blink_timer_hook); 	

			tls_os_timer_start(led_fun_timer);	
		}

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
int hw_platform_stop_led_blink(u16 led)	
{			
	if(led == LED_RUN)	
	{	
		hw_platform_led_ctrl(LED_RUN,0);			

		if (current_led_state & LED_RUN_ON_MASK)
		{
			current_led_state &= ~LED_RUN_MASK;
			current_led_state &= ~LED_RUN_ON_MASK;
			tls_os_timer_stop(led_run_timer);
		}
						
		return TRUE;
	}
	else if (led == LED_FUN)	
	{
		hw_platform_led_ctrl(LED_FUN,0);			

		if (current_led_state & LED_FUN_ON_MASK)
		{
			current_led_state &= ~LED_FUN_MASK;
			current_led_state &= ~LED_FUN_ON_MASK;
			tls_os_timer_stop(led_fun_timer);	
		}
						
		return TRUE;
	}
	else
	{
		return FALSE;	
	}

}





