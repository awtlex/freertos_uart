#ifndef __PARAM_BASE_H
#define __PARAM_BASE_H
#include "sys.h"	
#include "common.h"	

#define DEVICE_NUM				4
#define DEVICE_ADDR_DEFAULT 	1
#define SOFTWARE_VERSION 		"V01.13"	
#define HARDWARE_VERSION 		"V01.00"
#define COMPANY_NAME 			"�������ӹɷ����޹�˾"
#define DEVICE_NAME 			"˫�ſ�����";



//����  : ϵͳ���ò���   ��ƿռ�Ϊ20�ֽ�
typedef struct	DEVICE_INFO_t
{	
	u16 Device_Addr[DEVICE_NUM];						
	u8	SoftWare_Ver[7];			
	u8  HardWare_Ver[7];       		
	u8	Company_Name[21];						
	u8	Device_Name[21];	
	u32	U_ID;		
}DEVICE_INFO;		



void Device_Info_Default(void);		

void Device_Info_Get(DEVICE_INFO *info);

u16 Device_Addr_Info_Get(u8 num);

void Device_Addr_Info_Set(u8 num,u16 addr);




#endif

