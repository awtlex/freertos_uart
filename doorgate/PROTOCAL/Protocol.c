#include "mp_protocol_hostif.h"
#include "ProtocolBase.h"
#include "param_base.h"
#include "mp_mem.h"		
#include "protocol.h"
#include "BasicFunc.h"		
#include "calendar.h"	
#include "debug.h"	
#include "param.h"	
#include "common.h"


#if Post_Protocol_Open

static int post_protocmd_get_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)	
{		
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];
	
	if(cmd_group != 0xF2)
	{		
		LOG_INFO("cmd_group error\n");		
		return RTN_UNVALID_DATA;		
	}
	
	switch(cmd_type)
	{	
		case CMD_0x4D_GET_DATE:		
								
			*res_len = Ariber_GetSysTime(res_resp);	
			return RTN_NORMAL;

			break;	

		default:
		
			break;
	}
	
	return RTN_UNVALID_DATA;

}


static int post_protocmd_set_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 buff[PROTOCMD_MAX_ARG];
	u8 i;	
	u16 arg_found = tok->arg_found;
		

	for(i=0;i<arg_found;i++)
	{
		buff[i] = tok->arg[i];
	}

	for(i=0;i<arg_found/2;i++)	
	{	
		TwoAscTOHex(buff[i],buff[i+1]);
	}
		
		
		
	if(check_time_format((TypedefDateTime *)buff))	
	{
		return FALSE;
	}

	SetDateTime((TypedefDateTime *)buff);	

	return RTN_NORMAL;
}



static int post_protocmd_get_addr_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}	


static int post_protocmd_set_addr_proc(struct tls_protocmd_token_t *tok,
        										char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;	
}

const u8 SysCreatedDate[12] = __DATE__;	
const u8 SysCreatedTime[8] = __TIME__;

static int post_protocmd_get_version_proc(struct tls_protocmd_token_t *tok,
        											char *res_resp, u32 *res_len)
{
	u8 temp3[12];			
	DEVICE_INFO *info;
	u8 i;
	u16 dev_addr;	
	u32 buflen;

	u8 data_len;
	
	Device_Info_Get(info);
	
	dev_addr = info->Device_Addr[0];		
	u16ToAsc(dev_addr,res_resp);		
	
	MEMCPY(p, info->SoftWare_Ver, sizeof(info->SoftWare_Ver)-1);
	p += sizeof(info->SoftWare_Ver)-1;
	MEMCPY(p, info->HardWare_Ver, sizeof(info->HardWare_Ver)-1);
	p += sizeof(info->HardWare_Ver)-1;	
	MEMCPY(p, info->Company_Name, sizeof(info->Company_Name)-1);
	p += sizeof(info->Company_Name)-1;
	MEMCPY(p, info->Device_Name, sizeo(info->Device_Name)-1);
	p += sizeof(info->Device_Name)-1;f		

	//��������豸Ψһ���кŻ��е�����
	u32ToAsc(info->Uer_ID,temp3);				
	for(i=0;i<sizeof(temp3);i++)	
	{		
		*p++ = temp3[i];
	}			

	MEMCPY(p, SysCreatedDate, sizeof(SysCreatedDate));
	p += sizeof(SysCreatedDate);		
	
	MEMCPY(p, SysCreatedTime, sizeof(SysCreatedTime));
	p += sizeof(SysCreatedTime);			

	
	
	return RTN_NORMAL;
}


static int post_protocmd_set_configID_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int post_protocmd_get_configID_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;	
}


static int post_protocmd_remote_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}



	



static int post_protocmd_get_ariber_ver_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int post_protocmd_set_baudrate_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int post_protocmd_authority_confirm_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{	
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF0)
	{
		return FALSE;
	}

	switch(cmd_type)
	{
		case CMD_0x48_GET_AUTHOR:	

			LOG_INFO(" CMD:0x48 Type:Set Date\n");	

			Ariber_PasswordConfirm(tok,res_resp,res_len);
			
			break;	

		case CMD_0x48_DEL_AUTHOR:

			LOG_INFO(" CMD:0x48 Type:Delete Author\n");

			Ariber_CancelConfirm();		
		
			break;

		case CMD_0x48_CHG_PASSWD:

			LOG_INFO(" CMD:0x48 Type:Change Password\n");

			Ariber_ModifyPassword(tok,res_resp,res_len);	
		
			break;

		default:
	
			break;
		
	}
	return RTN_NORMAL;	
}
	

