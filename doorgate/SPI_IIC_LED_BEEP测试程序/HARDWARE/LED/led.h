#if 1

#ifndef __LED_H
#define __LED_H
#include "sys.h"		

void LED_Init(void);	

#define LED0   PBout(8)	
#define LED1   PEout(0)		
	
#endif


/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#else

#ifndef __BSP_LED_H
#define __BSP_LED_H

/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);
uint8_t bsp_IsLedOn(uint8_t _no);

#endif

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

