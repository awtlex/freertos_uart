#include <string.h>	
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "spi.h"
#include "flash.h"
#include "24CXXX.h"	

#include "calendar.h"
#include "beep.h"	

//ALIENTEK Mini STM32�����巶������20
//SPIʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
   	
//Ҫд�뵽W25Q64���ַ�������
const u8 *TEXT_Buffer = "MiniSTM32";	
const u8 *IIC_Buffer  = "IIC TEST";			

	
u8 Flash_ID[3];	

u8 datatemp[15];	 
u8 IIC_datatemp[15];	  

TypedefDateTime DateTime;	
	

static void  Wait(void)
{
	volatile u8 i;

	for(i = 0;i<8;i++);
}


 int main(void)
 { 
	u8 key;	
	u16 i=0;		
	u32 start;	
	UART_REC *rec;	
	u16 text_size,IIC_text_size;		
	u8 *ID;		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
				
	delay_init();	    	 //��ʱ������ʼ��	  	
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�		
	SPI_Flash_Init();  	//SPI FLASH ��ʼ�� 	 
	_24CXXX_init();										
	Calendar_Init();	
	Beep_Init();	
			
	memset(datatemp,0,sizeof(datatemp));		
	memset(IIC_datatemp,0,sizeof(IIC_datatemp));	

	ID = SPI_Flash_ReadID();			

	for(i=0;i<3;i++)		
	{			
		Flash_ID[i] = ID[i];	
	}	
	if(Flash_ID[1] == 0x26)		
	{			
		key = 1;	
	}						
	
	text_size 	  = strlen(TEXT_Buffer);			
	IIC_text_size = strlen(IIC_Buffer);
			
	start = 16*4*1024;	//FLASH ��СΪ4M�ֽ�	

#if 0				
						
	while(SPI_Flash_ReadID() != 0x26)	//��ⲻ��W25Q64
	{	
		delay_ms(500);	
		LED1=!LED1;//DS0��˸	
	}
		
#endif		

#if 0				
			
	while(AT24CXX_Check())
	{
		delay_ms(500);
		LED0=!LED0;
	}		
		
#endif						
		
	//BEEP = 0;	
	SPI_Flash_Write((u8*)TEXT_Buffer,start,text_size);	
	_24CXXX_WriteBytes(0,(u8*)IIC_Buffer,IIC_text_size);		
			
	delay_ms(1000);								

	Wait();
					
	SPI_Flash_Read(datatemp,start,text_size);			
	_24CXXX_ReadBytes(0,IIC_datatemp,IIC_text_size);			
	//BEEP = 1;					
	
	GetDateTime(&DateTime);			
	
	while(1)	
	{								
		delay_ms(1000);	
		LED0=!LED0;		
		LED1=!LED1;	
		
	}
}