static int post_protocmd_set_sys_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF1)
	{
		return FALSE;
	}
	
	switch(cmd_type)
	{
		case CMD_0x49_SET_DATE:
	
			LOG_INFO("CMD:0x49 Type:Set Date\n");		
	
			//Ariber_SetSysTime();	

			break;

		default:

			break;
	}
	return RTN_NORMAL;
}

	
static int post_protocmd_get_sys_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];
	
	if(cmd_group != 0xF2)
	{	
		LOG_INFO("cmd_group error\n");		
		return FALSE;	
	}
	
	switch(cmd_type)
	{	
		case CMD_0x4A_GET_DATE:		
				
			*res_len = Ariber_GetSysTime(res_resp);			

			break;

		case CMD_0x4A_GET_DOOR_STA:				
			
			break;

		default:
		
			break;
	}
	
	
	
	return RTN_NORMAL;
}


static int post_protocmd_set_guard_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF3)
	{
		return FALSE;
	}

	switch(cmd_type)
	{
		case CMD_0x4B_SET_VOICE:		

			LOG_INFO(" CMD:0x4B Type:Set Voice\n");	
			
			Ariber_SetVoiceMark();

			break;

		case CMD_0x4B_PLY_VOICE:			

			LOG_INFO(" CMD:0x4B Type:Play Voice\n"); 

			Ariber_PlayVoice();	

			break;

		default:

			break;
	}
	return RTN_NORMAL;
}


static int post_protocmd_get_guard_param_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	u8 cmd_group = tok->arg[0];
	u8 cmd_type  = tok->arg[1];

	if(cmd_group != 0xF4)
	{
		return FALSE;
	}	

	switch(cmd_type)
	{
		case CMD_0x4C_GET_VOICE:		

			LOG_INFO(" CMD:0x4C Type:Get Voice Mark\n");

			Ariber_GetVoiceMark();

			break;

		case CMD_0x4C_GET_FLASH_ID:			

			LOG_INFO(" CMD:0x4C Type:Get Falsh ID\n");
			
			Ariber_GetFlashID();
		
			break;

		default:

			break;
	}
	return RTN_NORMAL;
}

#endif





//��������Э��
#if Makepower_Protocol_Open

static int mp_protocmd_get_device_info_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_set_addr_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_get_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_set_time_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}


static int mp_protocmd_mem_dev_proc(struct tls_protocmd_token_t *tok,
        char *res_resp, u32 *res_len)
{
	return RTN_NORMAL;
}

#endif

	









static struct tls_protocmd_t  protocmd_tbl[] = {			
#if Post_Protocol_Open	
	{ 0x4D, 0, post_protocmd_get_time_proc },				//��ȡʱ��
	{ 0x4E, 0, post_protocmd_set_time_proc },				//����ʱ��
	{ 0x50, 0, post_protocmd_get_addr_proc },				//��ȡ��ַ
	{ 0xE3, 0, post_protocmd_set_addr_proc },				//���õ�ַ
	{ 0x12, 0, post_protocmd_get_version_proc },			//��ȡ�汾
	{ 0xFB, 0, post_protocmd_set_configID_proc },			//���ö������
	{ 0xFC, 0, post_protocmd_get_configID_proc },			//��ȡ�������
	{ 0x45, 0, post_protocmd_remote_proc },				//ң������
	
	{ 0x51, 0, post_protocmd_get_ariber_ver_proc },		//��ȡariber�İ汾
	{ 0xA1, 0, post_protocmd_set_baudrate_proc },			//���ò�����
	{ 0x48, 0, post_protocmd_authority_confirm_proc },	//��ȡȨ��
	{ 0x49, 0, post_protocmd_set_sys_param_proc },		//����ϵͳ����
	{ 0x4A, 0, post_protocmd_get_sys_param_proc },		//��ȡϵͳ����
	{ 0x4B, 0, post_protocmd_set_guard_param_proc },		//�����Ž�����
	{ 0x4C, 0, post_protocmd_get_guard_param_proc },		//��ȡ�Ž�����
#endif 	
		
#if Makepower_Protocol_Open
	{ 0x11, 0, mp_protocmd_get_device_info_proc },	//��ȡ�豸��Ϣ
	{ 0xA1, 0, mp_protocmd_set_addr_proc },		//�����豸��ַ
	{ 0x13, 0, mp_protocmd_get_time_proc },		//��ȡʱ��
	{ 0xA3, 0, mp_protocmd_set_time_proc },		//����ʱ��
	{ 0x37, 0, mp_protocmd_mem_dev_proc }, 		//�洢����
#endif

};		


