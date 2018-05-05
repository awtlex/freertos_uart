//������������������(C) COPYRIGHT 2011 STMicroelectronics����������������������
//[�ļ�]: makepower.c
//[����]: ����Ծ
//[�汾]: V2.0.1
//[����]: 2012/12/2
//[˵��]: ��˾�Ž�Э�����ݴ�����򲿷�.
//������������������������������������������������������������������������������
#include "MakepowerProtocol.h"
#include "basicfunc.h"	


// ��������  : ��˾Э��������ݵ�У����㷨
// �������  : p1 ҪУ������� ReDaLen ���ݳ���.
u8 chk_makepower_chksum(u8 *p1,u16 ReDaLen)		
{	
  	u16 i;
	u8 cs;
	u8 sum = 0;
	u8 Ta;
	u8 Ta1;
	u8 TeC;
	
	for(i=0;i<ReDaLen;i++)
	{		
    	sum += *p1++;
	}
	Ta = *p1++;
	Ta1 = *p1++;
	TeC = TwoAscTOHex(Ta,Ta1);
	cs = (u8)TeC;
	if((u8)(sum+cs)==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// ��������  : ��˾�Ž�Э�鷢������֡ͷ����
void MPDoorSendHead(void)
{	
  	ResetBuffer();	
	SendByte('#');
	SendByte(RTN);	  //������Ϣ
	SendByte(SysConfig.ModAddress>>8);  //�豸��ַ
	SendByte(SysConfig.ModAddress);     //�豸��ַ
	SendByte(0x02);	  //�豸���ͺ�
	SendByte((*ptr_PortResP).ProBuffer.MPDoorProDa.CMD);  //����
	SendByte((*ptr_PortResP).ProBuffer.MPDoorProDa.CmdID);//�����ʶ��
	SendByte(0);	   //���ⷢ�������峤��
	SendByte(0);	   //���ⷢ�������峤��
}

// ��������  : ��˾�Ž�Э��ط���У��ͺ���
void MPDoorSendChksum(void)  
{
	unsigned char *p;
	unsigned int i,Len;
	unsigned char ChksumData=0;	

	p=&(*ptr_PortResP).ProBuffer.DaBuf[1];
	Len=(*ptr_PortResP).Pro_Lenth-1;
	for(i=0;i<Len;i++)
	{	
    ChksumData += Hi_HexToAsc(*p);		//У��Ͱ�ASC����ʽ����
		ChksumData += Low_HexToAsc(*p);
		p++;
	}
	SendByte(~ChksumData+1);
}
// ��������  : ��˾�Ž�Э�鷢�����ݳ���
void MPDoorSendDataLen(void)
{	
	(*ptr_PortResP).ProBuffer.DaBuf[7]= ((*ptr_PortResP).Pro_Lenth-9)>>8;
	(*ptr_PortResP).ProBuffer.DaBuf[8]= (*ptr_PortResP).Pro_Lenth-9;
}
// ��������  : ��˾�Ž�Э�鷢��֡β
void MPDoorSendTail(void)
{	
	SendByte(0x0d);
}
// ��������  : ͨ��Ӧ����Ϣ
void MPDoor_To_pc(void)		
{	
	MPDoorSendHead();	
	MPDoorSendDataLen();	
	MPDoorSendChksum(); 
	MPDoorSendTail();		
	StartSend();		
} 
/////////////////////////////////
// ��������  : ��ȡ�豸��Ϣ
void MPDoorCMD11(void)		
{	
	MPDoorSendHead();
	SendHexBytes((unsigned char*)&HardWareVer,6);
	SendHexBytes("   ",1);
	SendHexBytes((uchar *)&DataStr,11);
	SendHexBytes("   ",4);	
	SendByte(LockTtype_StrongSecurity);//��ǿ������
	SendByte(SysConfig.ModAddress>>8);//�豸��ַ
	SendByte(SysConfig.ModAddress);//�豸��ַ
	MPDoorSendDataLen();	
	MPDoorSendChksum(); 
	MPDoorSendTail();		
	StartSend();		
}
// ��������  : ����ͨ�ŵ�ַ
void MPDoorCMDA1(void)		
{	
	u16  tempAddr =  (uint16_t )(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA1.SetAddr;
	ByteEndian(2,(uint8_t*)&tempAddr);
	SysConfig.ModAddress=tempAddr;	   
	SysConfig.ModAddress&=0xFFFF;     
	if(WriteEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(uchar *)&SysConfig)==Right)
	{	 		
    //LogMsg_AddRecord(LOG_DEVTYPE_COMMON,LOG_ChgDevAddr,(uchar *)&tempAddr, 2); //��ַ�޸�    	       
		MPDoor_To_pc();//SendDefault();
	}
	else
	{
	} 	
	//����ϵͳ���ò���
	if(ReadEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(uchar *)&SysConfig.ModAddress)!=Right)
	{	
		SysConfig=SysConfigDefault;
	} 
}
// ��������  : ��ȡʱ�������
void MPDoorCMD13(void)		
{	
	uint YearDatCMD13;
	uchar TempCMD13[7],i;
	//Date_ReadHex(&SysData.Sys_Date);	
	YearDatCMD13 = 2000+(uint)SysData.Sys_Date.StuDate.Year;	
	TempCMD13[0] = (uchar)(YearDatCMD13>>8);
	TempCMD13[1] = (uchar)YearDatCMD13;
	TempCMD13[2] = SysData.Sys_Date.StuDate.Month;
		//	SendByte(SysData.Sys_Date.StuDate.Week);
	TempCMD13[3] = SysData.Sys_Date.StuDate.Day;
	TempCMD13[4] = SysData.Sys_Date.StuDate.Hour;
	TempCMD13[5] = SysData.Sys_Date.StuDate.Minute;
	TempCMD13[6] = SysData.Sys_Date.StuDate.Second;
	MPDoorSendHead();
	for(i=0;i<7;i++)
	SendByte(TempCMD13[i]);
	MPDoorSendDataLen();	
	MPDoorSendChksum(); 
	MPDoorSendTail();		
	StartSend();		
}
// ��������  : ����ʱ�������
void MPDoorCMDA3(void)		
{	
	uint YearDatCMDA3;
	Date_Time DateCMDA3;
//	SysData.Sys_Date.StuDate.Year
	YearDatCMDA3 = (*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.YearH;
	YearDatCMDA3 <<= 8;
	YearDatCMDA3 = (*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.YearL;
	YearDatCMDA3 -= 2000;
	DateCMDA3.StuDate.Year = (uchar)YearDatCMDA3;
	DateCMDA3.StuDate.Month = (*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.Month;
	DateCMDA3.StuDate.Day = (*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.Day;
  DateCMDA3.StuDate.Week = 0;
	DateCMDA3.StuDate.Hour =( *ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.Hour;
	DateCMDA3.StuDate.Minute = (*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.Minute;
	DateCMDA3.StuDate.Second = (*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMDA3.Second;
	Date_WriteHex(&DateCMDA3);
	Date_ReadHex(&SysData.Sys_Date);
}
// ��������  : �洢������
void MPDoorCMD37(void)		
{	
	uint8_t 	Data[48]; 
	uint32_t    ADDR;
	uint8_t		Len;
	
	ADDR=(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Storage_Add[3];
	ADDR+=(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Storage_Add[2]*0x100;
	ADDR+=(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Storage_Add[1]*0x10000;
	ADDR+=(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Storage_Add[0]*0x1000000;
	Len=(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Info_Len;
	if((*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Storage_Type==0)
	{//eep
	   switch((*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Cmd_Type)
	   {
		  case 2:	//�� 			   
			   ReadEepromNoChek(ADDR,Len ,Data);
			break;		  
			case 1:	//д  			  
			   WriteEepromNoChek(ADDR,Len ,(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Data);
			break;
			case 0:	//���� 
				 memset(Data,0xff,sizeof(Data));
			   WriteEepromNoChek(ADDR,Len ,Data);
			break;
			default:
				RTN=0x04;//��������
			break;
	   }
	}
	else if((*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Storage_Type==1)
	{//flash
	   switch((*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Cmd_Type)
	   {
		  case 2:	//�� 			   
			   SST_Read_NoChekCfg(ADDR,Len ,Data);
			break;		  
			case 1:	//д  			  
			   SST_Write_NoChekCfg(ADDR,Len ,(*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Data);
			break;
			case 0:	//���� 			   
			   SST_Earse_Area(ADDR);//����flash����
			break;
			default:
				RTN=0x04;//��������
			break;
	   }
	}
	
	if((*ptr_PortResP).ProBuffer.MPDoorProDa.Para.MPDoorCMD37.Cmd_Type==2)
	{
		MPDoorSendHead();
		SendHexBytes(Data,Len);		
		MPDoorSendDataLen();	
		MPDoorSendChksum(); 
		MPDoorSendTail();		
		StartSend();	
	}
	else
	{
	   MPDoor_To_pc();
	}	
} 

// ��������  : ��˾�Ž�Э�����
void DealCLDoorProtocol(void)
{
	uint16_t CallAddr; 					
		
	CallAddr=TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[3],(*ptr_PortResP).ProBuffer.DaBuf[4]);
	CallAddr*=0x100;
	CallAddr+=TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[5],(*ptr_PortResP).ProBuffer.DaBuf[6]);
	
	if(CallAddr==SysConfig.ModAddress)
	{
		 if(chk_makepower_chksum((uchar *)&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-4)==1)
		 { 
				BufAscToHex(&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-2);
				RTN=0x01;//������Ϣbug3
				DealMPDoorPro();
		 }
	}
	else if(CallAddr==0xFFFF)
	{
		if(chk_makepower_chksum((uchar *)&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-4)==1)
		{ 
			BufAscToHex(&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-2);	
			if((*ptr_PortResP).ProBuffer.MPDoorProDa.CMD==0x11)//�㲥������
			{
				RTN=0x01;//������Ϣbug3
				MPDoorCMD11();
			}
		}
	}
}  
// ��������  : ��˾�Ž�Э�鴦�� 
void DealMPDoorPro(void)
{	
	uchar  Cmd = (*ptr_PortResP).ProBuffer.MPDoorProDa.CMD;
	switch(Cmd)
	{	
		case  0x11://��ȡ�豸��Ϣ
			MPDoorCMD11();
			break;
		case  0xA1://���õ�ַ
			MPDoorCMDA1();
			break;	
		case  0x13://��ȡϵͳʱ�������
			MPDoorCMD13();
			break;
		case  0xA3://����ϵͳʱ�������
			MPDoorCMDA3();
		  	MPDoor_To_pc();
			break;
		
		case  0x37://�洢������
			MPDoorCMD37();
			break;
				
		default:
			RTN=0x03;//��Ч�������- �����ʶ
		  	MPDoor_To_pc();
			break;
	}
}


