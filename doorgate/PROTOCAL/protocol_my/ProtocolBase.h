#ifndef __PROTOCOLBASE_H
#define __PROTOCOLBASE_H	
#include "sys.h"	

//Ĭ�ϵİ汾
#define VERSION_DEF 0x20

/*���Ʊ�ʶ��(�豸����)��						*/
/*�������ӹ�˾�ɼ������Ϳ�Ϊ 90H��D0H��*/
/*��λ�������ⷢ�ʹ��������ͣ�			*/
/*���ɼ���ֻ�������� D0H						*/
	
//CID1	
#define	MP_MENJING_CMD		0x8x
#define	MP_					0xD0

//CID2	
#define	MP_DEVICE_TIME_GET 			0x4D	
#define MP_DEVICE_TIME_SET				0x4E
#define MP_DEVICE_ADDR_GET				0x50
#define MP_DEVICE_ADDR_SET				0xE3
#define MP_BOARDCAST_READ				0x12
#define	MP_DEVICE_PARAM_SET			0xFB
#define	MP_DEVICE_PARAM_GET			0xFC	
#define	MP_REMOTE						0x45
#define	MP_AUTHENTICATION_GET_O_DEL	0x48
#define	MP_REMOTE_PARAM_SET			0x49
#define	MP_PARAM_RECORD_GET			0x4A
#define	MP_MENJING_PARAM_SET			0x4B
#define	MP_MENJING_PARAM_GET			0x4C
#define	MP_DEVICE_INFO_GET				0x51
#define	MP_BAUDRATE_SET				0xA1


void MP_Data_Reply(void);	


#endif

