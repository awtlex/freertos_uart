#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "mp_protocol_hostif.h"

#define Makepower_Protocol_Open 	1
#define Post_Protocol_Open			0		

#define   POST_VER		   	0x10 //Э��汾	  


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





void tls_protocol_rebuild();

int tls_protomd_parse(struct tls_atcmd_token_t *tok, char *buf, u32 len);

int tls_protocmd_exec(struct tls_atcmd_token_t *tok,char *res_rsp, u32 *res_len);



#endif

