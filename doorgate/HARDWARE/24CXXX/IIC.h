 
/*********************************************************************************************************
** Function name:          EEPROM	   
** Descriptions:          �ʺ���AT24C02~AT24C1024                                                 
** input parameters:        ��
** output parameters:       ��      
** Returned value:          0	 
** Created by:		    �����
    
** Modified date:     2013.8.15     
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
	

#ifndef __IIC_H__
#define __IIC_H__
#include "sys.h"

//���ܿ���
#define __IO_IIC 1
#if __IO_IIC == 1

//�˿ڶ��弰����
 		
#define IIC_SCK_HIGH()    GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define IIC_SCK_LOW()     GPIO_ResetBits(GPIOB,GPIO_Pin_6)


/*�˿�����  SDA:          PB7
           		     	 SCK(SCL):  PB6      */

#define IIC_SDA_OUT()    {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}
#define IIC_SDA_IN()     {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}

#define IIC_SDA_HIGH()   GPIO_SetBits(GPIOB,GPIO_Pin_7)	
#define IIC_SDA_LOW()    GPIO_ResetBits(GPIOB,GPIO_Pin_7)		
#define IIC_SDA_GET()    PBin(7)				


//��������
#define IIC_SEND_ACK		0x10	
#define IIC_SEND_NOACK		0x11

#define IIC_STATE_NOACK	0x20
#define IIC_STATE_ACK		0x21

	
//��������
extern unsigned char IIC_State;


//��������
#define IIC_StateGet()	IIC_State



extern void IIC_Init(void);                                  /*������IIC��ʼ��*/
extern void IIC_Start(void);                                /*��IIC*/
extern void IIC_Stop(void);                              /*�ر�IIC*/
extern void IIC_WriteByte(unsigned char Data);          /* IICд����*/
extern unsigned char IIC_ReadByte(unsigned char ACK);  /* IIC������*/

#else


#endif	//#if __IO_IIC == 1





#endif	//#ifndef __IIC_H__
