#include "sd2068.h"

static int SD2068A_WriteTimeOn(void);
static int  SD2068A_WriteTimeOff(void);

static void  SD2068A_PortInitial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(GPIO_SD2068_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIO_SD2068_SDA_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_OD;//��©���
	GPIO_Init(GPIO_SD2068_SDA_PORT, &GPIO_InitStructure);	
}

/* ��SDA��Ϊ����					*/
static void  SD2068A_SDA_IN(void)
{
	GPIO_InitTypeDef					GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/* ��SDA��Ϊ���					*/
static void  SD2068A_SDA_OUT(void)
{		
	GPIO_InitTypeDef					GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin			= GPIO_SD2068_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_Out_OD;//��©���
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}



void  SD2068A_Initial(void)
{
	SD2068A_PortInitial();		
}


static void  SD2068A_I2CWait(void)
{
	volatile u8 i;

	for(i = 0;i<8;i++);
}


/***SCL���ڸߵ�ƽ��SDA�ɸ߱�͹���һ����ʼ����****/

static int  SD2068A_I2CStart(void)
{
	SD2068A_SDA_OUT();
	SD2068A_SDA_HIGH();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	if(!SD2068A_SDA_GET())  //SDA��Ϊ�͵�ƽ������æ���˳�
	{
		return -1;
	}
	SD2068A_SDA_LOW();
	SD2068A_I2CWait();
	while(SD2068A_SDA_GET())//SDA��Ϊ�ߵ�ƽ������æ���˳�
	{
		return -1;
	}
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	return 0;
}

/******�ر�I2C����******************/
static void  SD2068A_I2CStop(void)
{
	SD2068A_SDA_OUT();

	SD2068A_SDA_LOW();
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	SD2068A_SDA_HIGH();
}

/********����ACK*****/
static void  SD2068A_I2CAck(void)
{
	SD2068A_SDA_OUT();
	SD2068A_SDA_LOW();
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	SD2068A_SCL_LOW();

}

/******û��ACK*******/
static void  SD2068A_I2CNoAck(void)
{
	SD2068A_SDA_OUT();
	SD2068A_SDA_HIGH();
	SD2068A_SCL_LOW();
	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	SD2068A_SCL_LOW();

}

/*************��ȡACK�ź�************����1 �� ACK��0 = ��ACK***/
static int SD2068A_I2CWaitAck(void)
{
	u8 ErrTime = 255;

	SD2068A_SCL_LOW();
	SD2068A_SDA_HIGH();
	SD2068A_SDA_IN();

	SD2068A_I2CWait();
	SD2068A_SCL_HIGH();
	SD2068A_I2CWait();
	while(SD2068A_SDA_GET())
	{
		ErrTime--;
		if(!ErrTime)
		{
			SD2068A_SCL_LOW();
			return -1;
		}
	}
	SD2068A_SCL_LOW();
	return 0;
}

/************MCU��SD2068����һ���ֽ�*************/
static void  SD2068A_I2CSendByte(u8 WriteData)  //���ݴӸ�λ����λ//
{
	volatile int x;
	u8 i = 8;
	while(i--)
	{
		SD2068A_SCL_LOW();

		for(x=5;x>0;x--);

		if(WriteData & 0x80)
		{
			SD2068A_SDA_HIGH();
		}
		else
		{
			SD2068A_SDA_LOW();
		}
		SD2068A_I2CWait();
		SD2068A_SCL_HIGH();
		SD2068A_I2CWait();
		WriteData<<=1;
	}
	SD2068A_SCL_LOW();
}

/*********MCU��SD2068����һ�ֽ�*********/
static u8  SD2068A_I2CReceiveByte(void) //���ݴӸ�λ����λ//
{
	u8  ReadData = 0;
	u8	i = 8;

	SD2068A_SDA_HIGH();			//����SDA Ϊ����
	while(i--)
	{
		ReadData<<=1;
		SD2068A_SCL_LOW();
		SD2068A_I2CWait();
		SD2068A_I2CWait();
		SD2068A_SCL_HIGH();
		SD2068A_I2CWait();
		SD2068A_I2CWait();
		if(SD2068A_SDA_GET())
		{
			ReadData |= 0x01;
		}
	}
	SD2068A_SCL_LOW();
	return ReadData;
}

/******��SD2068ʵʱ���ݼĴ���******/
/**
 * @brief ��2068�ж�ȡʱ��
 * @param[in] 
 * @return 
 * @note 
 */
static int SD2068A_I2CReadDate(u8 *TimeBuff)
{
	u8 n;
	if(SD2068A_I2CStart() != 0)
	{
		return -1;
	}
	SD2068A_I2CSendByte(READ_COMMAND);

	if(SD2068A_I2CWaitAck() != 0)
	{
		SD2068A_I2CStop();
		return -1;
	}

	for(n = 0;n<7;n++)
	{
		TimeBuff[n] = SD2068A_I2CReceiveByte();

		if( n != 6)
		{
			SD2068A_I2CAck();
		}
	}
	SD2068A_I2CNoAck();
	SD2068A_I2CStop();
	return 0;
}

/******дSD2068ʵʱ���ݼĴ���******/
static int SD2068A_I2CWriteDate(u8* WriteData)
{
	SD2068A_WriteTimeOn();

	if(SD2068A_I2CStart() != 0)
	{
		return -1;
	}

	SD2068A_I2CSendByte(WRITE_COMMAND);

	if( 0 != SD2068A_I2CWaitAck())
	{
		SD2068A_I2CStop();
		return -1;
	}

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(Second_Address);  //����д��ʼ��ַ
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[0]));  //Second
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[1]));  //minute
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[2]) | 0x80);  //hour,24Сʱ��
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[3]));  //week
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[4]));  //DAY
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[5]));  //MONTH
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(DEC2BCD(WriteData[6]));  //year
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();

	SD2068A_I2CStart();
	SD2068A_I2CSendByte(WRITE_COMMAND);
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(ReviseTime);//���õ���ʱ���ַ
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x00);//�����ֵ����Ĵ��� 
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();
	SD2068A_WriteTimeOff();
	return 0;
}

