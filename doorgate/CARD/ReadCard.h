#ifndef __READCARD_H
#define __READCARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
#include "globe.h"
	
#if ReadCard_Open

#define ReadCardNum		4
typedef __packed struct
{
  IDCardStruct 	OldIDCard;				//ˢ������;
  
  u16   Delay_ms;
}ReadCardStruct;
extern 	ReadCardStruct	ReadCard[ReadCardNum];

//////////////////
//�ڶ�ʱ����ʹ��
extern void ReadCard_TimerDeal(void);

extern void ReadCard_Init(u8 GpioOpen);
//��������  :ϵͳ��������״̬ ˢ������;�ڶ���״̬�� ˢ���ϱ�
extern void ReadCard_Work(void);
	
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
