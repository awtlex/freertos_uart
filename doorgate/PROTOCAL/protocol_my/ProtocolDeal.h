#ifndef __PROTOCOLDEAL_H
#define __PROTOCOLDEAL_H		
#include "sys.h"	


typedef union
{
	u8  	                  DaBuf[MAXPCReLen]; 	
	PostProProStruct     	  PostProDa;   	//�ʵ粿Э��
	MakepowerDoorProStruct  MPDoorProDa;   //�����Ž�Э��
}ProtocolStruct;


typedef struct ProtocolResource_t	
{	
	ProtocolStruct 	ProBuffer; 	   //������
	//�˿�˵��
	u8 	    Port_Type;		       //ͨ�Ŷ˿� =0Ϊ����1,=1Ϊ����2, =10.Ϊ����
	u8 	    Port_BufferNum;	   //ʹ�õĻ������� ��ʱ��ͬͨ�Ŷ˿ڻ�������Ҫ����
	//Э��
	u8 	    ResState;			   //��Դʹ��״̬ =0.���� =1.���ڽ������� =2.���ڷ������� =3.׼����η������� =4.һ��Э������������׼�����
	u8      Pro_Status;           //��Դ����״̬
	u8 	    Pro_Type;         	   //Э������    ��˾Э�� �ʵ粿Э�� �Ž�Э��
	u16 	Pro_Lenth;	           //ͳ��Э����ֽڳ���
	u16     Pro_RealityLenth;     //�����峤�ȣ���βЭ��ʹ��
	//ָ��
	u8 	    *ptr_sending;	        //ָ���ͻ�����

	//�ְ�����
	u8			SendMode;			//����20�ֽ���ʱ200ms
	u8			SendCount;			//�����ֽڼ���
	u8			PacketCount;		//����
	//��ʱ��
	u16     Delay_ms;
	//
	u8      ReceiveStatus;        //AT����ģʽʹ��
  
}ProtocolResource	


u8 CID2_Action(u8 CID2)	;


#endif

