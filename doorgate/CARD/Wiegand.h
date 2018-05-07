#ifndef __WIEGAND_H
#define __WIEGAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"	


#define WIEGAND_UNIT_LENGHT 8	
#define WIEGAND_BIT_LENGHT  66	
#define WIEGAND_BYTE_NUM	(WIEGAND_BIT_LENGHT/WIEGAND_UNIT_LENGHT + 1)	


typedef enum Wiegand_Sequence_Type_t
{
	Wiegand_Sequence_Normal 		= 0x00,//����
	Wiegand_Sequence_Reversal 	= 0x01,//����
	Wiegand_Sequence_NOT			= 0x02,//ȡ��
  Wiegand_Sequence_HighStart 		= 0x04,//�Ӹߣ��ͣ���ʼȡָ��λ��
}Wiegand_Sequence_Type;


	
typedef enum ReadHead_State_t
{
	ReadHead_IDLE 		= 0, 	//����״̬
	ReadHead_RECEIVE	= 1,	//���ڽ���
	ReadHead_FINISH	= 2,	//�������	
}ReadHead_State;

typedef enum CardDevice_State_t
{	
	CardDev_IDLE 		= 0, 	//����״̬	
	CardDev_RECEIVE		= 1,	//���ڽ���
	CardDev_FINISH		= 2,	//�������	
}CardDevice_State;			



typedef struct WIEGAND_PARAM_t		
{	
	CardDevice_State State;			//ˢ��ͷ״̬
	u8 KeyNumber;
	u8 Pos;				//�������洢wiegand���ֽڵ�λ��   
	
	u8 bits[WIEGAND_BYTE_NUM];//��ʵ����		
	u8 num_bits;	
}WIEGAND_PARAM;	

	


// ��������  : ��ȡΰ��ͨ�ŵĿ���
// ��   ��1  : Noʹ�õ�ˢ��ͷ��� 0��ʼ
extern u8 Wiegand_Receive(u8 No);
// ��������  : �Զ���ͷ��ȡ�Ŀ�����Ϣ����
// ��   ��1  : Noʹ�õ�ˢ��ͷ��� 0��ʼ
// ��   ��2  : *IDCardNo ���Ŵ洢��ָ��
// ��    ע	 ���������SysConfig.Wiegand_Sequenceϵͳ���ô�����Ϣ
extern void Wiegand_DealData(u8 No,u8 *IDCardNo,DoorParaStruct *DoorParaPointer);

		
#endif 
