#include <string.h>	
#include "flash.h" 
#include "led.h"	
#include "spi.h"
#include "delay.h"   



//��ʼ��SPI FLASH��IO��
void SPI_Flash_Init(void)	
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		
	GPIO_SetBits(GPIOC,GPIO_Pin_4);		
	
	SPI1_Init();		   //��ʼ��SPI
				
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);	//����Ϊ18Mʱ��,����ģʽ
	//SPI_FLASH_TYPE=SPI_Flash_ReadID();//��ȡFLASH ID.
}  	


const u8 *Flash_Test = "Flash Test";			
u8 Flash_Buf[15];		 


void SPI_Flash_Test(void)	
{			
	u16 len;
	u32 start;	

	len = strlen(Flash_Test)	;		
	start = 16*4*1024;	//FLASH ��СΪ4M�ֽ�		

	while(SPI_Flash_ReadID() != 0x26)	
	{				
		delay_xms(500);		
		LED0=!LED0;	
	}
		
	SPI_Flash_Write((u8*)Flash_Test,start,len);	

	delay_xms(500);	
	
	SPI_Flash_Read(Flash_Buf,start,len);		

	delay_xms(500);	
}

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   	
	SPI_FLASH_CS=0;                            //ʹ������   
	SPI1_ReadWriteByte(RDSR);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
} 	
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	u8 dummy_byte = 0x00;	
	
	SPI_Flash_Wait_Busy();	

	SPI_FLASH_CS=0; 	                        //ʹ������   
	SPI1_ReadWriteByte(WRSR);   				//����дȡ״̬�Ĵ�������    
//	SPI1_ReadWriteByte(dummy_byte);	
	SPI1_ReadWriteByte(sr);               	   //д��һ���ֽ�  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
}   
//SPI_FLASHдʹ��	
//��WEL��λ   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(WREN);      			//����дʹ��  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
} 		
//SPI_FLASHд��ֹ	
//��WEL����  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(WRDI);     //����д��ָֹ��    
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
} 			    
//��ȡоƬID W25X16��ID:0XEF14
u8 SPI_Flash_ReadID(void)
{	
	u8 partialInfo[3];
		  		
	SPI_FLASH_CS=0;				    
	SPI1_ReadWriteByte(JEDECID);//���Ͷ�ȡID����	     	 			   
	partialInfo[0] = SPI1_ReadWriteByte(0xFF);  	
	partialInfo[1] = SPI1_ReadWriteByte(0xFF);	 
	partialInfo[2] = SPI1_ReadWriteByte(0xFF);	 	
	SPI_FLASH_CS=1;				
	
	return partialInfo[1];			
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{
 	u16 i;    		
	SPI_Flash_Wait_Busy();
		
	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(READ);         			//���Ͷ�ȡ����   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   	
    for(i=0;i<NumByteToRead;i++)
	{ 		
        pBuffer[i]=SPI1_ReadWriteByte(0xFF);   //ѭ������  
    }
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
}  



//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   

void flash_hal_write_address(u32 address)
{    
    u8 addr_hi = (u8)((address)>>16);	
    u8 addr_mi = (u8)((address)>>8);
    u8 addr_lo = (u8)(address);		

    SPI1_ReadWriteByte( addr_hi );
    SPI1_ReadWriteByte( addr_mi );
    SPI1_ReadWriteByte( addr_lo );	
    
    return;
}
	

void flash_hal_write( u8 *buffer,u16 count )
{
    while( count-- )
    {	
        SPI1_ReadWriteByte( *( buffer++ ) );          
    }

    return;
}



void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{	
	u16 i; 
	
	SPI_Flash_Wait_Busy();

	SPI_FLASH_Write_Enable();

	SPI_FLASH_CS=0; 
	SPI1_ReadWriteByte(PP);	
	
	if( NumByteToWrite <= FLASH_PAGE_SIZE)	
	{
		SPI1_ReadWriteByte((u8)((WriteAddr)>>16));  //����24bit��ַ    
		SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
		SPI1_ReadWriteByte((u8)WriteAddr); 	 

	    for(i=0;i<NumByteToWrite;i++)
		{ 		
			SPI1_ReadWriteByte(pBuffer[i]);   //ѭ������  
	    }
	}
	else
	{
		u8 i;		
		u8 repeat = NumByteToWrite / FLASH_PAGE_SIZE;
		u8 repeat_over = NumByteToWrite % FLASH_PAGE_SIZE;
		
		for ( i = 0; i < repeat; i++ )
		{
			flash_hal_write_address( WriteAddr + ( i * FLASH_PAGE_SIZE ) );
			flash_hal_write( pBuffer + ( i * FLASH_PAGE_SIZE ),FLASH_PAGE_SIZE );
		}		
		if ( repeat_over )
		{
			flash_hal_write_address( WriteAddr + ( repeat * FLASH_PAGE_SIZE ) );
			flash_hal_write( pBuffer + ( repeat * FLASH_PAGE_SIZE ), repeat_over );
		}
	}
	SPI_FLASH_CS=1; 	
	
	return;
}




//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   	
  	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(CE);        //����Ƭ��������  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
	SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   		
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   	
  	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(SE);      				//������������ָ�� 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   	
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
    SPI_Flash_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void SPI_Flash_Wait_Busy(void)   
{   	
	while (SPI_Flash_ReadSR()&STATUS_BUSY);   // �ȴ�BUSYλ���
}  



void SPI_Flash_Global_Block_Unlock( void )
{	
    SPI_FLASH_Write_Enable();
    SPI_FLASH_CS=0; 
    SPI1_ReadWriteByte( ULBPR );	
    SPI_FLASH_CS=1; 			

    return;
}






/*

	ע��:
	
		Flash �ı��ԭ����ֻ�ܽ� 1 дΪ 0��
	�����ܽ� 0 дΪ 1�������� Flash ���֮ǰ��
	���뽫��Ӧ�Ŀ������
	�������Ĺ��̾��ǰ�����λ��дΪ 1 �Ĺ��̣�
	���ڵ������ֽڱ�Ϊ 0xFF��
	
*/