int protocmd_err_resp(char *buf, int err_code)
{
    int len;
    len = sprintf(buf, "+ERR=%d", -err_code);
    return len;
}


int protocmd_ok_resp(char *buf)
{	
    int len;
    len = sprintf(buf, "OK");	
    return len;
}

	
	
int LCHKSUM_LENID_CMP(u16 len_param)	
{
	u8 temp1 = (len_param >> 0)&0x0F;
	u8 temp2 = (len_param >> 4)&0x0F;
	u8 temp3 = (len_param >> 8)&0x0F;
	u8 temp4 = (len_param >> 12)&0x0F;	
		
	u8 sum = temp1 + temp2 + temp3;			
	
	sum = sum%16;
	
	sum = ((~sum)&0x0f) + 1;			

	
	if(sum == temp4)
	{
		return TRUE;	
	}
	return FALSE;
}

int VERSION_CHK(u16 len_param)	
{	
	return TRUE;	
}


int tls_protocmd_parse(struct tls_protocmd_token_t *tok, char *buf, u32 *res_len)
{	
    u16 remain_len=0;
	u8 i=0,j=0;			
	
	tok->VER = TwoAscTOHex(*(buf+1),*(buf+2));	//(1,2)
	//�汾���
	if(!VERSION_CHK(tok->VER))
	{	
		return RTN_VER_ERR;		
	}						
	
	tok->ADDR	= 	TwoAscTOHex(*(buf+3),*(buf+4));//(3,4)
	tok->CID1	= 	TwoAscTOHex(*(buf+5),*(buf+6));//(5,6)
	tok->CID2	= 	TwoAscTOHex(*(buf+7),*(buf+8));//(7,8)	
	tok->LENGTH = 	TwoCharToInt(TwoAscTOHex(*(buf+9),*(buf+10)),TwoAscTOHex(*(buf+11),*(buf+12)));//(9,10,11,12)
		
	//����У����
	if(!LCHKSUM_LENID_CMP(tok->LENGTH))		
	{				
		return RTN_CMDCHK_ERR;	
	}	

	tok->arg_found = (tok->LENGTH & 0xFFF)/2;		
	
	remain_len = tok->arg_found;	

	if(remain_len >= (PROTOCMD_MAX_ARG - 1))	
		return CMD_ERR_INV_PARAMS;		
				
	while (remain_len > 0)		
	{							
		tok->arg[j] = TwoAscTOHex(*(buf+13+i),*(buf+13+i+1));			
		j++;		
		i=i+2;				
		remain_len--;
	}
	
	LOG_INFO("Cmd = %x\n",tok->CID2); //ִ�е�����
	
	LOG_INFO("argv_cnt = %d\n",tok->arg_found);//��������
	
	LOG_INFO("ARGV = ");//������ֵ

	for(j=0;j<tok->arg_found;j++)				
	{								
		printf("%x ",tok->arg[j]);						
	}	
	printf("\n");			
		
	return 0;		
}
	
	
int tls_protocmd_exec(struct tls_protocmd_token_t *tok,char *res_rsp,u32 *res_len)
{			
	int err;
	struct tls_protocmd_t *protocmd, *match = NULL;

	/* look for AT CMD handle table */
	protocmd = protocmd_tbl;
		
	while(protocmd->name) 		
	{	
		if (protocmd->name == tok->CID2)		
		{			
			match = protocmd;
			break;
		}	
		protocmd++;			
	}


	/* at command handle */
	if (match) 
	{	
		err = match->proc_func(tok, res_rsp, res_len);
	} 
	else 
	{	
		/* at command not found */
		*res_len = sprintf(res_rsp, "+ERR=-2");
			
		err = -CMD_ERR_UNSUPP;
	}

	return err;
}



/**
* @brief	�Է��ͻ������������
* @param[1]  u8 *buff			д��Ļ�����
* @param[2]  u8 *data			д�������
* @param[3]  u8 len			д�����ݵĳ���
* @param[4]  u8 tear_flag		�Ƿ���в��	
* @return   none 
*/

