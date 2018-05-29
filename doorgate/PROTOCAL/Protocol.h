#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "param.h"	
#include "param_base.h"
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
	
typedef struct CMD_0x4D_0xE0_PARAM{
	
}CMD_0x4D_0xE0_PARAM_T;	

typedef struct CMD_0x4D_HANDLE{	
	CMD_0x4D_0xE0_PARAM_T param_0xE0;	
}CMD_0x4D_HANDLE_T;	


//0x48�����type
#define CMD_0x48_GET_AUTHOR 	0xE0	
#define CMD_0x48_DEL_AUTHOR 	0xE1	
#define CMD_0x48_CHG_PASSWD 	0xE2	


typedef struct CMD_0x4B_0xE0_PARAM{	

}CMD_0x4B_0xE0_PARAM_T;

typedef struct CMD_0x4B_0xE1_PARAM{

}CMD_0x4B_0xE1_PARAM_T;

typedef struct CMD_0x4B_0xE2_PARAM{	
	
}CMD_0x4B_0xE2_PARAM_T;



typedef struct CMD_0x48_HANDLE{
	CMD_0x4B_0xE0_PARAM_T param_0xE0;
	CMD_0x4B_0xE1_PARAM_T param_0xE1;
	CMD_0x4B_0xE2_PARAM_T param_0xE2;		
}CMD_0x48_HANDLE_T;




//0x49�����type
#define CMD_0x49_CORRECT_TIME 			0xE0	
#define CMD_0x49_SET_WORK_DAY_IN 		0xE1	
#define CMD_0x49_SET_REST_DAY_IN 		0xE2
#define CMD_0x49_SET_WEEK_AUTH_IN 	0xF1
#define CMD_0x49_USER_AUTHORIZE		0xE3		
#define CMD_0x49_USER_UNAUTHORIZE 	0xE4	
#define CMD_0x49_REST_DAY_IN_WEEK 	0xEA	
#define CMD_0x49_ADD_HOLIDAY			0xEB
#define CMD_0x49_DEL_HOLIDAY 			0xEC		
#define CMD_0x49_OPEN_DOOR 			0xED	
#define CMD_0x49_RECORD_MEM_ADDR 		0xF0			
#define CMD_0x49_SET_PARAM 			0xFC

typedef struct CMD_0x49_0xE0_PARAM{	
	
}CMD_0x49_0xE0_PARAM_T;

typedef struct CMD_0x49_0xE1_PARAM{
	
}CMD_0x49_0xE1_PARAM_T;

typedef struct CMD_0x49_0xE2_PARAM{	
	
}CMD_0x49_0xE2_PARAM_T;	

typedef struct CMD_0x49_0xE3_PARAM{	

}CMD_0x49_0xE3_PARAM_T;

typedef struct CMD_0x49_0xE4_PARAM{

}CMD_0x49_0xE4_PARAM_T;

typedef struct CMD_0x49_0xEA_PARAM{	
	
}CMD_0x49_0xEA_PARAM_T;	

typedef struct CMD_0x49_0xEB_PARAM{	
		
}CMD_0x49_0xEB_PARAM_T;

typedef struct CMD_0x49_0xEC_PARAM{	
		
}CMD_0x49_0xEC_PARAM_T;

typedef struct CMD_0x49_0xED_PARAM{	
		
}CMD_0x49_0xED_PARAM_T;

typedef struct CMD_0x49_0xF0_PARAM{	
		
}CMD_0x49_0xF0_PARAM_T;

typedef struct CMD_0x49_0xF1_PARAM{	
		
}CMD_0x49_0xF1_PARAM_T;

typedef struct CMD_0x49_0xFC_PARAM{	
		
}CMD_0x49_0xFC_PARAM_T;	


typedef struct CMD_0x49_HANDLE{	
	CMD_0x49_0xE0_PARAM_T param_0xE0;
	CMD_0x49_0xE1_PARAM_T param_0xE1;
	CMD_0x49_0xE2_PARAM_T param_0xE2;		
	CMD_0x49_0xE3_PARAM_T param_0xE3;
	CMD_0x49_0xE4_PARAM_T param_0xE4;
	CMD_0x49_0xEA_PARAM_T param_0xEA;	
	CMD_0x49_0xEB_PARAM_T param_0xEB;
	CMD_0x49_0xEC_PARAM_T param_0xEC;
	CMD_0x49_0xED_PARAM_T param_0xED;	
	CMD_0x49_0xF0_PARAM_T param_0xF0;	
	CMD_0x49_0xF1_PARAM_T param_0xF1;
	CMD_0x49_0xFC_PARAM_T param_0xFC;			
}CMD_0x49_HANDLE_T;


