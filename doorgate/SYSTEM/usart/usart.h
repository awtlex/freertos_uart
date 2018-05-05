#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//����ʹ��TC��ʽ���������ж�
#define USART_STM32SendIT_TC 

//485�������
//ʹ�õ���ΪGPIOD
#define GPIO485Direction_RCC_CLK			RCC_APB2Periph_GPIOA

#define GPIO485Direction_PORT				GPIOA	
#define GPIO485Direction_PIN				GPIO_Pin_11   

//����	
#define GPIO485Direction_Send()   			GPIO_SetBits(GPIO485Direction_PORT,GPIO485Direction_PIN)				
#define GPIO485Direction_Receive()   		GPIO_ResetBits(GPIO485Direction_PORT,GPIO485Direction_PIN)


//***********************������Ϣ********************************************************
#define 	COM1		   0
#define 	COM2		   1
#define 	COM3		   3
#define 	COM4		   4	

	
#define USART_REC_LEN  		200  	//�����������ֽ���200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	
  	
//����״̬���	
typedef struct _UART_REC
{
	u8 *RecData;
	u8 Rec_Over_Flag; 	
	u16 Len_Count;	
	u16 Rec_Len;	 
	u16 RxTimeout; 			
}UART_REC;	



void tls_uart_rx_disable(int uart_no);

void tls_uart_rx_enable(int uart_no);

void tls_uart_tx_disable(int uart_no);

void tls_uart_tx_enable(int uart_no);

void Uart_SendByte(u8 byte);	

void Uart_SendData(u8 *Buffer, u32 Length);

void STM32_USART1_GPIOConfig(void);

void STM32_USART2_GPIOConfig(void);

void STM32_USART3_GPIOConfig(void);

void STM32_USART5_GPIOConfig(void);	


u8 tls_get_usrt1_isr_count(void);


#endif


