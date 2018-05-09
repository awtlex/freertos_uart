/**
* @file		calendar.c
* @brief	�ն�ʵʱʱ��ά����ģ��
* @author   joe
* @version  v1.00
* @date     2009.10.15
* @note    
*/

#include "stm32f10x_lib.h"
#include "sd2068.h"
#include "calendar.h"


/**
* @brief  ϵͳ��RTC�����Ƿ�OK�ı��
*/
unsigned int	RTC_OK_flag;

/**
* @brief      Initializes calendar application.
* @param[in]  none
* @param[out] none
* @return     none
* @note                    
*/
void Calendar_Init(void)
{
	TypedefDateTime curDateTime;
		
	RTC_OK_flag = 1;		
	
	SD2068A_Initial();

	SD2068A_GetTime((u8*)&curDateTime);
	
	if (curDateTime.year + 2000 > 2200)
	{	
		curDateTime.sec		= 0x00;	
		curDateTime.min		= 0x00;
		curDateTime.hour	= 0x00;
		curDateTime.week	= 0x00;	
		curDateTime.day		= 0x01;
		curDateTime.month	= 0x01;	
		curDateTime.year	= 2010 - 2000;
		
		// ����ʱ��
		SD2068A_SetTime((u8*)&curDateTime);		//���ó�ʼʱ��Ϊ2010.01.01 00 00 00

		// ������ȡʱ��
		SD2068A_GetTime((u8*)&curDateTime);

		// ʱ�䲻�ԣ���ΪRTC����
		if(curDateTime.year + 2000 != 2010 )
		{	
			RTC_OK_flag = 0;		//��λRTC���ϵı��
		}	
	}
}


/**
* @brief      �򵥵ļ��ϵͳ����ʱ��ĸ�ʽ
* @param[in]  none
* @param[out] none
* @return     none
* @note    û�м������ �´���С������     
*/
static int check_time_format(TypedefDateTime *DateTime)
{	
	if (DateTime->sec >= 60)
	{
		return -1;
	}

	if (DateTime->min >= 60)
	{
		return -1;
	}

	if (DateTime->hour >= 24)
	{
		return -1;
	}

	if (DateTime->day > 31)
	{
		return -1;
	}	

	if (DateTime->month > 12)
	{
		return -1;
	}

	return 0;

}

/**
* @brief      ���RTC��ȡ��ʱ���ʽ�����⣬����Dummyʱ��
* @param[in]  none
* @param[out] none
* @return     none
* @note    û�м������ �´���С������     
*/
static int dummy_time_format(TypedefDateTime *DateTime)	
{	
	while(DateTime->sec >= 60)
	{
		DateTime->sec -= 60;
		DateTime->min++;
	}

	while(DateTime->min >= 60)
	{
		DateTime->min -= 60;
		DateTime->hour++;
	}

	while(DateTime->hour >= 24)
	{
		DateTime->hour -= 24;
		DateTime->day++;
	}

	while(DateTime->day > 31)
	{
		DateTime->day -= 31;
		DateTime->month++;
	}	

	while(DateTime->month > 12)
	{
		DateTime->month -= 12;
		DateTime->year++;
	}

	return 0;

}


/**
* @brief      ����ϵͳ����ʱ��
* @param[in]  u8 * timebuf  ������µ�ʱ�仺����
* @param[out] none
* @return     none
* @note       
*       ����������Ҫ��ʵ����ݵĻ����ϼ�ȥ2000
*/
int SetDateTime(TypedefDateTime *DateTime)
{
	if(SD2068A_SetTime((u8*)DateTime))	
	{	
		if (SD2068A_SetTime((u8*)DateTime))	
		{
			
			RTC_OK_flag = 0;		//��λRTC�����쳣���
				
			return FALSE
		}
	}	
		
	return TRUE
}

/**
* @brief     ��ȡϵͳ����ʱ��
* @param[in]  none
* @param[out] none
* @return     none
* @note                    
*/
int DataTime(TypedefDateTime *DateTime)
{	
	u8 cnt = 0;
	
	do 
	{	
		if (SD2068A_GetTime(DateTime))
		{
			if (SD2068A_GetTime(DateTime))
			{	
				RTC_OK_flag = 0;
			}
		}

		cnt++;	
	} while (check_time_format(DateTime) && (cnt < 4));


	if (cnt == 4)
	{
		dummy_time_format(DateTime);
		return FALSE;	
	}
	
	return TRUE;	
}
/**
* @brief     ϵͳʱ��ת��ΪBCD��
* @param[out] BYTE * time_bcd
* @return    none
* @note                    
*/
void Systime2BCD(u8 *time_bcd)
{
	//time_bcd[0] = (((currentDateTime.year + 2000)/1000) << 4) + ((currentDateTime.year + 2000)%1000)/100;	 //0x20
	//time_bcd[1] = ((((currentDateTime.year + 2000)%100)/10) << 4) + (currentDateTime.year + 2000)%10;	 //0x10

	//Ϊ������ٶȣ���������ݵĸ���λ�̶�Ϊ0x20�������ᵼ������ϵͳʱ�䳬��2100��ʱ��ϵͳʱ����ʾ����
	time_bcd[0] = 0x20;
	time_bcd[1] = ((currentDateTime.year/10) << 4) + currentDateTime.year%10;	 //0x10
	time_bcd[2] = ((currentDateTime.month/10) << 4) + currentDateTime.month%10;	 //0x05
	time_bcd[3] = ((currentDateTime.day/10) << 4) + currentDateTime.day%10;	 //0x11

	time_bcd[4] = ((currentDateTime.hour/10) << 4) + currentDateTime.hour%10;
	time_bcd[5] = ((currentDateTime.min/10) << 4) + currentDateTime.min%10;
	time_bcd[6] = ((currentDateTime.sec/10) << 4) + currentDateTime.sec%10;
}	


