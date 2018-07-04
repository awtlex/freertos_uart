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

#include "sst26vf032b.h"

#include "demo_spi_flash_fatfs.h"

#include "bsp.h"		


//ALIENTEK Mini STM32�����巶������20
//SPIʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
   	
//Ҫд�뵽W25Q64���ַ�������
const u8 *TEXT_Buffer = "Hetianqi";						
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

 u8 ID;  

 int main(void)
 { 
	u8 key;	
	u16 i=0;		
	u32 start;	
	UART_REC *rec;	
	u16 text_size,IIC_text_size;		
	
	uint8_t read;
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
					
	bsp_Init();	

	
					
	//delay_init();	    	 //��ʱ������ʼ��	  	
	
	//uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600		
			
	//LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�			
	
	SST26_Init();	//SPI FLASH ��ʼ��		

	SPI_Flash_Global_Block_Unlock();		

	ID = SPI_Flash_ReadID();			

	SPI_Flash_Erase_Sector(0);		

	SST26_Test();			
			
#if 0	
	
	_24CXXX_init();												
	Calendar_Init();	
	Beep_Init();			
			
	memset(datatemp,0,sizeof(datatemp));		
	memset(IIC_datatemp,0,sizeof(IIC_datatemp));	
						
	text_size 	  = strlen(TEXT_Buffer);					
	IIC_text_size = strlen(IIC_Buffer);
			
	start = 0;	//FLASH ��СΪ4M�ֽ�		
	
				

				
		
					
						
				
		
	GetDateTime(&DateTime);			

#endif		
	
	DemoFatFS();						


	bsp_StartAutoTimer(1, 500); 	

	while(1)
	{	
		bsp_Idle(); 	/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */
				
		if(bsp_CheckTimer(1))
		{	
			bsp_LedToggle(1);
			bsp_LedToggle(2);	
		}			
	}	

		
}





