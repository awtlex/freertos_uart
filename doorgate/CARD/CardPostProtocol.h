#ifndef	__POSTPRODAPROFUNC_H
#define __POSTPRODAPROFUNC_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
	
#if ReadCard_Open
  
//ʱ�α� ���ݳ�ʼ��
extern void Init_TimeListSeg(u8 DoorNumber);
//����ʱ��-׼��ʱ���б�
extern void Init_SetWeekListSeg1(u32 EepAddr);

extern void Card_PostProtocol(void);
  
#endif
  
#ifdef __cplusplus
}
#endif

#endif