/******дSD2068�������******/
static int SD2068A_WriteTimeOn(void)
{
	if(0 != SD2068A_I2CStart())
	{ 
		return -1;
	}

	SD2068A_I2CSendByte(WRITE_COMMAND);

	if( 0 != SD2068A_I2CWaitAck())
	{
		SD2068A_I2CStop();
		return -1;
	}

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x10);//����д��ַ10H,CTR2
	SD2068A_I2CWaitAck();

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x80); //��WRTC1 = 1
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();

	SD2068A_I2CStart();
	SD2068A_I2CSendByte(WRITE_COMMAND);
	SD2068A_I2CWaitAck();

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x0f);//����д��ַ0FH ,CTR1
	SD2068A_I2CWaitAck();

	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x84);//��WRTC2��WRTC3 = 1
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();
	return 0;
}

/******дSD2068��ֹ����******/
static int  SD2068A_WriteTimeOff(void)
{
	if(0 != SD2068A_I2CStart())
	{
		return -1;
	}
	SD2068A_I2CSendByte(WRITE_COMMAND);
	if( 0 != SD2068A_I2CWaitAck())
	{
		SD2068A_I2CStop();
		return -1;
	}
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x0f);//����д��ַ0FH ,CTR1
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0X00);//��WRTC2��WRTC3 = 0
	SD2068A_I2CWaitAck();
	SD2068A_SDA_OUT();
	SD2068A_I2CSendByte(0x00);//��WRTC1=0(10H��ַ)
	SD2068A_I2CWaitAck();
	SD2068A_I2CStop();
	return 0;
}

/**
 * @brief ��ȡRTC�е�ʱ��,��:��:ʱ:��:��:��:��
 * @param[in] 
 * @return 0 ��ȡʱ��ɹ�
 *        -1 ��ȡʱ��ʧ��
 * @note ���:
 */
int  SD2068A_GetTime(u8 *TimeBuff)
{
	if(SD2068A_I2CReadDate(TimeBuff) != 0)
	{
		return -1;
	}

	// BCDת10����
	TimeBuff[0]				= BCD2DEC(TimeBuff[0]);	// ��
	TimeBuff[1]				= BCD2DEC(TimeBuff[1]);	// ��
	TimeBuff[2]				= BCD2DEC(TimeBuff[2]);	// ʱ
	TimeBuff[3]				= BCD2DEC(TimeBuff[3]);	// ��
	TimeBuff[4]				= BCD2DEC(TimeBuff[4]);	// ��
	TimeBuff[5]				= BCD2DEC(TimeBuff[5]); 	// ��
	TimeBuff[6]				= BCD2DEC(TimeBuff[6]); 	// ��

	return 0;

}

/**
 * @brief ����RTC�е�ʱ��
 * @param[in] u8 *TimeBuff ��ʽ ��:��:ʱ:��:��:��:��
 * @return 0 ����ʱ��ɹ�
 *        -1 ����ʱ��ʧ��
 * @note ���룺���ʱ���ܣ������꣬ʮ����
 */
int  SD2068A_SetTime(u8 *TimeBuff)
{
//	TimeBuff[0] = SD2068_Time.Second ;
//	TimeBuff[1] = SD2068_Time.Minute ;
//	TimeBuff[2] = SD2068_Time.Hour ;
//	TimeBuff[3] = SD2068_Time.Week ;
//	TimeBuff[4] = SD2068_Time.Day ;
//	TimeBuff[5] = SD2068_Time.Month ;
//	TimeBuff[6] = SD2068_Time.Year ;

	return SD2068A_I2CWriteDate(TimeBuff);

}


