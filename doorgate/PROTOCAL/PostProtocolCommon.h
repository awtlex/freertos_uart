#ifndef __PostProtocolCommon_H
#define __PostProtocolCommon_H

#include "system.h"

//ȫ��Ψһ���к�

//�Ų��� ���ݳ�ʼ��
void Init_DoorPara(u8 DoorNumber);

//�㲥�������ȡ�豸��Ϣ
extern void Send_MPJ12(void);
// ���ܿ� 49 ����
extern void Ariber_SetSysParaAndRecord(void);
// ���ܿ�  : 4A ����
extern void Ariber_GetSysParaAndRecord(void);

// ��������  : �ʵ粿Э�鴦�� ����
extern void Common_PostProtocol(void);

#endif

