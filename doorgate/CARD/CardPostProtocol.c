//zhj
//v1.0
//20160112

#include "CardPostProtocol.h"
#include "globe.h"
#include "config.h"

#if ReadCard_Open
////////////////////////////////////////////////////////////////
//ÿ����Ϣ�� ���ݳ�ʼ��
void Init_CardRestDayMark(u32 EepAddr)
{   
  WeekRestDayStruct WeekRestDay = {0};
  WriteEepromAndChecksum(EepAddr, sizeof(WeekRestDayStruct),(u8 *)&WeekRestDay);
}
//һ���ڽڼ���Ϣ�� ���ݳ�ʼ��
void Init_CardHoliday(u32 EepAddr)	
{	          
  HolidayListStruct tempHoli = {0};                  
  WriteEepromAndChecksum(EepAddr, sizeof(HolidayListStruct), (u8 *)&tempHoli);                      
}
//����ʱ��-׼��ʱ���б�
void Init_SetWeekListSeg(u32 EepAddr)
{		    
  u32 addr = 0;
	u8 i,j;	     
	WorkWeeKDayTimeStruct WorkWeeKDayTime;
	for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//��6��ʱ��BCDת��hex
	{
		WorkWeeKDayTime.TimeRange[i].StartTimeHH = BCDToHex(0x00);
		WorkWeeKDayTime.TimeRange[i].StartTimeMM = BCDToHex(0x00);

		WorkWeeKDayTime.TimeRange[i].EndTimeHH = BCDToHex(0x23);
		WorkWeeKDayTime.TimeRange[i].EndTimeMM = BCDToHex(0x59);
	}
	for(i=0;i<MAX_WorkWeekTimeTable_NUM;i++)     
	{
		for(j=0;j<7;j++)  //ÿ��ģ�6��������*4��ռ���ֽڣ�+1(chk) = 25          
		{  		
			addr = EepAddr+i*175+j*25;  //������7��������*25=175 	
			WriteEepromAndChecksum(addr,sizeof(WorkWeeKDayTimeStruct),(u8 *)&WorkWeeKDayTime); 	
		}
	}       
}
//������ʱ��-׼��ʱ���б�
void Init_SetWorkDaySeg(u32 EepAddr)
{       
  u32 addr = 0;
	u8 i  = 0;
	WorkDayTimeStruct WorkDayTime;   
	
	for(i=0; i<WorkDayTimeSEG_NUM;i++)		//��4��ʱ��BCDת��hex
	{
		WorkDayTime.TimeRange[i].StartTimeHH = BCDToHex(0x00)	;
		WorkDayTime.TimeRange[i].StartTimeMM = BCDToHex(0x00)	;
		WorkDayTime.TimeRange[i].EndTimeHH = BCDToHex(0x23)	;
		WorkDayTime.TimeRange[i].EndTimeMM = BCDToHex(0x59)	;
	}
	for(i=0;i<MAX_WorkDayTimeTable_NUM;i++)
	{
		addr = EepAddr + i*(16+1); 
		WriteEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(u8 *)&WorkDayTime); 
	}
}
// ��������  : ��ʼ����������Ϣ�յ�׼��ʱ��
void Init_SetRestDaySeg(u32 EepAddr)
{	      
	u8 i= 0;
	RestDayTimeStruct RestDayTime;
	
	//��4��ʱ��BCDת��hex			
	for(i=0; i<RestDayTimeSEG_NUM; i++)		//��4��ʱ��BCDת��hex
	{
		RestDayTime.TimeRange[i].StartTimeHH = BCDToHex(0x00);
		RestDayTime.TimeRange[i].StartTimeMM = BCDToHex(0x00);

		RestDayTime.TimeRange[i].EndTimeHH = BCDToHex(0x23);
		RestDayTime.TimeRange[i].EndTimeMM = BCDToHex(0x59);
	} 		
	WriteEepromAndChecksum(EepAddr, sizeof(RestDayTimeStruct),(u8*)&RestDayTime); 
}
//ʱ�α� ���ݳ�ʼ��
void Init_TimeListSeg(u8 DoorNumber)
{
  switch(DoorNumber)
  {
    case FIRST_DOOR:
			Init_CardRestDayMark(WeekRestDay_EepAddr);
			Init_CardHoliday(HolidayList_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase_EepAddr);
    break;
		case SECOND_DOOR:
      Init_CardRestDayMark(WeekRestDay2_EepAddr);
			Init_CardHoliday(HolidayList2_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase2_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase2_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase2_EepAddr);
    break;
    case THIRD_DOOR:
      Init_CardRestDayMark(WeekRestDay3_EepAddr);
			Init_CardHoliday(HolidayList3_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase3_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase3_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase3_EepAddr);
    break;
    case FOURTH_DOOR:
      Init_CardRestDayMark(WeekRestDay4_EepAddr);
			Init_CardHoliday(HolidayList4_EepAddr);
			Init_SetWeekListSeg(WorkWeekDayTimeBase4_EepAddr);
      Init_SetWorkDaySeg(WorkDayTimeBase4_EepAddr);
      Init_SetRestDaySeg(RestDayTimeBase4_EepAddr);
    break;
  }
}
//����ʱ��-׼��ʱ���б�
void SetWeekListSeg(u32 EepAddr,u8 *BCtimeP)
{		    
  u32 addr = 0;
	u8 i,j;	     
	WorkWeeKDayTimeStruct WorkWeeKDayTime;
	for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//��6��ʱ��BCDת��hex
	{
		WorkWeeKDayTime.TimeRange[i].StartTimeHH = BCDToHex(*BCtimeP);
		WorkWeeKDayTime.TimeRange[i].StartTimeMM = BCDToHex(*(BCtimeP+1));

		WorkWeeKDayTime.TimeRange[i].EndTimeHH = BCDToHex(*(BCtimeP+2));
		WorkWeeKDayTime.TimeRange[i].EndTimeMM = BCDToHex(*(BCtimeP+3));
	}
	for(i=0;i<MAX_WorkWeekTimeTable_NUM;i++)     
	{
		for(j=0;j<7;j++)  //ÿ��ģ�6��������*4��ռ���ֽڣ�+1(chk) = 25          
		{  		
			addr = EepAddr+i*175+j*25;  //������7��������*25=175 	
			WriteEepromAndChecksum(addr,sizeof(WorkWeeKDayTimeStruct),(u8 *)&WorkWeeKDayTime); 	
		}
	}       
}
void Init_SetWeekListSeg1(u32 EepAddr)
{
	u8 Data[4] = {0};
	SetWeekListSeg(EepAddr,Data);
}
//////////////////////////////////////////////////////////////////////
//////////////////// ���ܿ� 49 ����/////////////////////////////////////  
// ��������  : ���������ڹ���׼��ʱ���б� E1
void Card_SetWorkDaySeg(u32 EepAddr)
{       
	u32 addr = 0;
	uint8_t groupId = 0;
	uint8_t i  = 0;
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 38)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	groupId =(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.GroupNO; 
	if(( groupId<=MAX_WorkDayTimeTable_NUM )&&(groupId >=1 ))
	{
		addr = EepAddr + (groupId-1)*(16+1); 
		for(i=0; i<WorkDayTimeSEG_NUM; i++)		//��4��ʱ��BCDת��hex
		{
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM)	;

			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM)	;
		}
		if(WriteEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime))== Error) 
		{	
			RTN = RTN_MODPARA_ERR;
			return;
		}	
	}
	else
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	} 
	SendDefault();
}
// ��������  : ������������Ϣ�յ�׼��ʱ�� //E2
void Card_SetRestDaySeg(u32 EepAddr)
{	      
  u32 addr = 0;
	uint8_t groupId = 0;
	uint8_t i= 0;
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 38)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	groupId= (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.GroupNO;
	if( groupId== 1)        //��Ϣ��ʱ�Σ�ֻ֧��һ�ű�
	{
	//��4��ʱ��BCDת��hex			
		for(i=0; i<4; i++)		//��4��ʱ��BCDת��hex
		{
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].StartTimeMM)	;

			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeHH)	;
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime[i].EndTimeMM)	;
		} 		
		addr = EepAddr+ (groupId-1)*(16+1);
		if(WriteEepromAndChecksum(addr, sizeof(RestDayTimeStruct),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetTimeSeg.SegTime))==Error)
		{	
			RTN = RTN_MODPARA_ERR;
			return;
		}
	}
	else
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч���� 
		return;
	}    
	SendDefault();      
}
// ��������  : �����Ž��� E3
void Card_AddCardList(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)//E3
{
	CardList_Record Record = {0};
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 36)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	Record.WhichProtocol = 0;		//Э�龭�ɰ�������������
	//IC���Ÿ�ֵ
	Record.CardNo[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[1] ;
	Record.CardNo[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[2] ;
	Record.CardNo[2] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[3] ;
	Record.CardNo[3] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.CardNO[4] ;
	//�û����
	Record.CardUseID[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[0] ;
	Record.CardUseID[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[1] ;
	Record.CardUseID[2] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[2] ;
	Record.CardUseID[3] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserID[3] ;
	//�û�����
	Record.CardUserKey[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserPassword[0] ;
  Record.CardUserKey[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserPassword[1] ;
	//�û���Ч��
	Record.CardUsefulLife = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Century<<24;
	Record.CardUsefulLife += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Year<<16;
	Record.CardUsefulLife += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Month<<8;
	Record.CardUsefulLife += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.Day;
	//����Ȩ��
  Record.UserVIP = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewAddCardList.UserPermission;
	
	if(CardLibrary_AddCard(CardLibraryParaEepAddr,CardLibraryParaPointer,&Record)== Error)//���ӿ�
	{
			RTN = RTN_MODPARA_ERR;                 
			return;
	}

	SendDefault();  
}
// ��������  : ɾ������Ϣ E4
void Card_DelCardList(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer)//E4
{	    
    CardList_Record tempRecord = {0};
    uint8_t mode = 0;
		uint16_t  recvDataLen ;         
		//Length�и�4λ����У��  
		recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
		ByteEndian(2,(uint8_t*)&recvDataLen);
		recvDataLen&=0x0fff;
		if( recvDataLen != 16)
		{
			RTN = RTN_UNVALID_DATA;                  //������Ч����
			return;
		}

		mode = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.DelMode; 
		if(mode == DELMODE_BYUSERCARDNO)            //�����û����Ž���ɾ��
		{
				//ɾ����Ӧ����
				tempRecord.CardNo[0] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[1];
				tempRecord.CardNo[1] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[2];
				tempRecord.CardNo[2] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[3];
				tempRecord.CardNo[3] = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.NewDelCardList.CardNO[4];
				if(Right != CardLibrary_DelCard(EEPCardRecordStartAddr,CardLibraryParaPointer,tempRecord.CardNo))
				{
						RTN = RTN_NOHAVE_INFO;          //�޸���Ϣ��
						return;
				}	
		}
		else if(mode == DELMODE_BYUSERID)       //�����û�ID�������ɾ��
		{
				RTN = RTN_UNVALID_DATA;                  //������Ч����
				return;
				//�ݲ�֧��
		}
		else if(mode == DELMODE_ALL) 
		{
				if(EEPCardRecordStartAddr == EEP_CardRecord_StartAddr) 
					CardLibrary_Clear(CardLibraryPara_EepAddr,CardLibraryParaPointer);                //ɾ��ȫ������   
				else if(EEPCardRecordStartAddr == EEP_CardRecord2_StartAddr) 
					CardLibrary_Clear(CardLibraryPara2_EepAddr,CardLibraryParaPointer);                //ɾ��ȫ������   
				else if(EEPCardRecordStartAddr == EEP_CardRecord3_StartAddr) 
					CardLibrary_Clear(CardLibraryPara3_EepAddr,CardLibraryParaPointer);                //ɾ��ȫ������   
				else if(EEPCardRecordStartAddr == EEP_CardRecord4_StartAddr) 
					CardLibrary_Clear(CardLibraryPara4_EepAddr,CardLibraryParaPointer);                //ɾ��ȫ������   		
		}
		else            
		{
				RTN = RTN_UNVALID_DATA;                  //������Ч����
				return;
		}

		SendDefault();       
}
// ��������  : ����ÿ����Ϣ�� //EA
void Card_SetRestDayMark(u32 EepAddr)
{   
    u32 addr = 0;
		uint16_t  recvDataLen ;         
		//Length�и�4λ����У��  
		recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
		ByteEndian(2,(uint8_t*)&recvDataLen);
		recvDataLen&=0x0fff;
		if( recvDataLen != 8)
		{
			RTN = RTN_UNVALID_DATA;                  //������Ч����
			return;
		}
    
    if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[0] >7)
        ||((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[0] <1))
    {
			(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[0] = 0xFF;     //Ϊ��ʡEEPROMд����������0xff
    } 
    
    if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[1] >7)
        ||((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[1] <1))
    {
      (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay[1] = 0xFF;
		}        
    addr = EepAddr;
    if(WriteEepromAndChecksum(addr, sizeof(Ariber_SetWeekRestDay),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekRestDay.RestDay))==Error)
		{
			RTN = RTN_MODPARA_ERR;
			return;
		}
    SendDefault();       
}

// ��������  : ���ýڼ���Ϣ�� //EB
void Card_AddHoliday(u32 EepAddr)  
{     
  uint8_t chkFlag = ERROR;
	u32 addr = 0;
	uint8_t i,j;
	HolidayStruct	Holiday;
  HolidayListStruct tempHoli = {0};
	uint16_t  recvDataLen ;         
	//Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 8)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}
 
	addr = EepAddr;

	Holiday.Month = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Month);
	Holiday.Day = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Day);
	if(Holiday.Month>12||Holiday.Month<1||Holiday.Day>31||Holiday.Day<1)
	{
	  RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}
	else
	{  
		ReadEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli);	
    if(tempHoli.Count > MAX_Holiday_NUM)  //20150907   
		{
			tempHoli.Count = 0;            //д��ڼ�����Ŀ
			i=0;
			chkFlag=SUCCESS;        
		}
		if(tempHoli.Count < MAX_Holiday_NUM)    
		{
			for(i=0;i<tempHoli.Count;i++)
			{
				if(tempHoli.Day[i].Month>=Holiday.Month) //�ȱȽ��·�
				{	
					for(;i<tempHoli.Count;i++)//����ͬ�ٱȽ�����
					{
						if(Holiday.Month == tempHoli.Day[i].Month)
						{
							if(tempHoli.Day[i].Day>=Holiday.Day)
							{
								break;
							}
						}
						else	break;
					}
					break;
				}
			}
			if(i==tempHoli.Count)
			{//��������������
				chkFlag=SUCCESS;
			}
			else if((tempHoli.Day[i].Month!=Holiday.Month)||(tempHoli.Day[i].Day!=Holiday.Day))
			{//����µĽڼ���
				for(j=tempHoli.Count;j>i;j--)
				{
				   tempHoli.Day[j].Month=tempHoli.Day[j-1].Month ;
				   tempHoli.Day[j].Day=tempHoli.Day[j-1].Day ;
				}
				chkFlag=SUCCESS;  				
			} 			
		}
		else
		{
			RTN = RTN_MEMORY_FULL;
			return;
		}

		if(chkFlag==SUCCESS)
		{//ִ����ӽ��ն���
			tempHoli.Day[i].Month=Holiday.Month ;
			tempHoli.Day[i].Day=Holiday.Day ;
			tempHoli.Count++;
			if(WriteEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli)==Error)
			{
				RTN = RTN_MODPARA_ERR;
				return;
			}
		}
	}  	    
  SendDefault();         
}
// ��������  : ɾ���ڼ���Ϣ�� //EC
void Card_DelHoliday(u32 EepAddr)	
{	    
  u32 addr = 0;	    
  uint8_t i = 0;  
  uint8_t chkFlag = ERROR;  
  HolidayStruct	Holiday;
  HolidayListStruct tempHoli = {0};
  uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
  recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
  ByteEndian(2,(uint8_t*)&recvDataLen);
  recvDataLen&=0x0fff;
  if( recvDataLen != 8)
  {
    RTN = RTN_UNVALID_DATA;                  //������Ч����
    return;
  }

	addr = EepAddr;
      
  Holiday.Month = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Month);
  Holiday.Day = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.Holiday.Day);	           
  if((Holiday.Month == 0)&&(Holiday.Day ==0))           // �������ն�Ϊ0�����ʾ��Ҫɾ��ȫ��              
  {	             
    tempHoli.Count=0;
    chkFlag=SUCCESS;     
  }  
  else            
  {
    if(( Holiday.Month <= 12)&&( Holiday.Month  >=1))                  
    {        
      if(( Holiday.Day<= 31)&&( Holiday.Day >=1))     //�������´��������31�����                      
      {
        if(ReadEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli)==Error)
        {
          RTN = RTN_MODPARA_ERR;
          return;
        }
        if(tempHoli.Count == 0)
        {
          RTN = RTN_MEMORY_EMPTY;
          return;
        }
        if(tempHoli.Count > MAX_Holiday_NUM)  tempHoli.Count =  MAX_Holiday_NUM;

        for(i=0; i<tempHoli.Count; i++)                     //�Ƚ��Ƿ�֮ǰ�Ѵ洢������Ӧ�ڼ���
        {
          if((tempHoli.Day[i].Month==Holiday.Month)&&(tempHoli.Day[i].Day==Holiday.Day))
          {								
            for(;i<tempHoli.Count;i++)
            {
              tempHoli.Day[i].Month=tempHoli.Day[i+1].Month ;
              tempHoli.Day[i].Day=tempHoli.Day[i+1].Day ;
            }
            tempHoli.Count--;
            chkFlag = SUCCESS;
            break;
          }
        }  
      }
      else      
      {
        RTN = RTN_UNVALID_DATA ;
        return;
      }
    }
    else
    {
      RTN = RTN_UNVALID_DATA;                  //������Ч����
      return;
    }       
  }         
  if(chkFlag == SUCCESS)
  {              
    if(WriteEepromAndChecksum(addr, sizeof(HolidayListStruct), (uint8_t *)&tempHoli)==Error)
    {
      RTN = RTN_MODPARA_ERR;
      return;
    }
  }
  else      
  {
    RTN = RTN_NOHAVE_INFO ;
		return;
  }       
          
  SendDefault();  
}

