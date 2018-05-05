#include "usart.h"	
#include "string.h"
#include "mp_uart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}


//__use_no_semihosting was requested, but _ttywrch was
_ttywrch(int ch)
{
	ch = ch;	
} 	


#endif 

	
u8 intr_counter = 0;		


static void USART_RS485IO(void);
	


//��������  : STM32���ڹܽ�����
void STM32_USART1_GPIOConfig(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//  ʹ��ʱ��

	/*����1ʹ��GPIOA��9,10��*/ 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);//  ʹ��ʱ��  
	//USART1 TX	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//USART1 Rx (PA.10)����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ʹ����Ӧ�жϣ�	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//RS485ͨ�ŷ���ܿؽ�	
	USART_RS485IO();

	intr_counter = 0;
}




void STM32_USART2_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
		
	/*����2��ӳ��ʹ��GPIOD��5,6��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  ʹ��ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);//  ʹ��ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_USART2 , ENABLE);//��ӳ��	
	//usart2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
	//USART2 Rx  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	//ʹ����Ӧ�жϣ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}



void STM32_USART3_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/*����3��ȫӳ��ʹ��GPIOD��8,9��*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  ʹ��ʱ�� 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);//  ʹ��ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3 , ENABLE); //ȫӳ��  
	//usart3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
	//USART3 Rx  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  

	//ʹ����Ӧ�жϣ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}	



void STM32_USART5_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); 

	/*����3ʹ��GPIOB��10,11��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);//  ʹ��ʱ��
	//USAR5 TX	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	//USAR5 Rx  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  ʹ��ʱ��  
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												  
	NVIC_Init(&NVIC_InitStructure);		

}


//���ܿ�  : ����RS485������ƽ�ͨ�� ����
void USART_RS485IO(void)
{      
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//============USART1 RS485/422DIR���ŷ�������
	RCC_APB2PeriphClockCmd(GPIO485Direction_RCC_CLK, ENABLE);
	//���˿ڳ�ʼ��Ϊ������� 
	GPIO_InitStructure.GPIO_Pin =  GPIO485Direction_PIN; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIO485Direction_PORT, &GPIO_InitStructure); 
	GPIO485Direction_Receive();    //��״̬	
}  



void Uart_SendByte(u8 byte) //����1�ֽ�����
{
	while(!((USART1->SR)&(1<<7)));
	USART1->DR = byte;	 	
}

	
void Uart_SendData(u8 *Buffer, u32 Length)
{
	while(Length != 0)
	{
		while(!((USART1->SR)&(1<<7)));//�ȴ�������
		USART1->DR = *Buffer; 
		Buffer++;
		Length--;
	}
}



static USART_TypeDef* COMX[]={USART1,USART2,USART3,UART4,UART5};	
	
void tls_uart_rx_disable(int uart_no)
{
	USART_ITConfig(COMX[uart_no],USART_IT_RXNE, DISABLE);  	
}

void tls_uart_rx_enable(int uart_no)
{	
   	USART_ITConfig(COMX[uart_no],USART_IT_RXNE, ENABLE);   
}

void tls_uart_tx_disable(int uart_no)
{
	USART_ITConfig(COMX[uart_no],USART_IT_TC, DISABLE);
}
	
void tls_uart_tx_enable(int uart_no)
{	
 	USART_ITConfig(COMX[uart_no],USART_IT_TC, ENABLE);	
}


 
#if EN_USART1_RX   //���ʹ���˽���


void USART1_IRQHandler(void)               //����1�жϷ������
{			
	//ȫ���ж��Ƿ����жϷ���
		

	//�����ж�	
	if( USART_GetITStatus(USART1,USART_IT_TXE) )
	{		
		USART_ClearITPendingBit(USART1,USART_IT_TXE);	

		//����жϷ���
		intr_counter = 0;	
	} 

	
	//�����ж�
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{		
		intr_counter= 1;	
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);				
				
		tls_uart_put_into_buffer(COM1,USART_ReceiveData(USART1));	
	} 
	//���ܲ�����������	
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{			
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
			
		USART_ReceiveData(USART1);			

		tls_uart_isr_rx_callback_x(COM1);			
		
		//����жϽ���
		intr_counter = 0;
	}			



}


u8 tls_get_usrt1_isr_count(void)
{	
	return intr_counter;
}	


#endif	




#if EN_USART2_RX	

void USART2_IRQHandler(void)    
{
		
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{		
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);		
	
		//tls_uart_put_into_buffer(COM2,USART_ReceiveData(USART2));	
				
	} 
	//���ܲ�����������
	else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{		
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
		USART_ReceiveData(USART2);			
			
		//tls_uart_isr_rx_callback_x(COM2);				
	}	

	
}

#endif	