//0x4A�����type	
#define CMD_0x4A_GET_WORK_DAY_IN 		0xE3		
#define CMD_0x4A_GET_REST_DAY_IN 		0xE4
#define CMD_0x4A_GET_AUTH_USER_INFO 	0xE5	
#define CMD_0x4A_GET_POINT_USER		0xE6	
#define CMD_0x4A_GET_REST_OF_WEEK 	0xE9
#define CMD_0x4A_GET_HOLIDAY 			0xEA
#define CMD_0x4A_GET_WEEK_IN 			0xEB		


	
typedef struct CMD_0x4A_0xE3_PARAM{

}CMD_0x4A_0xE3_PARAM_T;

typedef struct CMD_0x4A_0xE4_PARAM{

}CMD_0x4A_0xE4_PARAM_T;

typedef struct CMD_0x4A_0xE5_PARAM{
	
}CMD_0x4A_0xE5_PARAM_T;

typedef struct CMD_0x4A_0xE6_PARAM{

}CMD_0x4A_0xE6_PARAM_T;

typedef struct CMD_0x4A_0xE9_PARAM{

}CMD_0x4A_0xE9_PARAM_T;

typedef struct CMD_0x4A_0xEA_PARAM{

}CMD_0x4A_0xEA_PARAM_T;

typedef struct CMD_0x4A_0xEB_PARAM{
		
}CMD_0x4A_0xEB_PARAM_T;	


typedef struct CMD_0x4A_HANDLE{
	CMD_0x4A_0xE3_PARAM_T param_0xE3;
	CMD_0x4A_0xE4_PARAM_T param_0xE4;	
	CMD_0x4A_0xE5_PARAM_T param_0xE5;
	CMD_0x4A_0xE6_PARAM_T param_0xE6;
	CMD_0x4A_0xE9_PARAM_T param_0xE9;
	CMD_0x4A_0xEA_PARAM_T param_0xEA;
	CMD_0x4A_0xEB_PARAM_T param_0xEB;		
}CMD_0x4A_HANDLE_T;

	
//0x4B�����type
#define CMD_0x4B_SET_HANDLE_POS		0xE0
#define CMD_0x4B_SET_CARD_BIT			0xE1
#define CMD_0x4B_SET_ARMY_PARAM		0xE2
#define CMD_0x4B_SET_ALARM_PARAM		0xE3
#define CMD_0x4B_SET_ARMYING_STA		0xE4	
#define CMD_0x4B_SET_SYS_STA			0xE5	
#define CMD_0x4B_SET_SWITCH_OUT		0xE6
#define CMD_0x4B_SET_VOICE_MARK		0xE7
#define CMD_0x4B_SET_IO_SWITCH			0xE8		



typedef struct CMD_0x4B_0xE0_PARAM{

	u8 Handle_Pos[2];		
		
}CMD_0x4B_0xE0_PARAM_T;

typedef struct CMD_0x4A_0xE1_PARAM{
	
	u8 Card_Vaild_Bit;			
	u8 Card_Endian;	
	
}CMD_0x4B_0xE1_PARAM_T;

typedef struct CMD_0x4A_0xE2_PARAM{

}CMD_0x4B_0xE2_PARAM_T;

typedef struct CMD_0x4A_0xE3_PARAM{

}CMD_0x4B_0xE3_PARAM_T;

typedef struct CMD_0x4A_0xE4_PARAM{

}CMD_0x4B_0xE4_PARAM_T;

typedef struct CMD_0x4A_0xE5_PARAM{
	u8 System_Sta;
}CMD_0x4B_0xE5_PARAM_T;

typedef struct CMD_0x4A_0xE6_PARAM{

}CMD_0x4B_0xE6_PARAM_T;

typedef struct CMD_0x4A_0xE7_PARAM{

}CMD_0x4B_0xE7_PARAM_T;

typedef struct CMD_0x4A_0xE8_PARAM{
	
}CMD_0x4B_0xE8_PARAM_T;
	

typedef struct CMD_0x4B_HANDLE{
	CMD_0x4B_0xE0_PARAM_T param_0xE0;	
	CMD_0x4B_0xE1_PARAM_T param_0xE1;
	CMD_0x4B_0xE2_PARAM_T param_0xE2;
	CMD_0x4B_0xE3_PARAM_T param_0xE3;
	CMD_0x4B_0xE4_PARAM_T param_0xE4;
	CMD_0x4B_0xE5_PARAM_T param_0xE5;
	CMD_0x4B_0xE6_PARAM_T param_0xE6;	
	CMD_0x4B_0xE7_PARAM_T param_0xE7;
	CMD_0x4B_0xE8_PARAM_T param_0xE8;
}CMD_0x4B_HANDLE_T;



	
//0x4C�����type	
#define CMD_0x4C_GET_HANDLE_POS		 	0xE0//��ȡ���Ұ���λ��
#define CMD_0x4C_GET_CARD_BIT		 		0xE1//��ȡ����Чλ
#define CMD_0x4C_GET_ARMY_PARAM	 		0xE2//��ȡ�Ž�����������
#define CMD_0x4C_GET_ALARM_PARAM		 	0xE3//��ȡ��������
#define CMD_0x4C_GET_ARMY_ON_STA		 	0xE4//��ȡ�貼����״̬
#define CMD_0x4C_GET_DIY_PARAM		 		0xE5//��ȡ�Զ�������
#define CMD_0x4C_GET_VOICE_STA		 		0xE7//��ȡ��������״̬
#define CMD_0x4C_GET_SWITCH_PARAM		 	0xE8//��ȡ�����������������
#define CMD_0x4C_GET_ARMY_CONFIG		 	0xE9//��ȡ�Ž����������ò���
#define CMD_0x4C_GET_USER_PWD				0xEB//��ȡ�û�����
#define CMD_0x4C_GET_FLASH_ID 				0xED//��ȡflash id
#define CMD_0x4C_GET_DOOR_LOG 				0xEF//��ȡ�Ž���Ӧ��־