void fill_buff(u8 **buff,u8 data,u8 tear_flag)		
{
	/*
	�������ݲ���ʱ,����һ�ݿ���,ֻ�����˴��������ֵ,
	Ҳ��˵��������,��������û�б䡣
	ָ���ַ�ı仯������Ϊ��������������ˣ�
	����ָ����ָ����ǿ�ĵ�ַ��û��ġ�
	�����ڵ�����ͻ������Ƿݿ�����
	*/
	
	if(tear_flag)
	{		
		**buff = Hi_HexToAsc(data);	
		(*buff)++;
		**buff = Low_HexToAsc(data);		
		(*buff)++;	
	}
	else
	{	
		**buff = data;
		(*buff)++;	
	}	
}






static void tls_protocol_add_head(tls_respon_head *resp_h,u8 *buff,u32 *res_len)	
{	
	u8 *p = buff;//ָ������ͷ��
	u8 Head =  resp_h->SOI;					
	u8 Ver  =  resp_h->VER;
	u8 Adrr =  resp_h->ADR; 	
	u8 CID1 =  resp_h->CID1; 	
	
	//RTN��Length����ǰ����յ�����Ϣ������Ļظ�
	u8 RTN = resp_h->RTN;					
	u16 LEN = resp_h->RESPON_LEN;	
	
	u8 Len_H  = (u8)(LEN>>8);				
	u8 Len_L  = (u8)(LEN);			
	
	u16 Host_Adr = Adrr + (CID1&0x0F)*0x100;	
		
	LOG_INFO("RTN %x,HIGH %x,LOW %x\n",RTN,Len_H,Len_L);			
			
	//���ͷ
	fill_buff(&p,Head,0);				
	fill_buff(&p,Ver,1);						
	fill_buff(&p,Adrr,1);	
	fill_buff(&p,CID1,1);
	fill_buff(&p,RTN,1);						
	//����
	fill_buff(&p,Len_H,1);				
	fill_buff(&p,Len_L,1);							
				
	*res_len = *res_len + PROTOCOL_HEAD_LEN;	
}



static void tls_protocol_add_body(void)
{
		
}


static void tls_protocol_add_tail(u8 *buff,u32 *res_len)
{	
	u8 *p = buff;	
	u16 i,chk_sum,data_cnt;
	u8 tail = PROTOCOL_TAIL;			
	u8 tempSum[2];	
	
	p++;//ȥ��ͷ	
	
	data_cnt = 8*2 + 12;		

	for(i=0;i<data_cnt;i++)	
	{		
		chk_sum += *p++;
	}
		
	chk_sum = ~chk_sum+1;		

	tempSum[0] = (u8)(chk_sum>>8);
	tempSum[1] = (u8)(chk_sum);			

	fill_buff(&p,tempSum[0],1);
	fill_buff(&p,tempSum[1],1);	
	fill_buff(&p,tail,0);

	*res_len = *res_len + PROTOCOL_TAIL_LEN;		
}


//��������д���RTN��LENGTH��Щ��Ӧ��ͬ����µ���ֵ
static void tls_token_reunion(struct tls_protocmd_token_t *tok,
									tls_respon_head *respon_h,
									u8 err)
{
	u8  Head = PROTOCOL_HEAD;				
	u8  RTN  = err;								
	u16 LEN  = 0xF010;	

	respon_h->SOI 			= 	Head;
	respon_h->VER			= 	tok->VER;
	respon_h->ADR 			= 	tok->ADDR;
	respon_h->CID1 			=	tok->CID1;
	respon_h->RTN			=	RTN;	
	respon_h->RESPON_LEN	=	LEN;	
	respon_h->HEAD_LEN		=	PROTOCOL_HEAD_LEN;
	respon_h->TAIL_LEN		=	PROTOCOL_TAIL_LEN;	
}



int tls_protocol_rebuild(struct tls_protocmd_token_t *tok,
							 u8  *buff,
							 u32 *res_len,
							 u8 err)			
{	
	tls_respon_head respon_head;	

	tls_token_reunion(tok,&respon_head,err);			

	tls_protocol_add_head(&respon_head,buff,res_len);	
		
	tls_protocol_add_tail(buff,res_len);
	
}

















void tls_protocol_init(void)			
{
	
}


















//����Э��
void makepower_protocol_deal(void)
{
		
}






























//�ʵ�Э��
void post_protocol_deal(void)
{
		
}