// ��������  : ��������׼��ʱ���б�//F1
void Card_SetWeekListSeg(u32 EepAddr) 
{		    
  u32 addr = 0;
	uint8_t i = 0;
	uint16_t  recvDataLen ;         
	//Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 56)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

  addr = EepAddr;  
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.TableNO < MAX_WorkWeekTimeTable_NUM)     
	{
		if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.WeekDay <=7)
					&&((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.WeekDay>=1))
		{  			
			for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//��6��ʱ��BCDת��hex
			{
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeHH)	;
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].StartTimeMM)	;
	
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeHH = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeHH)	;
				(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeMM = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime[i].EndTimeMM)	;
			}
			
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.TableNO)*175; //������7��������*(6��ʱ�Σ�*4��ռ���ֽ�)+1(chk))+=175 
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.WeekDay-1)*25;   //ÿ��ģ�6��������*4��ռ���ֽڣ�+1(chk)	
			if(WriteEepromAndChecksum(addr, sizeof(WorkWeeKDayTimeStruct),(uint8_t *)((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetWeekListSeg.SegTime))==Error)
			{
				RTN = RTN_MODPARA_ERR;
				return;
			}
		}
		else
		{
			RTN = RTN_UNVALID_DATA;                  //������Ч���� 
			return;
		}
	}
	else
	{
		RTN = RTN_MEMORY_EMPTY;                  //������Ч����
		return;
	}      
  SendDefault();         
}
// ���ܿ� 49 ����
void Card_SetSysParaAndRecord(void)
{          
  //�������,���հ�����Э�飬���������������Ҫ��ͨ������У��ſ��Թ��������ݵ������Ŀǰ�ݲ�֧�֣�
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Cmd_Group==0xF1)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Cmd_Type)
		{	
      case 0xE1:				//���ù����յ�׼��ʱ�Σ���������/��Ϣ����ʽ����ʱ�������ã�
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetWorkDaySeg(WorkDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetWorkDaySeg(WorkDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetWorkDaySeg(WorkDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetWorkDaySeg(WorkDayTimeBase4_EepAddr);
        break;
      case 0xE2:				//���� �ǹ����գ��ڼ��ջ���������Ϣ�գ���׼��ʱ�� 
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetRestDaySeg(RestDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetRestDaySeg(RestDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetRestDaySeg(RestDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetRestDaySeg(RestDayTimeBase4_EepAddr);
        break;
      //����
      case 0xE3:				//�����û������ſ�������
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_AddCardList(CardLibraryPara_EepAddr,&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_AddCardList(CardLibraryPara2_EepAddr,&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_AddCardList(CardLibraryPara3_EepAddr,&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_AddCardList(CardLibraryPara4_EepAddr,&CardLibraryPara4);
        break;
      case 0xE4:				//ɾ���û������ſ�������
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_DelCardList(EEP_CardRecord_StartAddr,&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_DelCardList(EEP_CardRecord2_StartAddr,&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_DelCardList(EEP_CardRecord3_StartAddr,&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_DelCardList(EEP_CardRecord4_StartAddr,&CardLibraryPara4);
        break;	
      case 0xEA:				//�趨SM����������Ϣ��
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetRestDayMark(WeekRestDay_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetRestDayMark(WeekRestDay2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetRestDayMark(WeekRestDay3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetRestDayMark(WeekRestDay4_EepAddr);
        break;	 
      case 0xEB:				//�趨SM�Ľڼ��գ���������������Ϣ�գ�
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_AddHoliday(HolidayList_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_AddHoliday(HolidayList2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_AddHoliday(HolidayList3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_AddHoliday(HolidayList4_EepAddr);
        break;
      case 0xEC:				//ɾ��SM�ڵ�һ��ڼ���
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_DelHoliday(HolidayList_EepAddr); 
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_DelHoliday(HolidayList2_EepAddr); 
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_DelHoliday(HolidayList3_EepAddr); 
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_DelHoliday(HolidayList4_EepAddr); 
        break;	    
      case 0xF1:				//SU�趨SM�ڰ�����׼����ʱ����б�
        if(CallSystemAddr==SysConfig.ModAddress)
          Card_SetWeekListSeg(WorkWeekDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
          Card_SetWeekListSeg(WorkWeekDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
          Card_SetWeekListSeg(WorkWeekDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
          Card_SetWeekListSeg(WorkWeekDayTimeBase4_EepAddr);
        break;

      default:					//����ʶ��������ʽ
        RTN = RTN_UNKNOW_CMD;			
      break;
		}
	}
	else
	{	
		RTN=RTN_UNKNOW_CMD;	//����ʶ��������ʽ
	}	        
}
///////////////// ���ܿ�  : 4A ����//////////////////////////////////////////  
// ��������  : ��ȡ����ʱ��� E3
void Card_ReadWorDaykSeg(u32 EepAddr)
{      
	WorkDayTimeStruct      SegTime;             //֧��4��ʱ��
	u32    addr = 0;  
	uint8_t 		groupId = 0; 
	uint8_t 		i = 0;
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 6)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}
	
	groupId = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[0];
	if(( groupId >MAX_WorkDayTimeTable_NUM )||(groupId == 0 ))
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	} 
	
	addr = EepAddr;
	addr += (groupId-1)*17;
	ReadEepromAndChecksum(addr, sizeof(WorkDayTimeStruct),(uint8_t *)&SegTime.TimeRange) ; 
	for(i=0; i<WorkDayTimeSEG_NUM; i++)		//��4��ʱ��BCDת��hex
	{
		SegTime.TimeRange[i].StartTimeHH = HexToBCD(SegTime.TimeRange[i].StartTimeHH)	;
		SegTime.TimeRange[i].StartTimeMM = HexToBCD(SegTime.TimeRange[i].StartTimeMM)	;

		SegTime.TimeRange[i].EndTimeHH = HexToBCD(SegTime.TimeRange[i].EndTimeHH)	;
		SegTime.TimeRange[i].EndTimeMM = HexToBCD(SegTime.TimeRange[i].EndTimeMM)	;
	}  
	
	SendHead();  
	SendAsciiBytes((uchar*)&SegTime.TimeRange,sizeof(WorkDayTimeStruct));
	SendTaiDeal();                   
}
// ��������  : ��ȡ��Ϣ��ʱ���//E4
void Card_ReadRestDaySeg(u32 EepAddr)
{     
  RestDayTimeStruct      SegTime;             //֧��4��ʱ��
  u32    addr = 0;  
	uint8_t 		groupId = 0;
	uint8_t 		i = 0;
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 6)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	groupId = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[0];	     
	if( groupId !=1 )
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	} 
	
	addr = EepAddr ;
	addr += (groupId-1)*17;
	ReadEepromAndChecksum(addr, sizeof(RestDayTimeStruct),(uint8_t *)&SegTime.TimeRange) ; 

	for(i=0; i<RestDayTimeSEG_NUM; i++)		//��4��ʱ��BCDת��hex
	{
		SegTime.TimeRange[i].StartTimeHH = HexToBCD(SegTime.TimeRange[i].StartTimeHH)	;
		SegTime.TimeRange[i].StartTimeMM = HexToBCD(SegTime.TimeRange[i].StartTimeMM)	;

		SegTime.TimeRange[i].EndTimeHH = HexToBCD(SegTime.TimeRange[i].EndTimeHH)	;
		SegTime.TimeRange[i].EndTimeMM = HexToBCD(SegTime.TimeRange[i].EndTimeMM)	;
	}  

	SendHead();  
	SendAsciiBytes((uchar*)&SegTime.TimeRange,sizeof(RestDayTimeStruct));
	SendTaiDeal();                   
}
// ��������  : ��ȡ���û���//E5
void Card_ReadCardListSum(CardLibraryParaStruct *CardLibraryParaPointer)
{       
  uint16_t listSum = 0;
  
  listSum = CardLibraryParaPointer->CardCount; 
  SendHead(); 
  SendOneHexToTwoAsciiByte((uint8_t)listSum);        //С��ģʽ  //�û�������edit by zl
  SendOneHexToTwoAsciiByte((uint8_t)(listSum>>8));     
  SendTaiDeal();    
}
///////////////////////////�����⿪ʼ/////////////////////////////////////////
// ��������  : ���ؿ���¼ 
void Card_MakeUserStruct(CardList_Record *pRecord)
{       
  SendHead();	      
  //IC����
  SendOneHexToTwoAsciiByte(0);
  SendOneHexToTwoAsciiByte(pRecord->CardNo[0]);         //��������
  SendOneHexToTwoAsciiByte(pRecord->CardNo[1]); 
  SendOneHexToTwoAsciiByte(pRecord->CardNo[2]); 
  SendOneHexToTwoAsciiByte(pRecord->CardNo[3]);    
  //�û����
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[0]);         //��������
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[1]); 
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[2]); 
  SendOneHexToTwoAsciiByte(pRecord->CardUseID[3]);
  //�û����� 
  SendOneHexToTwoAsciiByte(pRecord->CardUserKey[0]); 
  SendOneHexToTwoAsciiByte(pRecord->CardUserKey[1]); 
  //��Ч���� @edit by zl �������޸�
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife>>24); 
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife>>16);
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife>>8); 
  SendOneHexToTwoAsciiByte(pRecord->CardUsefulLife); 
  //Ȩ��
  SendOneHexToTwoAsciiByte(pRecord->UserVIP); 

  SendTaiDeal();      
}
// ��������  : ��ȡSM��ָ���û������� E6
void Card_ReadUserInfo(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{
  uint16_t  recvDataLen ;      
  CardList_Record tempRecord = {0};    
  
  recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			//Length�и�4λ����У��
  ByteEndian(2,(uint8_t*)&recvDataLen);
  recvDataLen&=0x0fff;
  if( recvDataLen == 8)		//4���ֽ� ��ͳ�ư���Group��Type����ָ��λ�ü�¼2���ֽ�
  { //���Ϊ����һ������ʱ���Ƚ��лָ�
    if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[0] == 0x00)
        &&((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.Data[1] == 0x00))
    {  //�����ȡ���м����,
      CardLibrary_ResetReadCardConfig(EEPCardRecordStartAddr);
    }
    if(Right == CardLibrary_curReadCard(EEPCardRecordStartAddr,CardLibraryParaPointer,&tempRecord) )
    {
      Card_MakeUserStruct(&tempRecord);
        //��Ҫ��һ���������ƣ���֤ÿ�ζ����ܶ�ȫ
    }
    else
    {
      RTN = RTN_NOHAVE_INFO;
      return;
    }              
  } 
  else if ( recvDataLen == 12)//6���ֽڣ���ͳ�ư���Group��Type�����û����4���ֽ�
  { //Ŀǰ�������δ�����Ȳ�֧�֣�Ϊ��ʡcode �ռ�
    RTN = RTN_UNKNOW_CMD;
    return;
  }
  else if ( recvDataLen == 14)//7���ֽڣ���ͳ�ư���Group��Type�����û�����5���ֽ�   
  { //Ŀǰ�������δ�����Ȳ�֧�֣�Ϊ��ʡcode �ռ� 
    RTN = RTN_UNKNOW_CMD;
    return;
  }  
  else
  {
    RTN = RTN_UNVALID_DATA;
    return;
  }        
}
///////////////////////////////���������////////////////////////////////////////
// ��������  : ��ȡÿ����Ϣ�� //E9
void Card_ReadRestDay(u32 EepAddr) 
{  
  u32 addr = 0;
  WeekRestDayStruct  WeekrestDay = {0};
    
  addr = EepAddr;	     
  ReadEepromAndChecksum(addr, 2,(uchar*)&WeekrestDay) ;   
  
  if((WeekrestDay.Day[0]<1)||(WeekrestDay.Day[0]>7))
  {
    WeekrestDay.Day[0] = 0x00;              //��λ������0xFF�ᱨ��  
  }      
  if((WeekrestDay.Day[1]<1)||(WeekrestDay.Day[1]>7))
  {
    WeekrestDay.Day[1] = 0x00;    
  }

  SendHead(); 
  SendAsciiBytes((uchar*)&WeekrestDay,2);
  SendTaiDeal();
}

// ��������  : ��ȡ�ڼ���Ϣ�� //EA
void Card_ReadHoliday(u32 EepAddr)
{	   
	u32 addr = 0;	    
	uint8_t i = 0;     
	HolidayListStruct tempHoli = {0};  

	addr = EepAddr;	   		//���ڴ洢������������
	ReadEepromAndChecksum(addr,sizeof(HolidayListStruct), (uint8_t *)&tempHoli) ; 
  
  if(tempHoli.Count == 0||tempHoli.Count>MAX_Holiday_NUM)//20150906
	{
		RTN = RTN_MEMORY_EMPTY;
		return;
	}

	SendHead();
	SendOneHexToTwoAsciiByte(tempHoli.Count);  //���ͽڼ�������               
	for(i=0; i<tempHoli.Count; i++)
	{            
		SendOneHexToTwoAsciiByte(HexToBCD(tempHoli.Day[i].Month)) ;  
		SendOneHexToTwoAsciiByte(HexToBCD(tempHoli.Day[i].Day)) ; 
	}       
	SendTaiDeal();         
}

// ��������  : ��ȡ��������ʱ��� //EB
void Card_ReadWeekListSeg(u32 EepAddr)
{     
	WorkWeeKDayTimeStruct      WorkDayTime={0};             //֧��6��ʱ��
	u32    addr = 0;  
	uint8_t i = 0;
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 8)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}
    
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.TableNO < MAX_WorkWeekTimeTable_NUM)		//Э��涨��0~15����Ŀǰ��������Ե��ֻ֧��0~9   		
	{//�ܹ���ʱ��׼��ʱ��α��
		if(((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.WeekDay <=7)
				&&((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.WeekDay>=1))
		{//����				
			addr = EepAddr ; //��ʱ�α����ַ
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.TableNO)*175; //������7��������*(6��ʱ�Σ�*4��ռ���ֽ�)+1(chk))=175 
			addr += ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Info.ReadWeekListSeg.WeekDay-1)*25;   //ÿ��ģ�6��������*4��ռ���ֽڣ�+1(chk)
			ReadEepromAndChecksum(addr, sizeof(WorkWeeKDayTimeStruct),(uint8_t *)&WorkDayTime.TimeRange) ;  

			for(i=0; i<WorkWeeKDayTimeSEG_NUM; i++)		//��6��ʱ��BCDת��hex
			{
				WorkDayTime.TimeRange[i].StartTimeHH = HexToBCD(WorkDayTime.TimeRange[i].StartTimeHH)	;
				WorkDayTime.TimeRange[i].StartTimeMM = HexToBCD(WorkDayTime.TimeRange[i].StartTimeMM)	;

				WorkDayTime.TimeRange[i].EndTimeHH = HexToBCD(WorkDayTime.TimeRange[i].EndTimeHH)	;
				WorkDayTime.TimeRange[i].EndTimeMM = HexToBCD(WorkDayTime.TimeRange[i].EndTimeMM)	;
			}
		}
		else
		{
			RTN = RTN_UNVALID_DATA;                  //������Ч���� 
			return;
		}
	}
	else
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч���� 
		return;
	} 
	
	SendHead();  
	SendAsciiBytes((uchar*)&WorkDayTime.TimeRange,sizeof(WorkWeeKDayTimeStruct));
	SendTaiDeal();                    
}

// ���ܿ�  : 4A ����
void Card_GetSysParaAndRecord(void)
{	    	    
  //�������	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Cmd_Group==0xF2)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Cmd_Type)
		{	
			//********ʱ��**********
			case 0xE3:				//��ȡSM��һ�鹤����ǰ׼��ʱ��
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadWorDaykSeg(WorkDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadWorDaykSeg(WorkDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadWorDaykSeg(WorkDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadWorDaykSeg(WorkDayTimeBase4_EepAddr);
				break;
			case 0xE4:				//��ȡSM��һ��ǹ�����׼��ʱ��
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadRestDaySeg(RestDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadRestDaySeg(RestDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadRestDaySeg(RestDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadRestDaySeg(RestDayTimeBase4_EepAddr);
				break;
			//********����*******
			case 0xE5:				//��ȡSM ���ѵ�����û���Ŀ
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadCardListSum(&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadCardListSum(&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadCardListSum(&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadCardListSum(&CardLibraryPara4);
				break;
			case 0xE6:				//��ȡSM��ָ���û�������
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadUserInfo(EEP_CardRecord_StartAddr,&CardLibraryPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadUserInfo(EEP_CardRecord2_StartAddr,&CardLibraryPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadUserInfo(EEP_CardRecord3_StartAddr,&CardLibraryPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadUserInfo(EEP_CardRecord4_StartAddr,&CardLibraryPara4);
				break;
			
			case 0xE9:				//��ȡSM���Ƶ���֮�趨ÿ�����ڹ̶�����Ϣ��
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadRestDay(WeekRestDay_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadRestDay(WeekRestDay2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadRestDay(WeekRestDay3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadRestDay(WeekRestDay4_EepAddr);
				break;
			case 0xEA:				//��ȡSM���Ƶ���֮�趨��Ϣ�գ���������Ϣ�ճ��⣩
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadHoliday(HolidayList_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadHoliday(HolidayList2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadHoliday(HolidayList3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadHoliday(HolidayList4_EepAddr);
				break;
			case 0xEB:				// ��ȡSM���Ƶ���֮������һ�������յ�׼��ʱ��
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase2_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase3_EepAddr);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadWeekListSeg(WorkWeekDayTimeBase4_EepAddr);
				break;	                    						 	
						 
			default:
				RTN = RTN_UNKNOW_CMD;	// ����ʶ��������ʽ
			break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;
	}	  	
}
/////////////////// ���ܿ�  : 4B ����/////////////////////////////////////    
// ��������  : ���ÿ���Чλ 1�ֽڿ���Чλ+1�ֽڸߵͱ�־λ E1  
//1�ֽڿ���Чλ = ��1~8��1�ֽڸߵͱ�־λ   0Ĭ�� 1 ����
void Card_SetCardNumValid(u32 DoorParaEepAddr,DoorParaStruct *DoorParaPointer)
{
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x08)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0]>9||(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0]==0)
	{
		RTN = RTN_UNVALID_DATA;	
		return;
	}
	DoorParaPointer->Wiegand_CardBits=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0];
	DoorParaPointer->Wiegand_Sequence=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[1];
	WriteEepromAndChecksum(DoorParaEepAddr,sizeof(DoorParaStruct),(uchar *)DoorParaPointer);
	SendDefault();
}	
// ��������  : ����ϵͳ״̬ E5
u8 Card_SetGetCardID(SysDataStruct	*SysDataPointer)
{
  uint8_t i = 0;
	uint16_t  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(uint8_t*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x06)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return 0;
	}

	SysDataPointer->SysState[0]=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0];
	for(i=0;i<Port_Buffer_Count;i++)
	{  
		if(PortRes[i].ResState==ConnectState_DATA_DEAL)//��ͨ�����ݴ���
		{	
			SysDataPointer->SysState[1]=i;
		}
	}

	SendDefault();
	return 1;
}
//4B������
void Card_SetArmingPara(void)
{      	    
  //�������	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Cmd_Group==0xF3)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Cmd_Type)
		{	
			case 0xE1://���ÿ���Чλ
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_SetCardNumValid(DoorPara_EepAddr,&DoorPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_SetCardNumValid(DoorPara2_EepAddr,&DoorPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_SetCardNumValid(DoorPara3_EepAddr,&DoorPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_SetCardNumValid(DoorPara4_EepAddr,&DoorPara4);
				break;
			case 0xE5://����ϵͳ״̬  ��ȡ�����ϱ�
				if(CallSystemAddr==SysConfig.ModAddress)
        {
					if(Card_SetGetCardID(&SysData))
						SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime1);//30��û��ˢ�� ���л�������״̬
				}
        else if(CallSystemAddr==SysConfig.ModAddress+1)
        {
					if(Card_SetGetCardID(&SysData2))
						SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime1);//30��û��ˢ�� ���л�������״̬
				}
        else if(CallSystemAddr==SysConfig.ModAddress+2)
				{	
          if(Card_SetGetCardID(&SysData3))
						SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime1);//30��û��ˢ�� ���л�������״̬
				}
        else if(CallSystemAddr==SysConfig.ModAddress+3)
        {
          if(Card_SetGetCardID(&SysData4))
						SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime1);//30��û��ˢ�� ���л�������״̬
				}
        break;	
					 
			default:
				RTN = RTN_UNKNOW_CMD;	// ����ʶ��������ʽ
			break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;
	}		//����ʶ��������ʽ  	
}
//////////////////////////////////////////////////////////////////////////
///////////////// ���ܿ�  : 4C ����/////////////////////////////// 
//��������  : ��ȡ����Чλ	1�ֽڿ���Чλ+1�ֽڸߵͱ�־λ E1
//1�ֽڿ���Чλ = ��1~8�� 1�ֽڸߵͱ�־λ   0Ĭ��  1 ����
void Card_ReadCardNumValid(DoorParaStruct *DoorParaPointer)
{
	SendHead();
	SendOneHexToTwoAsciiByte(DoorParaPointer->Wiegand_CardBits);        //
	SendOneHexToTwoAsciiByte(DoorParaPointer->Wiegand_Sequence);
  SendTaiDeal();  
}
void Card_GetArmingPara(void)
{      	    
    //�������	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4C.Cmd_Group==0xF4)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4C.Cmd_Type)
		{	   
			case 0xE1:				//��ȡ����Чλ
				if(CallSystemAddr==SysConfig.ModAddress)
					Card_ReadCardNumValid(&DoorPara);
        else if(CallSystemAddr==SysConfig.ModAddress+1)
					Card_ReadCardNumValid(&DoorPara2);
        else if(CallSystemAddr==SysConfig.ModAddress+2)
					Card_ReadCardNumValid(&DoorPara3);
        else if(CallSystemAddr==SysConfig.ModAddress+3)
					Card_ReadCardNumValid(&DoorPara4);
				break;

			default:
				RTN = RTN_UNKNOW_CMD;	// ����ʶ��������ʽ
			break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;//����ʶ��������ʽ
	}		  	
}	 
//Э�鴦��
void Card_PostProtocol(void)
{
  u8  Cmd = (*ptr_PortResP).ProBuffer.PostProDa.Cid2;
  RTN = RTN_NORMAL;
	switch(Cmd)
	{	
		case 0x49:	//SU����SM�ڲ�������������					
		  Card_SetSysParaAndRecord();  	 				
			break;
	  case 0x4A:	//SU��ȡSM����Ϣ						
	   	Card_GetSysParaAndRecord(); 
			break;
		///////////////NewAdd���ڹ�˾����/////////////
		case 0x4B:	//SU����SM�ڲ�������������
		  Card_SetArmingPara();	
			break;
		case 0x4C:	//SU��ȡSM����Ϣ
		  Card_GetArmingPara();	
			break;	

		default:
			RTN= RTN_UNKNOW_CMD;     //��Ч����
			break; 
	}
}
#endif