typedef struct CMD_0x4C_0xE0_PARAM{
	
	u8 Handle_Pos[2];	
		
}CMD_0x4C_0xE0_PARAM_T;


typedef struct CMD_0x4C_0xE1_PARAM{
	
	u8 Card_Vaild_Bit;
	u8 Card_Endian;	
	
}CMD_0x4C_0xE1_PARAM_T;


typedef struct CMD_0x4C_0xE2_PARAM{
	
	u8 Army_Type;
	u8 Army_Time;		
	
}CMD_0x4C_0xE2_PARAM_T;


typedef struct CMD_0x4C_0xE3_PARAM{
	
	u16 Alarm_Arming_Time;
	u8 Alarm_Mask;
	u8 Alarm_Revoke;		
	
}CMD_0x4C_0xE3_PARAM_T;


typedef struct CMD_0x4C_0xE4_PARAM{
	
	u8 Armying_Flag;
	u8 Armying_Type;
	u8 Armying_Tim_Num;
	u8 Armying_N_Byte;
	u8 Armying_Tim[6];	
	u8 Armying_Man;
	u8 Armying_Condition;
	u8 LianDong[8];
	
}CMD_0x4C_0xE4_PARAM_T;


typedef struct CMD_0x4C_0xE5_PARAM{

}CMD_0x4C_0xE5_PARAM_T;


typedef struct CMD_0x4C_0xE7_PARAM{
	
	u8 Door_Num;
	u16 Voice_Mask;	
	
}CMD_0x4C_0xE7_PARAM_T;


typedef struct CMD_0x4C_0xE8_PARAM{
	
	u8 Sw_In_Num;
	SWITCH_IN_PARAM_T Sw_In_Param[8];		
	
	u8 Sw_Out_Num;
	u8 expr_param_num;	
	SWITCH_OUT_PARAM_T Sw_Out_Param[8];
	
}CMD_0x4C_0xE8_PARAM_T;


typedef struct CMD_0x4C_0xE9_PARAM{


}CMD_0x4C_0xE9_PARAM_T;


typedef struct CMD_0x4C_0xEB_PARAM{
	
	u8 code_group;
	u8 use_sta;	
	u8 code[3];	
	u8 code_bkp[3];	
	
}CMD_0x4C_0xEB_PARAM_T;


typedef struct CMD_0x4C_0xED_PARAM{
	
	u8 Flash_ID; 	
}CMD_0x4C_0xED_PARAM_T;


typedef struct CMD_0x4C_0xEF_PARAM{
	
	u8 MainCmd;
	u8 ViceCmd;

}CMD_0x4C_0xEF_PARAM_T;


typedef struct CMD_0x4C_HANDLE{		
	CMD_0x4C_0xE0_PARAM_T param_0xE0;	
	CMD_0x4C_0xE1_PARAM_T param_0xE1;
	CMD_0x4C_0xE2_PARAM_T param_0xE2;
	CMD_0x4C_0xE3_PARAM_T param_0xE3;
	CMD_0x4C_0xE4_PARAM_T param_0xE4;	
	CMD_0x4C_0xE5_PARAM_T param_0xE5;
	CMD_0x4C_0xE7_PARAM_T param_0xE7;
	CMD_0x4C_0xE8_PARAM_T param_0xE8;
	CMD_0x4C_0xE9_PARAM_T param_0xE9;
	CMD_0x4C_0xEB_PARAM_T param_0xEB;
	CMD_0x4C_0xED_PARAM_T param_0xED;
	CMD_0x4C_0xEF_PARAM_T param_0xEF;		
}CMD_0x4C_HANDLE_T;




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
	u16  LENGTH;
	u8 	 HEAD_LEN;
	u8 	 BODY_LEN;	
	u8   TAIL_LEN;
}tls_respon_head;		



int tls_protocol_rebuild(struct tls_protocmd_token_t *tok,u8 *buff,u32 *res_len,u8 err);	

int tls_protocmd_parse(struct tls_protocmd_token_t *tok, char *buf,u32 *res_len);	

int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp, u32 *res_len);	



//����Э��
void makepower_protocol_deal(void);

//�ʵ�Э��
void post_protocol_deal(void);

//Э���ʼ��
void tls_protocol_init(void);	




#endif



