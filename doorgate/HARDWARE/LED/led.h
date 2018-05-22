#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
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
#define LED0 PBout(8)	// PA8	
#define LED1 PEout(0)	// PD2	


#define LED3 PAout(8)	// PA8
#define LED4 PDout(2)	// PD2		


#define LED_RUN 0	// PA8	
#define LED_FUN 1	// PD2


void My_Led_Init(void);


void Led_Init(void);//��ʼ��

void Led_Test(void);	

void hw_platform_led_ctrl(u16 led,u16 ctrl);	

int hw_platform_start_led_blink(u16 led,u16 delay);	

int hw_platform_stop_led_blink(u16 led);	



		 				    
#endif

	
