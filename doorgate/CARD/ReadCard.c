//zhj
//20170712
//F2.1V1.0

#include "readcard.h"	 
#include "globe.h"
#include "config.h"

#if ReadCard_Open

ReadCardStruct  ReadCard[ReadCardNum];//ΰ��������


// ��������  : ��Ч���ж�
// �������  : pCardInfoTemp ����Ϣ	
// ���ز���  : ��Ч�ڽ��
u8 ReadCard_IsDateOfExpired(CardList_Record	*pCardInfoTemp)
{
	uint32_t Date;
	uint16_t temp; 	
	temp = IntHexToIntBCD(SysData.Sys_Date.StuDate.Year+2000);  
	Date = temp<<16; 
	Date+=IntHexToIntBCD(SysData.Sys_Date.StuDate.Month)<<8;
	Date+=IntHexToIntBCD(SysData.Sys_Date.StuDate.Day);
  if(Date < pCardInfoTemp->CardUsefulLife)
	{
		return Right;
	}
	else
	{
	  return Error;
	} 	 		
}
// ��������  : ��ѯ	�����ǲ��� �ڼ���
// ���ز���  : None
u8 ReadCard_IsHoliday(u8 DoorNumber)
{
	u32 addr = 0;   
	u8 i = 0;
	HolidayListStruct tempHoli = {0};//һ���ڵĽڼ���
	
	if(DoorNumber == FIRST_DOOR) addr  = HolidayList_EepAddr;
	if(DoorNumber == SECOND_DOOR) addr  = HolidayList2_EepAddr;
	if(DoorNumber == THIRD_DOOR) addr  = HolidayList3_EepAddr;
	if(DoorNumber == FOURTH_DOOR) addr  = HolidayList4_EepAddr;

	ReadEepromAndChecksum(addr,sizeof(HolidayListStruct), (u8 *)&tempHoli) ; 
	
	if(tempHoli.Count < MAX_Holiday_NUM)  
	{
		for(i=0; i<tempHoli.Count; i++)
		{
			if((tempHoli.Day[i].Month == SysData.Sys_Date.StuDate.Month)		//��������ͬ
				&&(tempHoli.Day[i].Day == SysData.Sys_Date.StuDate.Day))
			{
				return Right;
			}
		}	
	}  
	return Error;	
}

// ��������  : ʱ��Ƚ�	   
// �������1 : pSegTime ʱ��
// ���ز���  : �ȽϽ��
u8 ReadCard_IsValidSegTime(TimeRangeStruct *pSegTime)
{
	uint16_t StartTime;
	uint16_t EndTime;
	uint16_t Time;

	Time= SysData.Sys_Date.StuDate.Hour*0x100+SysData.Sys_Date.StuDate.Minute;
	StartTime= pSegTime->StartTimeHH*0x100+pSegTime->StartTimeMM;
	EndTime= pSegTime->EndTimeHH*0x100+pSegTime->EndTimeMM;
	if( Time>=StartTime)
	{		
		if( Time<EndTime)
		{
			return Right	;	
		}	
	}  
	return Error;
}	 

