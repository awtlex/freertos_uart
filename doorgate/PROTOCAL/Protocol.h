#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"		
#include "mp_protocol_hostif.h"	

#define Makepower_Protocol_Open 	CFG_OFF	
#define Post_Protocol_Open			CFG_ON				

#define POST_VER	0x20 //Э��汾	  

#define PROTOCOL_HEAD_LEN	13//Э��ͷ���̶�����
#define PROTOCOL_TAIL_LEN	5 //Э��β���̶�����
#define	PROTOCOL_HEAD	0x7E
#define PROTOCOL_TAIL	0x0D	
	


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



