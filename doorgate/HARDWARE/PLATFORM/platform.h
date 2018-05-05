#ifndef __PLATFORM_H
#define __PLATFORM_H 
#include "sys.h"
#include "param.h"

#define LED_SystemRun_OPEN  	3 		//1��ֻ��Ϊ�������еƣ�2���������е�+����ָʾ�� 3:�������е�+������ͬ��ָʾ��
#define	BEEP_OPEN 	        	CFG_ON
#define EEPROM_OPEN         	CFG_ON
#define SST_OPEN 		        CFG_ON
#define SD2068_OPEN         	CFG_ON
#define VOICE_OPEN				CFG_ON

#define OUTSWITCH_OPEN      	CFG_ON
#define DateManager_OPEN		CFG_ON

#define Memory_OPEN         	CFG_ON


int platformInit(void);  


#endif	
