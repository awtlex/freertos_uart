#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "param.h"	
#include "mp_protocol_hostif.h"

#define Makepower_Protocol_Open 	CFG_OFF	
#define Post_Protocol_Open			CFG_ON				

#define POST_VER	0x20 //Э��汾	  

#define PROTOCOL_HEAD_LEN	13//Э��ͷ���̶�����
#define PROTOCOL_TAIL_LEN	5 //Э��β���̶�����
#define	PROTOCOL_HEAD	0x7E
#define PROTOCOL_TAIL	0x0D	



//0x4D�����type	
#define CMD_0x4D_GET_DATE 		0xE0			


//0x48�����type
#define CMD_0x48_GET_AUTHOR 	0xE0	
#define CMD_0x48_DEL_AUTHOR 	0xE1	
#define CMD_0x48_CHG_PASSWD 	0xE2	


//0x49�����type
#define CMD_0x49_SET_WORK_DAY_IN 		0xE0
#define CMD_0x49_SET_REST_DAY_IN 		0xE1
#define CMD_0x49_SET_WEEK_IN 			0xE2
	


//0x4A�����type	
#define CMD_0x4A_GET_WORK_DAY_IN 		0xE3		
#define CMD_0x4A_GET_REST_DAY_IN 		0xE4
#define CMD_0x4A_GET_WEEK_IN 			0xEB		


//0x4B�����type
#define CMD_0x4B_SET_VOICE		0xE7	
#define CMD_0x4B_PLY_VOICE		0xEA

	
//0x4C�����type		
#define CMD_0x4C_GET_HANDLE_POS		 	0xE0//��ȡ���Ұ���λ��
#define CMD_0x4C_GET_CARD_BIT		 		0xE1//��ȡ����Чλ
#define CMD_0x4C_GET_ARMY_OFF_STA	 		0xE2//��ȡ�Ž�����������
#define CMD_0x4C_GET_ALARM_PARAM		 	0xE3//��ȡ��������
#define CMD_0x4C_GET_ARMY_ON_STA		 	0xE4//��ȡ�貼����״̬
#define CMD_0x4C_GET_DIY_PARAM		 		0xE5//��ȡ�Զ�������
#define CMD_0x4C_GET_VOICE_STA		 		0xE7//��ȡ��������״̬
#define CMD_0x4C_GET_SWITCH_PARAM		 	0xE8//��ȡ�����������������
#define CMD_0x4C_ARMY_PARAM		 		0xE9//��ȡ�Ž����������ò���
#define CMD_0x4C_GET_USER_PWD				0xEB//��ȡ�û�����
#define CMD_0x4C_GET_FLASH_ID 				0xED//��ȡflash id
#define CMD_0x4C_GET_DOOR_LOG 				0xEF//��ȡ�Ž���Ӧ��־




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

typedef struct tls_protocmd_t {	
    char name;
    u8   flag;	
    int (* proc_func)(struct tls_protocmd_token_t *tok, char *res_resp, u32 *res_len);
}tls_protocmd;


typedef struct tls_respon_head_t {	
	u8 	 SOI;		
	u8   VER;
	u8 	 ADR;
	u8 	 CID1;		
	u8 	 RTN;		
	u16  RESPON_DATA;
	u8 	 HEAD_LEN;
	u8 	 BODY_LEN;	
	u8   TAIL_LEN;
}tls_respon_head;		

		
	
int tls_protocol_rebuild(struct tls_protocmd_token_t *tok,u8 *buff,u32 *res_len,u8 err);	

int tls_protocmd_parse(struct tls_protocmd_token_t *tok, char *buf,u32 *res_len);	

int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp, u32 *res_len);	



#endif

