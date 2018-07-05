#ifndef __BSP_H
#define __BSP_H



/* ʹ����Դ�ļ���ʹ��uCOS-III�ĺ���, �����Դ�ļ���Ҫ��ָBSP�����ļ� */
#define  uCOS_EN      1

#if uCOS_EN == 1    
	#include "os.h"   

	#define  ENABLE_INT()      CPU_CRITICAL_EXIT()     /* ʹ��ȫ���ж� */
	#define  DISABLE_INT()     CPU_CRITICAL_ENTER()    /* ��ֹȫ���ж� */
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif



#include "bsp_uart_fifo.h"
#include "bsp_led.h"	
#include "bsp_beep.h"	
#include "bsp_key.h"



/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);


#endif

