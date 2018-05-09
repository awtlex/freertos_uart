#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "mp_protocol_hostif.h"

#define Makepower_Protocol_Open 	1
#define Post_Protocol_Open			0		


typedef struct ProtocolResource_T
{	
	ProtocolStruct 	ProBuffer; 		//������
	//�˿�˵��
	u8  Port_Type;		    	//ͨ�Ŷ˿� =0Ϊ����1,=1Ϊ����2, =10.Ϊ����
	u8  Port_BufferNum;		//ʹ�õĻ������� ��ʱ��ͬͨ�Ŷ˿ڻ�������Ҫ����
	//Э��
	u8 	ResState;			    //��Դʹ��״̬ =0.���� =1.���ڽ������� =2.���ڷ������� =3.׼����η������� =4.һ��Э������������׼�����
	u8  Pro_Status;            //��Դ����״̬
	u8 	Pro_Type;        	    //Э������    ��˾Э�� �ʵ粿Э�� �Ž�Э��
	u16 Pro_Lenth;	        	//ͳ��Э����ֽڳ���
	u16 Pro_RealityLenth;      //�����峤�ȣ���βЭ��ʹ��
	//ָ��
	u8 	*ptr_sending;	        //ָ���ͻ�����

	//�ְ�����
	u8	SendMode;			//����20�ֽ���ʱ200ms
	u8	SendCount;			//�����ֽڼ���
	u8	PacketCount;		//����
	//��ʱ��
	u16 Delay_ms;	

}ProtocolResource;


	
typedef int (* protocmd_proc)(struct tls_protocmd_token_t *tok, char *res_resp, u32 *res_len);

struct tls_protocmd_t {	
    char name;
    u8   flag;
    int (* proc_func)(struct tls_protocmd_token_t *tok, char *res_resp, u32 *res_len);
};







int tls_protomd_parse(struct tls_atcmd_token_t *tok, char *buf, u32 len);

int tls_protocmd_exec(struct tls_atcmd_token_t *tok,char *res_rsp, u32 *res_len);



#endif

