#ifndef MP_CARD_TASK_H
#define MP_CARD_TASK_H		
#include "sys.h"	


enum
{
	Wiegand_Sequence_Normal 		= 0x00,//����
	Wiegand_Sequence_Reversal 	= 0x01,//����
	Wiegand_Sequence_NOT			= 0x02,//ȡ��
  Wiegand_Sequence_HighStart 		= 0x04,//�Ӹߣ��ͣ���ʼȡָ��λ��
};

enum	
{
	SysState_REALTIME 		= 0,	//����״̬	
	SysState_CONFIG 		= 1,	//����״̬
	SysState_READCARD		= 2,	//����״̬
	SysState_DEBUG			= 3,	//����״̬	
};


//4B����Կ��Ĳ���
typedef struct tls_card_cmd_4b
{
	u8 Card_ValidBit;
	u8 Card_EndianSet;	
}tls_card_cmd_4b_t;	



typedef struct tls_card_dev
{
	tls_card_cmd_4b_t *card_cmd;


	CMD_0x4B_HANDLE_T *card_cmd;	

	 
	tls_os_mailbox_t        rev_mailbox;			
	s16 (*card_rev_callback)(char *buf, u16 len);	
	
}tls_card_dev_t;

void tls_card_dev_init(void);				


#endif

