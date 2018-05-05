#ifndef	__PostProtocolDeal_H
#define __PostProtocolDeal_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"

//RTN���ص�ֵ
enum
{
  RTN_NORMAL          = 0x00,     //�����ظ�
  RTN_VER_ERR         = 0x01,     //�汾������
  RTN_CMDCHK_ERR      = 0x02,     //����У���CHKSUMʧ��
  RTN_PARACHAK_ERR    = 0x03,     //�������ֵ��ۼӺ�У��ʧ��
  RTN_CID2_ERR        = 0x04,     //��Ч��CID2
  RTN_UNKNOW_CMD      = 0x05,     //�����ʽ����
  RTN_UNVALID_DATA    = 0x06,     //���ݲ�������Ч����
  RTN_NOACCESS        = 0x07,     //��Ȩ�޽��з���
  RTN_PASSWORD_WRONG  = 0xE0,     //Ȩ��У�����벻��ȷ
  RTN_MODPASSWORD_ERR = 0xE1,     //�������޸�ʧ��
  RTN_MEMORY_FULL     = 0xE2,     //�洢�ռ�����
  RTN_MODPARA_ERR     = 0xE3,     //�޸��ڲ�����ʧ�ܲ��ɹ�
  RTN_MEMORY_EMPTY    = 0xE4,     //�洢�ռ��ѿ�
  RTN_NOHAVE_INFO     = 0xE5,     //�޶�Ӧ��Ϣ��
  RTN_HAVESAME_USER   = 0xE6,     //�ظ��趨��ͬ���û����Ž������ֲ���
  RTN_BluetoothPasswod_Error = 0xE9,	//����������� 
}; 
/*************���ݽṹ*************************/
typedef __packed struct
{	
	u16 lLenth:4;
	u16 mLenth:4;
	u16 hLenth:4;
	u16 lChksum:4;
}Packed_data;
typedef __packed union
{	
	u16  			  word;
	Packed_data	div;
}Taglchksum;

extern u8   RTN;
extern u8   Var;
extern u16	CallSystemAddr;

// ��������  : �ʵ粿Э�鷢������֡ͷ����
// �������  : RTN ����״̬.
extern void SendHead(void);
// ��������  : �ʵ粿Э��ط���У��ͺ�����Ҫ��HEXת��ASC����У��ͣ�
// �������  : len У�����ݳ���.
extern void SendPostChksum(void);
// ��������  : �������ͻ���̶�λ�ò���
// �������  : FixupNo λ�� valus ����.
extern void SendFixupByte(u8 FixupNo,u8 valus);
// ��������  : �ʵ粿Э�鷢�����ݳ���
extern void SendDataLen(void);
// ��������  : �ʵ粿Э�鷢��֡β
extern void SendTail(void);
// ��������  : �ʵ粿Э�����ݳ���У��
extern u8 ChkPostDataSum(void);
// ��������  : �ʵ粿Э��Ӧ��������������
extern void SendDefault(void);
// ��������  : �ʵ粿Э��Ӧ��Ľ�β���� �������� ����У�������β ����������
extern void SendTaiDeal(void);
// ��������  : �ʵ粿Э��������ݵ�У����㷨
// �������  : p1 ҪУ������� ReDaLen ���ݳ���.
static u8 chk_post_chksum(u8  *p1,u16  ReDaLen);
// ��������  : �ʵ粿Э�����
extern void DealPostProtocol(void);
// ��������  : �ʵ粿Э�鴦��
extern void DealPostPro(void);

#ifdef __cplusplus
}
#endif

#endif
