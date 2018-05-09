#include "ProtocolBase.h"


static void MP_Data_Init(void);
static u8 MP_Data_Ver_Get(void);
static void MP_Data_Ver_Send(void);
static u8 MP_Data_Addr_Get(void);
static void MP_Data_Addr_Send(void);
static void MP_Data_CID1_Send(void);
static void MP_Data_SOI_Send(void);
static void MP_Data_CID2_Send(void);
static void MP_Data_Length_Send(void);
static void MP_Data_Info_Send(void);
static void MP_Data_ChkSum_Send(void);
static void MP_Data_Tail_Send(void);


/**********************************************************************************************************
* Description: 	MP_Data_SendHead()
				MP_Data_SendTail()
				MP_Data_SendDefault()	
* 
* Arguments  : 
*
* Returns    : 
*
**********************************************************************************************************/

void MP_Data_Reply(void)
{		
	//���ظ������ݷ��뻺����		
	MP_Data_SOI_Send();
	MP_Data_Ver_Send();
	MP_Data_Addr_Send();
	MP_Data_CID1_Send();
	MP_Data_Length_Send();
	MP_Data_Info_Send();
	MP_Data_ChkSum_Send();
	MP_Data_Tail_Send();

	//��������������ͨ�����ڷ��ͳ�ȥ
	
}



/**********************************************************************************************************
* Description: 
*			 void MP_Data_Init(void);	
*			 u8 MP_Data_Ver_Get(void);
*			 void MP_Data_Ver_Send(void);
*			 u8 MP_Data_Addr_Get(void);
*			 void MP_Data_Addr_Send(void);
*			 void MP_Data_CID1_Send(void);
*			 void MP_Data_SOI_Send(void);
*
* Arguments  : 
*
* Returns    : 
*
**********************************************************************************************************/

static void MP_Data_Init(void)	
{
	
}

static u8 MP_Data_Ver_Get(void)
{
	
	return ;
}

static void MP_Data_Ver_Send(void)
{
	u8 Ver = MP_Data_Ver_Get();
	
	if(Ver != NULL)
	{
		SendOneHexToTwoAsciiByte(Ver);	
	}
	else//����Ĭ�ϵ�
	{
		SendOneHexToTwoAsciiByte(VERSION_DEF);	
	}
}

static u8 MP_Data_Addr_Get(void)
{
	return ;
}

static void MP_Data_Addr_Send(void)
{
	u8 addr = MP_Data_Addr_Get();

	if(addr == NULL)
	{
		addr = 1;	
	}

	SendOneHexToTwoAsciiByte(addr);	

}

static void MP_Data_CID1_Send(void)
{
	u8 CID1;
	if(MP_Data_Ver_Get() == 0x20)
	{
		CID1=0xD0;	
	}	
	else
	{
		u8 addr = MP_Data_Addr_Get();	
		CID1=0x80+(addr&0x0F);	
	}
	
	SendOneHexToTwoAsciiByte(CID1);
}

static void MP_Data_SOI_Send(void)
{
	SendByte('~');
}


// ��������  : �ʵ粿Э�鷢��֡β
static void MP_Data_Tail_Send(void)	
{		
	
}

static void MP_Data_CID2_Send(void)
{
	
}

static void MP_Data_Length_Send(void)
{
	
}

static void MP_Data_Info_Send(void)
{
	
}


static void MP_Data_ChkSum_Send(void)
{
	
}

