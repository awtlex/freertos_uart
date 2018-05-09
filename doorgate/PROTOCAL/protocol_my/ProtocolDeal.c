#include "ProtocolDeal.h"
#include "param.h"	
#include "debug.h"


static void MP_Device_Time_Get(void);	
static void MP_Device_Time_Set(void);
static void MP_Device_Addr_Get(void);
static void MP_Device_Addr_Set(void);
static void MP_Device_BoardCast_Read(void);
static void MP_Device_Param_Get(void);
static void MP_Device_Param_Set(void);
static void MP_MenJing_Param_Get(void);
static void MP_MenJing_Param_Set(void);
static u8 MP_BaudRate_Set(void);


/**********************************************************************************************************
* Description: 	 �Խ���������CID2����ִ��
* 
* Arguments  : 	CID2������
*
* Returns    : 		��������ִ�еĽ��	
*
**********************************************************************************************************/
	
u8 CID2_Action(u8 CID2)	
{
	u8 RTN = RTN_NORMAL;	
	
	switch(CID2)	
	{	
		case MP_DEVICE_TIME_GET:	
			MP_Device_Time_Get();
			break;
		
		case MP_DEVICE_TIME_SET:		
			MP_Device_Time_Set();
			break;	
				
		case MP_DEVICE_ADDR_GET:	
			MP_Device_Addr_Get();
			break;
					
		case MP_DEVICE_ADDR_SET:	
			MP_Device_Addr_Set();
			break;
					
		case MP_BOARDCAST_READ:	
			MP_Device_BoardCast_Read();
			break;
					
		case MP_DEVICE_PARAM_SET:
			MP_Device_Param_Get();
			break;
					
		case MP_DEVICE_PARAM_GET:	
			MP_Device_Param_Set();
			break;
					
		case MP_REMOTE:			
			
			break;
					
		case MP_AUTHENTICATION_GET_O_DEL:	
			
			break;
					
		case MP_REMOTE_PARAM_SET:	
			
			break;
					
		case MP_PARAM_RECORD_GET:
			
			break;
					
		case MP_MENJING_PARAM_SET:	
			MP_MenJing_Param_Set();
			break;
					
		case MP_MENJING_PARAM_GET:	
			MP_MenJing_Param_Get();
			break;
					
		case MP_DEVICE_INFO_GET:	
			
			break;
					
		case MP_BAUDRATE_SET:	
			MP_BaudRate_Set();	
			break;	

		default :	
			
			break;
	}

	return RTN;	
}






/**********************************************************************************************************
* Description: 	CID2��Ӧִ�еĶ���
* 
* Arguments  : 	
*
* Returns    : 		��
*
**********************************************************************************************************/


static void MP_Device_Time_Get()
{
	
}

static void MP_Device_Time_Set()
{
	
}

static void MP_Device_Addr_Get()
{
	
}

static void MP_Device_Addr_Set()
{
		
}

static void MP_Device_BoardCast_Read()
{
		
}

static void MP_Device_Param_Get()
{
	
}
	
static void MP_Device_Param_Set()
{
		
}

static void MP_MenJing_Param_Get()
{
	
}
	
static void MP_MenJing_Param_Set()
{
		
}


static u8 MP_BaudRate_Set(void)	
{
	
}


static u8 *parse_cmd_line(struct tls_uart *uart)
{	
	struct tls_uart_circ_buf *recv = &uart->uart_port->recv;

	//������յ���ͷ����
	if(recv->buf[recv->tail] != '~')		
	{
		LOG_ERROR("Head Recv Err");		
		return SDK_ERR;		
	}

	//
	
	
	 
}



