#ifndef __PROTOCOLBASE_H
#define __PROTOCOLBASE_H	
#include "sys.h"	
#include "mp_protocol_hostif.h"

#define ASCII_LEN(len)	  (len*2)		

//�������ݷ��صĳ���(��λ:�ֽ�)
#define NO_RESP_DATA	0x00
#define SET_SUCCEED_RTN_LEN   0	

#define GET_HANDLE_POS_RTN_LEN			2
#define GET_CARD_BIT_RTN_LEN  			2	
#define GET_ARMY_PARAM_RTN_LEN  		4		
#define GET_ALARM_PARAM_RTN_LEN   		4	
#define GET_ARMYING_STA_RTN_LEN		20
#define	GET_USER_DEF_DATA_RTN_LEN		0
#define	GET_VOICE_MASK_STA_RTN_LEN	3	
#define	GET_SWITCH_CONFIG_RTN_LEN		0		
#define	GET_ARMY_CONFIG_RTN_LEN		0
#define GET_FLASH_ID_RTN_LEN  			0				
#define GET_DOOR_LOG_RTN_LEN  			0					
#define GET_USER_PASS_RTN_LEN  		8			
	


#define GET_SYS_TIME_RTN_LEN	8
#define SET_SYS_TIME_RTN_LEN	0	

 
#define GET_LIST_RTN_LEN		16	


//�����ͳ���
typedef enum HANDLE_POS
{
	HANDLE_RIGHT = 2,
	HANDLE_LEFT = 1,	
}HANDLE_POS_T; 	


//�����ͳ���
typedef enum ARMY_TYPE
{
	DEVICE_ARMING = 1,
	DEVICE_UN_ARMING = 0,
	DEVICE_AUTO_ARMING = 0,
	DEVICE_MAN_ARMING = 1,	
}ARMY_TYPE_T; 	


//�Զ��������ֶ�����
typedef enum ALARM_TYPE
{
	ALARM_MASK_ON = 1,
	ALARM_MASK_OFF = 0,	
	ALARM_REVOKE_BY_KEY = 1,	
	ALARM_REVOKE_NO_KEY = 0,	
}ALARM_TYPE_T; 	


typedef struct SWITCH_IN_PARAM
{
	u8 state;
	u16 sec;
	u8 rele_sta;//����״̬
}SWITCH_IN_PARAM_T;


typedef struct EXPRESS_PARAM
{	
	u8 num;	
	u8 param[17];
	u8 sign[17];		
}EXPRESS_PARAM_T;	


typedef struct SWITCH_OUT_PARAM
{	
	u8 state;
	EXPRESS_PARAM_T argv;				
		
}SWITCH_OUT_PARAM_T;	


int Ariber_GetAuthority(struct tls_protocmd_token_t *tok,
        							u8 *res_resp, u32 *res_len,CMD_0x48_HANDLE_T *cmd);

int Ariber_CancelConfirm(CMD_0x48_HANDLE_T *cmd);	

int Ariber_ModifyPassword(struct tls_protocmd_token_t *tok,
        							u8 *res_resp, u32 *res_len,CMD_0x48_HANDLE_T *cmd);	

int Ariber_GetSysTime(u8 *res_resp);						

int Ariber_SetSysTime(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x49_HANDLE_T *cmd);





int Ariber_GetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4A_HANDLE_T *cmd);

int Ariber_GetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4A_HANDLE_T *cmd);

int Ariber_GetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4A_HANDLE_T *cmd);



	

int Ariber_SetWeekPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);

int Ariber_SetWorkDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);
 
int Ariber_SetRestDayPermitList(struct tls_protocmd_token_t *tok,u8 *res_resp);
 



//4B���������ָ��	

int Ariber_SetHandlePos(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetCardBit(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetArmyParam(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetAlarmParam(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetArmyingSta(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetSystemSta(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetSwitchOut(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetVoiceMark(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	

int Ariber_SetIOSwitchParam(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4B_HANDLE_T *cmd);	



/*4C�����µ�������*/	

//��ȡ���Ұ���
int Ariber_GetHandlePos(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ����Чλ
int Ariber_GetCardBit(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ�Ž�����������
int Ariber_GetArmyParam(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ��������
int Ariber_GetAlarmParam(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);

//��ȡ�貼����״̬
int Ariber_GetArmyingSta(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ�Զ�������
int Ariber_GetUserDefinedData(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ��������״̬
int Ariber_GetVoiceMaskSta(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ�����������������
int Ariber_GetSwitchConfig(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ�Ž����������ò���
int Ariber_GetArmyConfigParam(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡflash id
int Ariber_GetFlashID(struct tls_protocmd_token_t *tok,u8 *res_resp,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ�Ž���Ӧ��־
int Ariber_GetDoorLog(struct tls_protocmd_token_t *tok,u8 *res_resp,,CMD_0x4C_HANDLE_T *cmd);	

//��ȡ�û�����
int Ariber_GetUserPassword(struct tls_protocmd_token_t *tok,u8 *res_resp,,CMD_0x4C_HANDLE_T *cmd);

	

#endif