// ��������  : ��ѯ	���Ƿ�����Чʱ��
// �������1 : pCardInfoTemp ���ļ�¼
// ���ز���  : None
u8 ReadCard_IsSegTimeValid(u8 DoorNumber,CardList_Record *pCardInfoTemp)
{
	u32 addr = 0;	    
  WorkWeeKDayTimeStruct  WorkWeekDayTime={0};             //֧��6��ʱ��
	WorkDayTimeStruct		   WorkDayTime={0};                 //֧��4��ʱ��
	RestDayTimeStruct		   RestDayTime={0};                 //֧��4��ʱ��
	WeekRestDayStruct		   WeekRestDay={0};                 //2��
	u8 i = 0;

	if((pCardInfoTemp->UserVIP &0xC0) == 0x00)	  	//�����ڹ���ʽ
	{
		if(DoorNumber == FIRST_DOOR) addr = WorkWeekDayTimeBase_EepAddr;
		if(DoorNumber == SECOND_DOOR) addr = WorkWeekDayTimeBase2_EepAddr;
		if(DoorNumber == THIRD_DOOR) addr = WorkWeekDayTimeBase3_EepAddr;
		if(DoorNumber == FOURTH_DOOR) addr = WorkWeekDayTimeBase4_EepAddr;

		addr += (pCardInfoTemp->UserVIP&0x0F)*175; //��������7��������*(6��ʱ�Σ�*4��ռ���ֽ�)+1(chk))+=175 
		if(SysData.Sys_Date.StuDate.Week>7 || SysData.Sys_Date.StuDate.Week == 0)
		{
			SysData.Sys_Date.StuDate.Week=1;
		}
		addr += (SysData.Sys_Date.StuDate.Week-1)*25;   //ÿ��ģ�6��������*4��ռ���ֽڣ�+1(chk)		
    ReadEepromAndChecksum(addr, sizeof(WorkWeeKDayTimeStruct),(u8 *)&WorkWeekDayTime.TimeRange) ;  

		for(i=0;i<WorkWeeKDayTimeSEG_NUM;i++)   	//6��ʱ��
		{
			if(ReadCard_IsValidSegTime(&WorkWeekDayTime.TimeRange[i]) == Right)
			{
				return Right;
			}
			else
			{}	//��ǰʱ�䣬���û���Ȩ����
		}
	}
	else if((pCardInfoTemp->UserVIP &0xC0) == 0x80)	  //�������մ���ʽ
	{	    
	    if(DoorNumber == FIRST_DOOR) addr = WeekRestDay_EepAddr;
			if(DoorNumber == SECOND_DOOR) addr = WeekRestDay2_EepAddr; 
			if(DoorNumber == THIRD_DOOR) addr = WeekRestDay3_EepAddr; 
			if(DoorNumber == FOURTH_DOOR) addr = WeekRestDay4_EepAddr; 

	    ReadEepromAndChecksum(addr, sizeof(WeekRestDayStruct),(u8 *)&WeekRestDay.Day) ;    
	    
	    if((WeekRestDay.Day[0]<1)||(WeekRestDay.Day[0]>7))
	    {
	        WeekRestDay.Day[0] = 0x00;              //��λ������0xFF�ᱨ��  
	    }
	    if((WeekRestDay.Day[1]<1)||(WeekRestDay.Day[1]>7))
	    {
	        WeekRestDay.Day[1] = 0x00;    
	    }

		if((SysData.Sys_Date.StuDate.Week == WeekRestDay.Day[0] ) 		// Ϊ��Ϣ�գ�������Ϣ�յ�ʱ�ν����ж�
		  ||(SysData.Sys_Date.StuDate.Week == WeekRestDay.Day[1] ))
		{  //��Ϣ�գ���ȡ��Ӧ����Ϣʱ��	 			
			if(DoorNumber == FIRST_DOOR) addr = RestDayTimeBase_EepAddr;
			if(DoorNumber == SECOND_DOOR) addr = RestDayTimeBase2_EepAddr;
			if(DoorNumber == THIRD_DOOR) addr = RestDayTimeBase3_EepAddr;
			if(DoorNumber == FOURTH_DOOR) addr = RestDayTimeBase4_EepAddr;
      ReadEepromAndChecksum(addr, sizeof(RestDayTimeStruct),(u8 *)RestDayTime.TimeRange); 
			for(i=0;i<RestDayTimeSEG_NUM;i++)	  	//4��ʱ��
			{
				if(ReadCard_IsValidSegTime(&RestDayTime.TimeRange[i]) == Right)
				{ 					
					return Right;
				}
				else
				{}	//��ǰʱ�䣬���û���Ȩ����
			}										
		}
		else  // Ϊ�����գ����չ����յ�ʱ�ν����ж�
		{
			if(DoorNumber == FIRST_DOOR) addr = WorkDayTimeBase_EepAddr;
			if(DoorNumber == SECOND_DOOR) addr = WorkDayTimeBase2_EepAddr;
			if(DoorNumber == THIRD_DOOR) addr = WorkDayTimeBase3_EepAddr;
			if(DoorNumber == FOURTH_DOOR) addr = WorkDayTimeBase4_EepAddr;
			addr += ((pCardInfoTemp->UserVIP&0x07)-1)*17;	//
      ReadEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(u8 *)WorkDayTime.TimeRange) ; 
			for(i=0;i<WorkDayTimeSEG_NUM;i++)	  	//4��ʱ��
			{
				if(ReadCard_IsValidSegTime(&WorkDayTime.TimeRange[i]) == Right)
				{ 					
					return Right;
				}
				else
				{}	//��ǰʱ�䣬���û���Ȩ����
			}		
		} 
	}
	else
	{
		//��Ч����
	}  
	return Error;
}
// ��������  : ���ݿ�����Ϣ������
// �������  : pCardInfoTemp ����Ϣ
// �������  : pPara  ������Ϣ
void ReadCard_JudgeCardValid(u8 DoorNumber,CardList_Record	*pCardInfoTemp, u8 *pPara)
{
	if((pCardInfoTemp->UserVIP &0xC0) == 0xC0)	  	//��Ȩ��
	{
		//����ˢ������
		*pPara = Ariber_CARD_IN_OPEN_EVENT;
	}
	else 			//�����ڹ���ʽ/�������մ���ʽ
	{
		if(ReadCard_IsDateOfExpired(pCardInfoTemp) == Right)//�ж���Ч���Ƿ��ѵ���,Ŀǰʱ��δ��
		{
			if(ReadCard_IsHoliday(DoorNumber) != Right)		//�����ڼ���ʱ����
			{
				if(ReadCard_IsSegTimeValid(DoorNumber,pCardInfoTemp) == Right)
				{
					*pPara = Ariber_CARD_IN_OPEN_EVENT;	
				}
				else
				{
					//��Чʱ��ˢ��	
					*pPara = Ariber_WEEK_OUT_CARD_OPEN_EVENT;
				}	
			}
			else
			{
				//������ˢ��
				*pPara = Ariber_WEEK_OUT_CARD_OPEN_EVENT;		
			}		
		}
		else
		{
			//���ڿ�
			*pPara 	= Ariber_DATAOUT_CARD_OPEN_EVENT;			
		}
	}
}
// ��������  : ����ȫ00�ж�
// �������1 : pObject ����.
// �������2 : cnt ����
// ���ز���  : 0:ȫ00��1������ȫ00
u8 CompareZero(u8 *pObject,u8 cnt)
{
	u8 i;
	for(i=0;i<cnt;i++)
	{
		if(*pObject++)
		{
			return 1;
		}
	}
	return 0;
}
// ��������  : ����ȫFF�ж�
// �������1 : pObject ����.
// �������2 : cnt ����
// ���ز���  : 0:ȫFF��1������ȫFF
u8 CompareFF(u8 *pObject,u8 cnt)
{
	u8 i;
	for(i=0;i<cnt;i++)
	
	{
		if(*pObject++!=0xFF)
		{
			return 1;
		}
	}
	return 0;
}
// ��������  : ˢ��������
// �������  : IDCard ����.
void ReadCard_OpenDoorDeal(u8 DoorNumber,SysDataStruct *SysDataP,CardLibraryParaStruct *CardLibraryParaP,u32 EEPCardRecordStartAddr)
{
  u8 Pos,Index;
	CardList_Record Record;		   //����Ϣ
	EEP_CardSectorStruct EEP_Sector;//EEP�п����� 
	u8  eventPara	 = 0;	
	
	if(CardLibrary_CardSearch(EEPCardRecordStartAddr,CardLibraryParaP,&SysDataP->IDCard.IDCardNO[0],&EEP_Sector,&Record,&Index,&Pos )==Right)//��ѯ��  
	{
		ReadCard_JudgeCardValid(DoorNumber,&Record, &eventPara);  					
	}
	else
	{	    	
		eventPara = Ariber_UNLOW_CARD_OPEN_EVENT;		//δ��Ȩ������
	}
	//ִ�ж�Ӧ��������Ҫʹ�õı���
	switch(eventPara)
	{
		case  Ariber_CARD_IN_OPEN_EVENT:	 //����ˢ������
      SysDataP->Door_CurAction &= 0xff00;
		  SysDataP->Door_CurAction |= CARD_NORMAL_ACT;	
      break;
		case  Ariber_WEEK_OUT_CARD_OPEN_EVENT:	 //��Чʱ��ˢ��
			BEEP_INVALIDCARD();
      Door_StoreRecord_CommonPara(DoorNumber,Ariber_WEEK_OUT_CARD_OPEN_EVENT);
			SetVoiceAction(Card_WeekOut_Voc,1,1);		//����ʱ�����Ŀ����ܿ���
		break;
		case  Ariber_DATAOUT_CARD_OPEN_EVENT:	 	//���ڿ�
			BEEP_INVALIDCARD();
      Door_StoreRecord_CommonPara(DoorNumber,Ariber_DATAOUT_CARD_OPEN_EVENT);
			SetVoiceAction(Card_TimeOut_Voc,1,1);		//���ѹ���
		break;
		case  Ariber_UNLOW_CARD_OPEN_EVENT:	 	//δ��Ȩ������
		default:						 									
			BEEP_INVALIDCARD();
      Door_StoreRecord_CommonPara(DoorNumber,Ariber_UNLOW_CARD_OPEN_EVENT);
			SetVoiceAction(Card_Unlow_Voc,1,1);			//��δ��Ȩ
		break;
	}
}
// ��������  : ���ڶ���״̬�� ˢ���ϱ�
// �������  : pIDCard ����.
void ReadCard_CardNoReport(IDCardStruct *pIDCard,SysDataStruct *SysDataP)
{  	
	if(PortRes[SysDataP->SysState[1]].ResState==ConnectState_IDLE)//����DEBUGFLAG	jin20140419
	{			
		ptr_PortResP=&PortRes[SysDataP->SysState[1]];
		(*ptr_PortResP).ResState=ConnectState_DATA_DEAL;
		RTN = 0xD0;//һ��Ҫ 0xD0
		SendHead();
		SendOneHexToTwoAsciiByte(0xF4);
		SendOneHexToTwoAsciiByte(0xEC);
		SendOneHexToTwoAsciiByte(0x01);						/*װ���ź�*/
		SendAsciiBytes((uchar*)pIDCard,sizeof(IDCardStruct));	/*װ�뿨��*/
		SendTaiDeal();		
	}
}
//�������ܴ���
//i ����ͷ��
//DoorNumber �ź� �ϴ������жϵ�ַ��ʱ�μ�����Ϣ��ȡ
//DoorParaP	 ���Ŵ�������
//SysDataP	 ����״̬�ж� �����¼�
//CardLibraryParaP �������
//EEPCardRecordStartAddr �����ַ
void ReadCard_Deal(u8 i,u8 DoorNumber,DoorParaStruct *DoorParaP,SysDataStruct *SysDataP,CardLibraryParaStruct *CardLibraryParaP,u32 EEPCardRecordStartAddr)
{
	IDCardStruct 	IDCard;
	//�������û�ȡ����
	Wiegand_DealData(i,&IDCard.IDCardNO[0],DoorParaP);
	//���Ź���
	if((CompareZero(&IDCard.IDCardNO[0],sizeof(IDCardStruct))==0)||(CompareFF(&IDCard.IDCardNO[0],sizeof(IDCardStruct))==0))
	{
		return;
	}
	//ͬ�ſ�ˢ������
	if(memcmp(&ReadCard[i].OldIDCard.IDCardNO[0],&IDCard.IDCardNO[0],sizeof(IDCard.IDCardNO))==0)
	{
		return;
	}
	ReadCard[i].OldIDCard=IDCard;
	ReadCard_TimerOpen(i,DoorParaP->BrushSameCardSecondTime*1000);//ͬ�ſ�����ʱ��
	//SysDataP->ReadCardNumber = i;
  SysData.ReadCardNumber = i;
	//״̬�ж�
	if(SysDataP->SysState[0]==SysState_READCARD)
	{
		switch(DoorNumber)
		{
			case FIRST_DOOR:
				CallSystemAddr = SysConfig.ModAddress;
				SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime1);    //����30�붨ʱ��,ʱ�䵽�Զ����ص�����״̬
				break;
			case SECOND_DOOR:
				CallSystemAddr = SysConfig.ModAddress+1;
				SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime1);    //����30�붨ʱ��,ʱ�䵽�Զ����ص�����״̬
				break;
			case THIRD_DOOR:
				CallSystemAddr = SysConfig.ModAddress+2;
				SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime1);    //����30�붨ʱ��,ʱ�䵽�Զ����ص�����״̬
				break;
			case FOURTH_DOOR:
				CallSystemAddr = SysConfig.ModAddress+3;
				SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime1);    //����30�붨ʱ��,ʱ�䵽�Զ����ص�����״̬
				break;
		}
		ReadCard_CardNoReport(&IDCard,SysDataP);       //�ڶ���״̬�£�����ȡ��ID����Ϣ��������λ��
		BEEP_UploadCard();
	}
	else
	{
		SysDataP->IDCard=IDCard;
		ReadCard_OpenDoorDeal(DoorNumber,SysDataP,CardLibraryParaP,EEPCardRecordStartAddr); 		//������ID
	}
}
//��������  :ϵͳ��������״̬ ˢ������;�ڶ���״̬�� ˢ���ϱ�
void ReadCard_Work(void)
{
	
}

#endif
