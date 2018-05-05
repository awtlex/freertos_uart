//zhj
//v1.0
//20151208

#include "PostProtocolCommon.h"
#include "globe.h"
#include "config.h"

//==========���� ���ñ����==============
uchar *PString;   // ָ���ַ���ָ�룬��ȡ�ַ�����ָ��

////////////////////////
void Delay_SystemReset(void)
{
	for(SysData.time_i=0;SysData.time_i<100000;SysData.time_i++)
	{
		if((*ptr_PortResP).ResState != ConnectState_SEND) 
		{
			break;
		}
	}
	NVIC_SystemReset();
}
////////////////////////
//��ȡʱ��
void Send_MPJ4D(void)
{	
	u16 u16temp = 0; 
	SendHead();    
	//Э�鷢��time is HEX��ʽ
	u16temp=SysData.Sys_Date.StuDate.Year;
	u16temp = (SysData.Sys_Date.StuDate.Year+2000);    
	SendOneHexToTwoAsciiByte(u16temp>>8);
	SendOneHexToTwoAsciiByte(u16temp);
	SendOneHexToTwoAsciiByte((SysData.Sys_Date.StuDate.Month));
	SendOneHexToTwoAsciiByte((SysData.Sys_Date.StuDate.Day));
	SendOneHexToTwoAsciiByte((SysData.Sys_Date.StuDate.Week));

	SendOneHexToTwoAsciiByte((SysData.Sys_Date.StuDate.Hour));
	SendOneHexToTwoAsciiByte((SysData.Sys_Date.StuDate.Minute));
	SendOneHexToTwoAsciiByte((SysData.Sys_Date.StuDate.Second));
	SendTaiDeal(); 	
}
//����ʱ��
void Send_MPJ4E(void)
{	
	Date_Time  tempDate = {0};
  u16 recvDataLen ;   
  u16 year;
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(u8*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x10)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}
  year = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.century<<8)+(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.year;
	if(year/100  == 20 )      //��Ƚ�У��ͨ��
	{
		//ʱ��ΪHEX���ʽ
		tempDate.StuDate.Year   = year - 2000;
		tempDate.StuDate.Month  = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.month); 
		tempDate.StuDate.Day    = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.day); 
		tempDate.StuDate.Week   = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.week); 
		tempDate.StuDate.Hour   = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.hour); 
		tempDate.StuDate.Minute = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.minute); 
		tempDate.StuDate.Second = ((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4E.second); 	
		
		//�������
		if((tempDate.StuDate.Year < 100)&&(tempDate.StuDate.Month < 13 )
			&&(tempDate.StuDate.Day <32)&&(tempDate.StuDate.Hour< 25)        
			&&(tempDate.StuDate.Minute < 61)&&(tempDate.StuDate.Second <61))
		{
			SysData.Sys_Date = tempDate;
			Date_WriteHex(&SysData.Sys_Date);		
			SendDefault(); 
		}
		else
		{
			RTN = RTN_UNVALID_DATA;     
		}   
	}
	else
	{
		RTN = RTN_UNVALID_DATA;  
	}    
}
//��ȡ�豸��ַ
void Send_MPJ50(void)
{		
	SendHead();
	SendOneHexToTwoAsciiByte(SysConfig.ModAddress);     // ��Э���ַ		
	SendTaiDeal();								
}
//�����豸��ַ
void Send_MPJE3(void)
{	  	
  u16  tempAddr;
	u16  recvDataLen ;         
  
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(u8*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen == 2)
	{
		tempAddr = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.OtherDaBuf[0]; 
	}
  else if( recvDataLen == 4)//�ʵ粿�Ž�Э�����õ�ַ
  {
    tempAddr =  (u16 )(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMDE3.SetAddr;
    ByteEndian(2,(u8*)&tempAddr);
    tempAddr&=0x0FFF; 
  }
  else
  {
    RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
  }
  
  if((tempAddr & 0x00ff) == 0x0000 || (tempAddr & 0x00ff) == 0x00ff )
  {
    RTN = RTN_UNVALID_DATA;                  //������Ч����
    return;
  }
  SysConfig.ModAddress=tempAddr;	   
  SysConfig.ModAddress&=0x0FFF;
	if(WriteEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(u8 *)&SysConfig.ModAddress)== Error )
	{	 		   	       
		RTN = RTN_MODPARA_ERR;
		return;
	}
	SendDefault();
}
//�㲥�������ȡ�豸��Ϣ
void Send_MPJ12(void)          
{  
	u8* CpuID;
	
	SendHead();	
	SendOneHexToTwoAsciiByte(SysConfig.ModAddress>>8);
	SendOneHexToTwoAsciiByte(SysConfig.ModAddress);
	SendHexBytes((u8*)SoftWareVer,sizeof(SoftWareVer)-1);
	SendHexBytes((u8*)HardWareVer,sizeof(HardWareVer)-1);
	SendHexBytes((u8*)CompName,sizeof(CompName)-1);
	SendHexBytes((u8*)FactName,sizeof(FactName)-1);

	CpuID = (u8*)U_ID;
	SendAsciiBytes(CpuID,12);

	SendHexBytes((u8 *)&DataStr,11);
	SendByte(' ');
	SendHexBytes((u8 *)&TimeStr,8);
	SendTaiDeal();	  
}
//////////////////////////////////////
/////////////////////////
//���ַ���ת���ɸ���������
float String_To_Float(void)
{	
  uchar  TeCnt=0;
	float  FloatDa;
	char   TeDaBuf[10]={0,0,0,0,0,0,0,0,0,0};
	while( (*PString!=0x2C)&&(TeCnt<10) )             // �ֽ��
	{	
    TeDaBuf[TeCnt] = *PString++;
		TeCnt++;			   // �������ݳ��Ȳ��ɳ���10���ַ�
	}
	PString++;   // �����ֽ��
	FloatDa = atof(&TeDaBuf[0]);      // ���ò���
	return   FloatDa;
}
////////////////////////
//1�ֽ�  ���ַ���(ʮ������)"������ת����HEX��ʽ ��"A8"==>0xA8
uchar String_To_Hex(void)   
{	
  uchar  V;
	V = *PString++;
	V = TwoAscTOHex(V, *PString++);
	return   V;
}
//2�ֽ�  ���ַ���(ʮ������)"������ת����HEX��ʽ ��"A8A1"==>0xA8A1
u16 String_To_u16Hex(void)   
{	
  u8 V;
  u16 W;
	V = *PString++;
	V = TwoAscTOHex(V, *PString++);
  W = V;
  W = W<<8;
  V = *PString++;
	V = TwoAscTOHex(V, *PString++);
  W += V;
	return   W;
}
/////////////////���//////////////////////////
#if OUTSWITCH_OPEN
//��1 ���������������������
u8 SetInOutSwitchConfig(void)
{	
  uchar  i,Count,k;
  for(i=0;i<MAXSwitchCNT_IN;i++)
  {
    InOutSwitchConfig.InConfig[i].InID = String_To_Hex();
    InOutSwitchConfig.InConfig[i].ErrCnt = String_To_Hex();
    InOutSwitchConfig.InConfig[i].InDelayTime = String_To_u16Hex();
    InOutSwitchConfig.InConfig[i].InDefendState = String_To_Hex();
    if(*PString++!=0x2C) return Error;
  }
	for(i=0; i<MAXSwitchCNT_OUT; i++)
	{	
    InOutSwitchConfig.OutConfig[i].OutState = String_To_Hex();
    InOutSwitchConfig.OutConfig[i].OutputDelayTime = String_To_Hex();
    InOutSwitchConfig.OutConfig[i].OutputHoldTime = String_To_Hex();
    InOutSwitchConfig.OutConfig[i].OutExpress.Total = String_To_Hex();    // ��ص���·��
			
    Count=InOutSwitchConfig.OutConfig[i].OutExpress.Total;    // ��ص���·��;
    if(Count>0&&Count<=10)
    {
      Count--;
      for(k=0;k<Count;k++)
      {
        InOutSwitchConfig.OutConfig[i].OutExpress.OutID[k]=String_To_Hex();
        InOutSwitchConfig.OutConfig[i].OutExpress.Math[k]=*PString++;				
      }
      InOutSwitchConfig.OutConfig[i].OutExpress.OutID[Count]=String_To_Hex();
    } 
    if(*PString++!=0x2C) return Error;
	}
  
  return Right;
}
#endif
//��2 ����ϵͳ��������
u8 SetSysConfig(void)
{	
  SysConfig.ModAddress  = String_To_Float();
  SysConfig.USART_BaudRate = String_To_Float();
  SysConfig.DeviceType = String_To_Float();  
	SysConfig.Beep_Mode = String_To_Float(); 
  return Right;
}
//��3 �����Ž���������
u8 SetDoorParaFunction(DoorParaStruct *DoorParaP)
{	
  u8 i;
  DoorParaP->RadiotubeWorkMode  = String_To_Float();
  DoorParaP->RadiotubeTime  = String_To_Float();
  DoorParaP->OverOpenTimeFlag  = String_To_Float();
  DoorParaP->Over_Open_Time  = String_To_Float();
  DoorParaP->BreakDoorDelayTime = String_To_Float();
  DoorParaP->Over_Unlocked_Time = String_To_Float();
  DoorParaP->Wiegand_CardBits = String_To_Float();
  DoorParaP->Wiegand_Sequence = String_To_Float();
  DoorParaP->BrushSameCardSecondTime = String_To_Float();
  DoorParaP->ProtocolPasswordEnableFlag = String_To_Float();
  DoorParaP->JurisdictionPassword[0] = String_To_Hex();
  DoorParaP->JurisdictionPassword[1] = String_To_Hex();
  DoorParaP->JurisdictionPassword[2] = String_To_Hex();
  DoorParaP->JurisdictionPassword[3] = String_To_Hex();
  DoorParaP->JurisdictionPassword[4] = String_To_Hex();
  if(*PString++!=0x2C) return Error;
  DoorParaP->ReadCardHeadType = String_To_Float();
  DoorParaP->DoorUseEnable = String_To_Float();
  for(i=0;i<6;i++)
  {
    DoorParaP->Door1Password[i] = String_To_Hex();
  }
  if(*PString++!=0x2C) return Error;
  for(i=0;i<6;i++)
  {
    DoorParaP->Door2Password[i] = String_To_Hex();
  }
  if(*PString++!=0x2C) return Error;
  for(i=0;i<6;i++)
  {
    DoorParaP->Door2Password[i] = String_To_Hex();
  }
  if(*PString++!=0x2C) return Error;
  for(i=0;i<6;i++)
  {
    DoorParaP->Door2Password[i] = String_To_Hex();
  }
  if(*PString++!=0x2C) return Error;
  return Right;
}
//��1����ģ������
u8 SetBluetoothParaFunction(BluetoothParaStruct *BluetoothParaP)
{	
	u16 i;
  BluetoothParaP->Command  = String_To_Float();
  BluetoothParaP->ProtocolOpen  = String_To_Float();
  BluetoothParaP->Mode  = String_To_Float();
	
  BluetoothParaP->NameFormat  = String_To_Float();
	memset(BluetoothParaP->Name,0,sizeof(BluetoothParaP->Name));
	if(BluetoothParaP->NameFormat == 0)
	{
		for(i=0;i<20 && *PString!=0x2C;i++)
		{
			BluetoothParaP->Name[i] = *PString++;
		}
		if(*PString++!=0x2C) return Error;
	}
	else if(BluetoothParaP->NameFormat == 1)
	{
		for(i=0;i<20 && *PString!=0x2C;i++)
		{
			BluetoothParaP->Name[i] = String_To_Hex();
		}
		if(*PString++!=0x2C) return Error;
	}
	BluetoothParaP->NameLength = i;
	
	for(i=0;i<6;i++)
	{
		BluetoothParaP->MacAddr[i] = String_To_Hex();
	}
	if(*PString++!=0x2C) return Error;
	
	for(i=0;i<sizeof(BluetoothPara.ID);i++)
	{
		BluetoothParaP->ID[i] = String_To_Hex();
	}
	if(*PString++!=0x2C) return Error;
	
	BluetoothParaP->FactoryNumber  = String_To_Float();
	
	for(i=0;i<sizeof(BluetoothPara.UserNumber);i++)
	{
		BluetoothParaP->UserNumber[i] = String_To_Hex();
	}
	if(*PString++!=0x2C) return Error;
	
	BluetoothParaP->LockStatus  = String_To_Float();

	for(i=0;i<sizeof(BluetoothPara.RootKey);i++)
	{
		BluetoothParaP->RootKey[i] = String_To_Hex();
	}
	if(*PString++!=0x2C) return Error;
	
	for(i=0;i<sizeof(BluetoothPara.pIV);i++)
	{
		BluetoothParaP->pIV[i] = String_To_Hex();
	}
	if(*PString++!=0x2C) return Error;
	
  return Right;
}
void SetConfigID_FB(void)
{  	
  uchar Table;
	uint CHAN;
	PString=&(*ptr_PortResP).ProBuffer.PostProDa.ProPara.OtherDaBuf[0];
  CHAN =((u16)(*PString)<<8) + *(PString+1);
  PString = PString + 3;
	Table = String_To_Float();
	switch(Table)
	{
//		case 41:			//����41��Ĳ���(��ȡ�ۺ���������)
//		break;
//		case 40:			//����40��Ĳ���(��ȡ��ʷ��������) 
//		break;
    #if OUTSWITCH_OPEN
		case  1:      	//����������������ñ�---�ַ����Ƚ�����
      if(CHAN != MAXSwitchCNT_IN+MAXSwitchCNT_OUT+1)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
			if(SetInOutSwitchConfig()==Right)
      {
        WriteEepromAndChecksum(InOutSwitchConfig_EepAddr,sizeof(InOutSwitchConfigStruct),(u8 *)&InOutSwitchConfig);	
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(InOutSwitchConfig_EepAddr,sizeof(InOutSwitchConfigStruct),(u8 *)&InOutSwitchConfig)!=Right)
        {
          InOutSwitchConfig=InOutSwitchConfigDefault;
        }
      }
      SendDefault();
      Delay_SystemReset();
      break;
    #endif
    case  2://ϵͳ����
      if(CHAN != 5)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
      if(SetSysConfig()==Right)
      {
        WriteEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(u8 *)&SysConfig);	    
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(u8 *)&SysConfig)!=Right)
        {
          SysConfig = SysConfigDefault;
        } 
        return;
      }
      SendDefault();
      Delay_SystemReset();
      break;
    case  3://��1�Ž�����
      if(CHAN != 18)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
      if(SetDoorParaFunction(&DoorPara)==Right)
      {
        WriteEepromAndChecksum(DoorPara_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara);	    
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(DoorPara_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara)!=Right)
        {
          DoorPara = DoorParaDefault;
        } 
        return;
      }
      SendDefault();
      Delay_SystemReset();
      break;
    case  4://��2�Ž�����
      if(CHAN != 18)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
      if(SetDoorParaFunction(&DoorPara2)==Right)
      {
        WriteEepromAndChecksum(DoorPara2_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara2);	    
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(DoorPara2_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara2)!=Right)
        {
          DoorPara2 = DoorParaDefault;
        } 
        return;
      }
      SendDefault();
      Delay_SystemReset();
      break;
    case  5://��3�Ž�����
      if(CHAN != 18)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
      if(SetDoorParaFunction(&DoorPara3)==Right)
      {
        WriteEepromAndChecksum(DoorPara3_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara3);	    
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(DoorPara3_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara3)!=Right)
        {
          DoorPara3 = DoorParaDefault;
        } 
        return;
      }
      SendDefault();
      Delay_SystemReset();
      break;
    case  6://��4�Ž�����
      if(CHAN != 18)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
      if(SetDoorParaFunction(&DoorPara4)==Right)
      {
        WriteEepromAndChecksum(DoorPara4_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara4);	    
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(DoorPara4_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara4)!=Right)
        {
          DoorPara4 = DoorParaDefault;
        } 
        return;
      }
      SendDefault();
      Delay_SystemReset();
      break;
    case  7://����ģ������
      if(CHAN != 13)
      {
        RTN = RTN_UNVALID_DATA;
        return;
      }
      if(SetBluetoothParaFunction(&BluetoothPara)==Right)
      {
        WriteEepromAndChecksum(BluetoothParaStruct_EepAddr,sizeof(BluetoothParaStruct),(u8 *)&BluetoothPara);	    
      }
      else
      {
        RTN = RTN_UNVALID_DATA;
        if(ReadEepromAndChecksum(BluetoothParaStruct_EepAddr,sizeof(BluetoothParaStruct),(u8 *)&BluetoothPara)!=Right)
        {
          BluetoothPara = BluetoothParaDefault;
        } 
        return;
      }
      SendDefault();
      Delay_SystemReset();
      break;
    
		default:
      RTN = RTN_UNVALID_DATA;                  //������Ч����
      break;
	}
}
///////////////////////////////
//==========��ȡ���ñ����================
/*  �������ܣ���������(char/int/long/float)ת����ʮ�����ַ���         
		��ڲ����� float : ��ת�����ݣ�      
		uchar�� ��ת���������ͣ�	0: char\int\long��   1:Float;
*/
void Send_Str_Float(float HnteB,uchar DataTY)
{	
  char lenth,i;
	char	st[20];
	if((HnteB>4294967295.0f) || (HnteB<-2147483647.0f))   // 9999  9999 
	{	
    HnteB = 0;	
	}
	if(DataTY)    // ��0��������
	{	
    lenth = sprintf(st,"%.3f",HnteB);
	}
	else          // 0�� char\int\long
	{	
    lenth = sprintf(st,"%.0f",HnteB);
	}
	for(i=0;i<lenth;i++)
	{	
    SendOneHexToTwoAsciiByte(st[i]);
	}
}
//------��1 ���������ʽ���ϱ�-------------
// 5--->"0005", "12345"--->"3039"�� ���������Թ̶���ȣ�ʮ�����Ƹ�ʽ�ϱ�
void Send_Str_HexFun(ulong TeA,uchar Width)   //  ���������ñ�1��
{	
  uchar  i;
	char   lenth,sd[10];          // Ulong, ��32λ���8����
	switch(Width)                           //  %04,  �̶�����Ϊ4�����������ǰ�油0;  ��0Ϊ�����Ԫ�� 0 �Ļ���ʾ�ո��� 0���ո����ڶ�ֵ����ʾ�ո�ͷ���
	{	
    case  8:
			lenth = sprintf(sd,"%08X",TeA);        // Ulong 0xFFFFFFFF
		break;
		case  4:
			lenth = sprintf(sd,"%04X",TeA);        // Uint 0xFFFF      �̶�����Ϊ4�����������ǰ�油0;  ��0Ϊ�����Ԫ��
		break;
		case  2:
			lenth = sprintf(sd,"%02X",TeA);        // uchar  0xFF
		break;
		case  1:
			lenth = sprintf(sd,"%lX", TeA);        // ��С����Ϊ1���� ������ͨ��ID����ϱ�  0x10203040---->"10203040";   0xD0AB-->"D0AB" 
		break;
		default:
			lenth = 0;
		break;
	}
	for(i=0; i<lenth; i++)
	{	
    SendOneHexToTwoAsciiByte(sd[i]);
	}
}
/////////////
//���Ž�����
u8 ReadDoorParaFunction(u8 Table,DoorParaStruct *DoorParaP)
{
  u8 i;
  Send_Word(18);      // �ַ�������
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(Table,0);     // ���
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->RadiotubeWorkMode ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->RadiotubeTime ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->OverOpenTimeFlag ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->Over_Open_Time ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->BreakDoorDelayTime ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->Over_Unlocked_Time ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->Wiegand_CardBits ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->Wiegand_Sequence ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->BrushSameCardSecondTime ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->ProtocolPasswordEnableFlag ,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_HexFun(DoorParaP->JurisdictionPassword[0],2);     
  Send_Str_HexFun(DoorParaP->JurisdictionPassword[1],2);    
  Send_Str_HexFun(DoorParaP->JurisdictionPassword[2],2);     
  Send_Str_HexFun(DoorParaP->JurisdictionPassword[3],2);     
  Send_Str_HexFun(DoorParaP->JurisdictionPassword[4],2);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->ReadCardHeadType,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(DoorParaP->DoorUseEnable,0);     
  SendOneHexToTwoAsciiByte(0x2C);
  for(i=0;i<6;i++)
  {
    Send_Str_HexFun(DoorParaP->Door1Password[i],2);
  }
  SendOneHexToTwoAsciiByte(0x2C);
  for(i=0;i<6;i++)
  {
    Send_Str_HexFun(DoorParaP->Door2Password[i],2);
  }
  SendOneHexToTwoAsciiByte(0x2C);
  for(i=0;i<6;i++)
  {
    Send_Str_HexFun(DoorParaP->Door3Password[i],2);
  }
  SendOneHexToTwoAsciiByte(0x2C);
  for(i=0;i<6;i++)
  {
    Send_Str_HexFun(DoorParaP->Door4Password[i],2);
  }
  SendOneHexToTwoAsciiByte(0x2C);

	SendTaiDeal();
  
  return Right;
}
//����������
u8 ReadBluetoothParaFunction(u8 Table)
{
	u16 i;
	Send_Word(13);      						//�ַ�������
  SendOneHexToTwoAsciiByte(0x2C);
  Send_Str_Float(Table,0);     	//���
  SendOneHexToTwoAsciiByte(0x2C);
	Send_Str_Float(BluetoothPara.Command,0);     
  SendOneHexToTwoAsciiByte(0x2C);
	Send_Str_Float(BluetoothPara.ProtocolOpen,0);     
  SendOneHexToTwoAsciiByte(0x2C);
	Send_Str_Float(BluetoothPara.Mode,0);     
  SendOneHexToTwoAsciiByte(0x2C);
	Send_Str_Float(BluetoothPara.NameFormat,0);     
  SendOneHexToTwoAsciiByte(0x2C);
	if(BluetoothPara.NameFormat == 0)
	{
		SendAsciiBytes(BluetoothPara.Name,BluetoothPara.NameLength);
	}
	else if(BluetoothPara.NameFormat == 1)
	{
		for(i=0;i<BluetoothPara.NameLength;i++)
		{
			SendOneHexToTwoAsciiByte(Hi_HexToAsc(BluetoothPara.Name[i]));//16����
			SendOneHexToTwoAsciiByte(Low_HexToAsc(BluetoothPara.Name[i]));
		}
	}
	SendOneHexToTwoAsciiByte(0x2C);
	for(i=0;i<6;i++)
	{
		SendOneHexToTwoAsciiByte(Hi_HexToAsc(BluetoothPara.MacAddr[i]));//16����
		SendOneHexToTwoAsciiByte(Low_HexToAsc(BluetoothPara.MacAddr[i]));
	}
	SendOneHexToTwoAsciiByte(0x2C);
	for(i=0;i<sizeof(BluetoothPara.ID);i++)
	{
		SendOneHexToTwoAsciiByte(Hi_HexToAsc(BluetoothPara.ID[i]));//16����
		SendOneHexToTwoAsciiByte(Low_HexToAsc(BluetoothPara.ID[i]));
	}
	SendOneHexToTwoAsciiByte(0x2C);
	Send_Str_Float(BluetoothPara.FactoryNumber,0);
	SendOneHexToTwoAsciiByte(0x2C);
	for(i=0;i<sizeof(BluetoothPara.UserNumber);i++)
	{
		SendOneHexToTwoAsciiByte(Hi_HexToAsc(BluetoothPara.UserNumber[i]));//16����
		SendOneHexToTwoAsciiByte(Low_HexToAsc(BluetoothPara.UserNumber[i]));
	}
	SendOneHexToTwoAsciiByte(0x2C);
	Send_Str_Float(BluetoothPara.LockStatus,0);     
  SendOneHexToTwoAsciiByte(0x2C);
	for(i=0;i<sizeof(BluetoothPara.RootKey);i++)
	{
		SendOneHexToTwoAsciiByte(Hi_HexToAsc(BluetoothPara.RootKey[i]));//16����
		SendOneHexToTwoAsciiByte(Low_HexToAsc(BluetoothPara.RootKey[i]));
	}
	SendOneHexToTwoAsciiByte(0x2C);
	for(i=0;i<sizeof(BluetoothPara.pIV);i++)
	{
		SendOneHexToTwoAsciiByte(Hi_HexToAsc(BluetoothPara.pIV[i]));//16����
		SendOneHexToTwoAsciiByte(Low_HexToAsc(BluetoothPara.pIV[i]));
	}
	SendOneHexToTwoAsciiByte(0x2C);
	
	SendTaiDeal();
  
  return Right;
}
void ReadConfigID_FC(void)
{	
  u8 Table=0;
  u8 i,k,Count;
	PString=&(*ptr_PortResP).ProBuffer.PostProDa.ProPara.OtherDaBuf[0];
	Table = String_To_Float();
  SendHead();
	switch(Table)
	{
//		case 41:	//��ȡ41�����
//		break;
//		case 40:	//��ȡ40�����
//		break;
    #if OUTSWITCH_OPEN
		case  1://��1 ���������������
			Send_Word(MAXSwitchCNT_IN+MAXSwitchCNT_OUT+1);      // �ַ�������
			SendOneHexToTwoAsciiByte(0x2C);
			Send_Str_Float(Table,0);     // ���"1"
      SendOneHexToTwoAsciiByte(0x2C);
      for(i=0;i<MAXSwitchCNT_IN;i++)
      {
        Send_Str_HexFun(InOutSwitchConfig.InConfig[i].InID,2);
        Send_Str_HexFun(InOutSwitchConfig.InConfig[i].ErrCnt,2);
        Send_Str_HexFun(InOutSwitchConfig.InConfig[i].InDelayTime,4);
        Send_Str_HexFun(InOutSwitchConfig.InConfig[i].InDefendState,2);
        SendOneHexToTwoAsciiByte(0x2C);
      }
      for(i=0; i<MAXSwitchCNT_OUT; i++)
      {	
        Send_Str_HexFun(InOutSwitchConfig.OutConfig[i].OutState,2);
        Send_Str_HexFun(InOutSwitchConfig.OutConfig[i].OutputDelayTime,2);
        Send_Str_HexFun(InOutSwitchConfig.OutConfig[i].OutputHoldTime,2);
        Send_Str_HexFun(InOutSwitchConfig.OutConfig[i].OutExpress.Total,2);    // ��ص���·��
          
        Count=InOutSwitchConfig.OutConfig[i].OutExpress.Total;    // ��ص���·��;
        if(Count>0&&Count<=10)
        {
          Count--;
          for(k=0;k<Count;k++)
          {
            Send_Str_HexFun(InOutSwitchConfig.OutConfig[i].OutExpress.OutID[k],2);
            SendOneHexToTwoAsciiByte(InOutSwitchConfig.OutConfig[i].OutExpress.Math[k]);				
          }
          Send_Str_HexFun(InOutSwitchConfig.OutConfig[i].OutExpress.OutID[Count],2);
        }
        SendOneHexToTwoAsciiByte(0x2C);
      }
      SendTaiDeal();
		break;
    #endif
    case  2://ϵͳ����
			Send_Word(5);      // �ַ�������
			SendOneHexToTwoAsciiByte(0x2C);
			Send_Str_Float(Table,0);     // ���
      SendOneHexToTwoAsciiByte(0x2C);
      Send_Str_Float(SysConfig.ModAddress ,0);     
      SendOneHexToTwoAsciiByte(0x2C);
      Send_Str_Float(SysConfig.USART_BaudRate ,0);     
      SendOneHexToTwoAsciiByte(0x2C);
      Send_Str_Float(SysConfig.DeviceType ,0);     
      SendOneHexToTwoAsciiByte(0x2C);
			Send_Str_Float(SysConfig.Beep_Mode ,0);     
      SendOneHexToTwoAsciiByte(0x2C);
      SendTaiDeal();
		break;
    case  3:
      ReadDoorParaFunction(Table,&DoorPara);
		break;
    case  4:
      ReadDoorParaFunction(Table,&DoorPara2);
		break;
    case  5:
      ReadDoorParaFunction(Table,&DoorPara3);
		break;
    case  6:
      ReadDoorParaFunction(Table,&DoorPara4);
		break;
    case  7:
      ReadBluetoothParaFunction(Table);
		break;
    default:
      RTN = RTN_UNVALID_DATA;                  //������Ч����
      break;
  }
}
////////////////////////////////////////
//=====ң������=============================================
void  CtrlEspecialFun(uchar  Vale,uchar Chann )   // �������
{	
  switch(Chann)    // ��ֵ������
	{
		case  31:    // ��λ������
      SendDefault();
      Delay_SystemReset();
      break;
    case  32:   //�ָ���������
      switch(Vale)
      {
        case 1://����Ĭ������ 
          SysConfig = SysConfigDefault;
          WriteEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(uchar *)&SysConfig);
					#if OUTSWITCH_OPEN
          InOutSwitchConfig=InOutSwitchConfigDefault;
          WriteEepromAndChecksum(InOutSwitchConfig_EepAddr,sizeof(InOutSwitchConfigStruct),(u8 *)&InOutSwitchConfig);
          #endif
					#if DOORACTION_OPEN		
					Init_DoorPara(FIRST_DOOR);
					Init_DoorPara(SECOND_DOOR);
					Init_DoorPara(THIRD_DOOR);
					Init_DoorPara(FOURTH_DOOR);
					#endif
					#if ReadCard_Open
					Init_TimeListSeg(FIRST_DOOR);
          Init_TimeListSeg(SECOND_DOOR);
					Init_TimeListSeg(THIRD_DOOR);
					Init_TimeListSeg(FOURTH_DOOR);
					#endif
      
          SendDefault();
          Delay_SystemReset();
          break;
       
        default:
          RTN = RTN_UNVALID_DATA;                  //������Ч����
          break;
      }
      break;
	case  33:   //��ʷ��¼�ظ���������
      switch(Vale)
      {
				#if DoorRecordQueue_Open
        case 1://������ʷ��¼�ظ���������
					Door_StoreRecord1_Clear(RecordQueue1Para_EepAddr,&Sys_DoorRecord);
					SendDefault();
					break;
				#endif
				default:
					RTN = RTN_UNVALID_DATA;                  //������Ч����
					break;
			}
			break;
		default: 
      RTN = RTN_UNVALID_DATA;                  //������Ч����
      break;
	}
}
void Send_MPJ45(void)
{	
  u8 Chann,Vale;
  
  PString=&(*ptr_PortResP).ProBuffer.PostProDa.ProPara.OtherDaBuf[0];
	Chann = *PString++;
	Vale =  *PString++;
	if(Chann>0 && Chann<=MAXSwitchCNT_OUT)
	{	
		#if OUTSWITCH_OPEN
    OutSwitchEvent[Chann-1].OutSFlag = 1;   // �Ž��¼�������״̬
		OutSwitchEvent[Chann-1].OutSTime = DoorEventOutStateDealyTime;  // ��״̬����ʱ
		#endif
    SendDefault();
	}
	else
	{	
    CtrlEspecialFun(Vale,Chann);
	}
}

//////////////////////////�Ž��ʵ粿����/////////////
//51���� ��ȡ�豸��Ϣ
void Ariber_51(void)           
{  
	u8 Va;	 	
	SendHead();
	Va = sizeof(SoftWareVer)-1;
	SendAsciiBytes((u8*)SoftWareVer,Va);
	for(;Va<12;Va++)
	{
	  SendOneHexToTwoAsciiByte(0x20);
	}  	
	SendAsciiBytes((u8*)CompName,sizeof(CompName)-1);			
	SendTaiDeal(); 	  
}
// ��������  : �����豸������
void Send_MPJA1(void)
{	
	u32  USART_BaudRate;
	u16  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(u8*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 8)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	if( CallSystemAddr != SysConfig.ModAddress)//20150605 ֻ��ʵ�ʵ�ַ�ſ����޸ĵ�ַ
	{
		RTN = RTN_UNKNOW_CMD;
		return;
	}
	USART_BaudRate = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMDA1.USART_BaudRate;
	ByteEndian(4,(u8*)&USART_BaudRate);
	USART_BaudRate = U32BCDToU32Hex(USART_BaudRate);
	if(	USART_BaudRate != 1200 && USART_BaudRate != 2400 &&
			USART_BaudRate != 4800 && USART_BaudRate != 9600 &&
			USART_BaudRate != 14400 && USART_BaudRate != 19200 &&
			USART_BaudRate != 38400 && USART_BaudRate != 56000 &&
			USART_BaudRate != 57600 && USART_BaudRate != 115200)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}
	SysConfig.USART_BaudRate = USART_BaudRate;	        
	if(WriteEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(u8 *)&SysConfig.ModAddress)== Error )
	{	 		   	       
		RTN = RTN_MODPARA_ERR;
		return;
	}

	SendDefault();
	Delay_SystemReset();
}
//////////////////////���ܿ� 48 ���� Ȩ������///////////////////////////////////
//��ȡȨ��
//���أ�1 ������ȷ
u8 Ariber_ConfirmAuthority(DoorParaStruct *DoorParaPointer)
{
	u8 password[5],i;
	u8 *PasswordPointer = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD48.Password;
	u16  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(u8*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x0E)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return 0;
	}
	for(i=0;i<5;i++)
	{
		password[i]= PasswordPointer[i];
	}
	if( memcmp(password,DoorParaPointer->JurisdictionPassword,sizeof(password)) == 0)
	{
		SendDefault();
		return 1;
	}
	else
	{
		RTN = RTN_PASSWORD_WRONG;              
		return 0;
	}
}
//ȡ��Ȩ��
void Ariber_CancleAuthority(SysDataStruct	*SysDataPointer)
{
	SysDataPointer->SysState[0] = SysState_REALTIME;
	SendDefault();
}
//�޸Ļ�ȡȨ������
void Ariber_ModifyPassword(u32 DoorParaEepAddr,DoorParaStruct *DoorParaPointer)
{
	u8 password[6],i,sum;
	u8 *PasswordPointer = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD48.Password;
	u16  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(u8*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 0x10)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	if(SysData.SysState[0]!=SysState_CONFIG)
	{
		RTN = RTN_NOACCESS;
		return;
	}
	for(i=0;i<6;i++)
	{
		password[i]= PasswordPointer[i];
	}
	
	sum = password[0];
	for(i=1;i<5;i++)
	{
		sum = sum ^ password[i];
	}
	if(sum != password[i])
	{
		RTN = RTN_PASSWORD_WRONG;
		return;
	}
	
	memcpy(DoorParaPointer->JurisdictionPassword,password,sizeof(DoorParaPointer->JurisdictionPassword));
	if(WriteEepromAndChecksum(DoorParaEepAddr,sizeof(DoorParaStruct),(u8 *)DoorParaPointer)== Error)
	{
		RTN = RTN_MODPARA_ERR;
		return;
	}
	
	SendDefault();
}
//Ȩ������
void Ariber_AuthenticationFun(void)
{
  //�������
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD48.Cmd_Group == 0xF0) 
	{
		//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD48.Cmd_Type)
		{	    		
			case 0xE0:				//��ȡȨ��
        if(CallSystemAddr==SysConfig.ModAddress)
        {
          if(Ariber_ConfirmAuthority(&DoorPara))
          {
            SysData.SysState[0] = SysState_CONFIG;
            SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime);
          }
        }
        else if(CallSystemAddr==SysConfig.ModAddress+1)
        {
          if(Ariber_ConfirmAuthority(&DoorPara2))
          {
            SysData2.SysState[0] = SysState_CONFIG;
            SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime);
          }
        }
        else if(CallSystemAddr==SysConfig.ModAddress+2)
        {
          if(Ariber_ConfirmAuthority(&DoorPara3))
          {
            SysData3.SysState[0] = SysState_CONFIG;
            SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime);
          }
        }
        else if(CallSystemAddr==SysConfig.ModAddress+3)
        {
          if(Ariber_ConfirmAuthority(&DoorPara4))
          {
            SysData4.SysState[0] = SysState_CONFIG;
            SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime);
          }
        }
				break;
			case 0xE1:				//����ȡ����SM������Ȩ��,��ȡ����¼
        if(CallSystemAddr==SysConfig.ModAddress)
          Ariber_CancleAuthority(&SysData);
        if(CallSystemAddr==SysConfig.ModAddress+1)
          Ariber_CancleAuthority(&SysData2);
        if(CallSystemAddr==SysConfig.ModAddress+2)
          Ariber_CancleAuthority(&SysData3);
        if(CallSystemAddr==SysConfig.ModAddress+3)
          Ariber_CancleAuthority(&SysData4);
				break;
			case 0xE2:				//�޸�SN�ķ�������
        if(CallSystemAddr==SysConfig.ModAddress)
          Ariber_ModifyPassword(DoorPara_EepAddr,&DoorPara);
        if(CallSystemAddr==SysConfig.ModAddress+1)
          Ariber_ModifyPassword(DoorPara2_EepAddr,&DoorPara2);
        if(CallSystemAddr==SysConfig.ModAddress+2)
          Ariber_ModifyPassword(DoorPara3_EepAddr,&DoorPara3);
        if(CallSystemAddr==SysConfig.ModAddress+4)
          Ariber_ModifyPassword(DoorPara4_EepAddr,&DoorPara4);
				break;	
			default:
				RTN = RTN_UNKNOW_CMD;			//����ʶ��������ʽ
				break;
		}
	}
	else
	{
		RTN = RTN_UNKNOW_CMD;			//����ʶ��������ʽ
	}
}

//////////////////// ���ܿ� 49 ����/////////////////////////////////////  
// ��������  : ����ϵͳʱ�� E0
void Ariber_SetSysTime(void)
{	       
	Date_Time  tempDate = {0};
	u16  recvDataLen ;         
  //Length�и�4λ����У��  
	recvDataLen = (*ptr_PortResP).ProBuffer.PostProDa.Length ;			
	ByteEndian(2,(u8*)&recvDataLen);
	recvDataLen&=0x0fff;
	if( recvDataLen != 20)
	{
		RTN = RTN_UNVALID_DATA;                  //������Ч����
		return;
	}

	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.century  == 0x20 )      //��Ƚ�У��ͨ��
	{
		//ʱ��ΪBCD���ʽ
		tempDate.StuDate.Year   = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.year);  
		tempDate.StuDate.Month  = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.month); 
		tempDate.StuDate.Day    = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.day); 
		tempDate.StuDate.Week   = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.week); 
		tempDate.StuDate.Hour   = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.hour); 
		tempDate.StuDate.Minute = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.minute); 
		tempDate.StuDate.Second = BCDToHex((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Info.SetDateAndTime.second); 	
		
		//�������
		if((tempDate.StuDate.Year < 100)&&(tempDate.StuDate.Month < 13 )
			&&(tempDate.StuDate.Day <32)&&(tempDate.StuDate.Hour< 25)        
			&&(tempDate.StuDate.Minute < 61)&&(tempDate.StuDate.Second <61))
		{
			SysData.Sys_Date = tempDate;
			Date_WriteHex(&SysData.Sys_Date);		
			SendDefault(); 
		}
		else
		{
			RTN = RTN_UNVALID_DATA;     
		}   
	}
	else
	{
		RTN = RTN_UNVALID_DATA;  
	}    
}
// ���ܿ� 49 ����
void Ariber_SetSysParaAndRecord(void)
{          
  //�������,���հ�����Э�飬���������������Ҫ��ͨ������У��ſ��Թ��������ݵ������Ŀǰ�ݲ�֧�֣�
	if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Cmd_Group==0xF1)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD49.Cmd_Type)
		{	
      case 0xE0:				//��������ʱ��
        Ariber_SetSysTime();
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
// ��������  : ��ȡϵͳʱ�� E0
void Ariber_GetSysTime(void) 
{
	u16 u16temp = 0; 
  SendHead();    
  //Э�鷢��time is BCD��ʽ
	u16temp=SysData.Sys_Date.StuDate.Year;
	u16temp = IntHexToIntBCD(SysData.Sys_Date.StuDate.Year+2000);    
	SendOneHexToTwoAsciiByte(u16temp>>8);
	SendOneHexToTwoAsciiByte(u16temp);
	SendOneHexToTwoAsciiByte(IntHexToIntBCD(SysData.Sys_Date.StuDate.Month));
	SendOneHexToTwoAsciiByte(IntHexToIntBCD(SysData.Sys_Date.StuDate.Day));
	SendOneHexToTwoAsciiByte(IntHexToIntBCD(SysData.Sys_Date.StuDate.Week));
	
	SendOneHexToTwoAsciiByte(IntHexToIntBCD(SysData.Sys_Date.StuDate.Hour));
	SendOneHexToTwoAsciiByte(IntHexToIntBCD(SysData.Sys_Date.StuDate.Minute));
	SendOneHexToTwoAsciiByte(IntHexToIntBCD(SysData.Sys_Date.StuDate.Second));
  SendTaiDeal(); 	
}	
// ���ܿ�  : 4A ����
void Ariber_GetSysParaAndRecord(void)
{	    	    
  //�������	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Cmd_Group==0xF2)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4A.Cmd_Type)
		{	
			case 0xE0:				//��ȡSM��ʵʱ��
				Ariber_GetSysTime(); 
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
//������������������������������������������������������������������������������
// ��������  : Ariber_SetVoiceMark
// ��������  : ������������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Ariber_SetVoiceMark(void)
{	 
	if(0 == (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0])//�ź�
	{
		DoorPara.VoiceMark = (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[1]*0x100;
		DoorPara.VoiceMark += (*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[2];
		WriteEepromAndChecksum(DoorPara_EepAddr,sizeof(DoorParaStruct),(uchar *)&DoorPara);
	}
	else
	{
		RTN = RTN_UNVALID_DATA;	
	}
	SendDefault();
}
/////////////////////////////
//�Ų��� ���ݳ�ʼ��
void Init_DoorPara(u8 DoorNumber)
{
  switch(DoorNumber)
  {
    case FIRST_DOOR:
      DoorPara=DoorParaDefault;
      WriteEepromAndChecksum(DoorPara_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara);
    break;
    case SECOND_DOOR:
      DoorPara2=DoorParaDefault;
      WriteEepromAndChecksum(DoorPara2_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara2);
    break;
    case THIRD_DOOR:
      DoorPara3=DoorParaDefault;
      WriteEepromAndChecksum(DoorPara3_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara3);
    break;
    case FOURTH_DOOR:
      DoorPara4=DoorParaDefault;
      WriteEepromAndChecksum(DoorPara4_EepAddr,sizeof(DoorParaStruct),(u8 *)&DoorPara4);
    break;
  }
}
// ������������ EA
void Ariber_PlayVoice(u8 DoorNumber)
{
	u8 Ta;
	Ta=(*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Info.Data[0];
  switch(Ta)
  {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			PlayVoiceAction_Test(Ta,1,1);
			SendDefault();
		break;
		#if ReadCard_Open
		case 251:
			Init_TimeListSeg(DoorNumber);
			SendDefault();
			break;
		#endif
    case 253://�ָ���������
      SysConfig = SysConfigDefault;
      WriteEepromAndChecksum(SysConfig_EepAddr,sizeof(SysConfigStruct),(uchar *)&SysConfig);
      #if OUTSWITCH_OPEN
      InOutSwitchConfig=InOutSwitchConfigDefault;
      WriteEepromAndChecksum(InOutSwitchConfig_EepAddr,sizeof(InOutSwitchConfigStruct),(u8 *)&InOutSwitchConfig);
      #endif
			#if DOORACTION_OPEN		
			Init_DoorPara(DoorNumber);
			#endif
			#if ReadCard_Open
			Init_TimeListSeg(DoorNumber);
			#endif

      SendDefault();
      Delay_SystemReset();
      break;
    case 255://����
      SendDefault();
      Delay_SystemReset();
      break;
    default:
			RTN = RTN_UNVALID_DATA;
      break;
  }
}
//4B������
void Ariber_SetArmingPara(void)
{      	    
  //�������	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Cmd_Group==0xF3)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4B.Cmd_Type)
		{	
			case 0xE7:
				Ariber_SetVoiceMark();
				break;	
      case 0xEA:   // �����������ԣ�������Ƭ�� �̵�������
        if(CallSystemAddr==SysConfig.ModAddress)
          Ariber_PlayVoice(FIRST_DOOR);
        if(CallSystemAddr==SysConfig.ModAddress+1)
          Ariber_PlayVoice(SECOND_DOOR);
        if(CallSystemAddr==SysConfig.ModAddress+2)
          Ariber_PlayVoice(THIRD_DOOR);
        if(CallSystemAddr==SysConfig.ModAddress+3)
          Ariber_PlayVoice(FOURTH_DOOR);
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
//������������������������������������������������������������������������������
// ��������  : Ariber_GetVoiceMark
// ��������  : ��ȡ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Ariber_GetVoiceMark(void)
{	  	
	SendHead(); 
	SendOneHexToTwoAsciiByte(0);   
	SendOneHexToTwoAsciiByte((u8)(DoorPara.VoiceMark>>8));
	SendOneHexToTwoAsciiByte((u8)(DoorPara.VoiceMark));     
	SendTaiDeal();  	
} 
//��ȡflash id ED
void Ariber_GetFlashID(void)//ED
{
	SendHead(); 
	if( Flash.SSTFlashState==1 )
	{	
		SendOneHexToTwoAsciiByte(0x8E); // 0x8E-8MBit flash;  
	}
	else if(Flash.SSTFlashState==2)
	{	
		SendOneHexToTwoAsciiByte(0x4A); // 0x4A-32MBit flash
	}
	else
	{	
		SendOneHexToTwoAsciiByte(0x0);  // ����ֵ����
	}
  SendTaiDeal();  	
}
//4C������
void Ariber_GetArmingPara(void)
{
   //�������	   	
  if((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4C.Cmd_Group==0xF4)
	{	//��������
		switch((*ptr_PortResP).ProBuffer.PostProDa.ProPara.ReceCMD4C.Cmd_Type)
		{	
			case 0xE7:
				Ariber_GetVoiceMark();
				break;
			case 0xED:   			//SU��ȡflash id ��Ϣ
				Ariber_GetFlashID();
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
// ��������  : �ʵ粿Э�鴦�� ����
void Common_PostProtocol(void)
{	
  u8  Cmd = (*ptr_PortResP).ProBuffer.PostProDa.Cid2;
  RTN = RTN_NORMAL;
	switch(Cmd)
	{	
    ////////////////////////////////////////
    case 0x4D:	//��ȡʱ��
			Send_MPJ4D();
			break;
    case 0x4E:	//����ʱ��
			Send_MPJ4E(); 					
			break;
    case 0x50:	//��ȡ��ַ	
			Send_MPJ50();
			break;
    case 0xE3:	//�����豸��ַ
      if( CallSystemAddr == (SysConfig.ModAddress&0x0fff))//ֻ��ʵ�ʵ�ַ�ſ����޸ĵ�ַ
        Send_MPJE3(); 					
			break;
    case 0x12:	//�����Ҽ�Ӳ���汾��Ϣ
			Send_MPJ12();
			break;
    /////////////////////////
    //���
    case 0xFB:	
			SetConfigID_FB();
			break;
    case 0xFC:
			ReadConfigID_FC();
      break;
    ////////////////////////
    //ң��
    case 0x45:  
			Send_MPJ45();
      break;
    
    /////////////////////////////////////////
    case 0x51:	//�����Ҽ�Ӳ���汾��Ϣ
			Ariber_51();
      break;
    case 0xA1:	//����ͨ�Ų�����
      if( CallSystemAddr == (SysConfig.ModAddress&0x0fff))//ֻ��ʵ�ʵ�ַ�ſ����޸ĵ�ַ
        Send_MPJA1();
			break;
    ////////////////////////////////////////////
    case 0x48:	//SU��SM�Ĳ�����Ȩ��ȷ����SM�������޸�
		  Ariber_AuthenticationFun();
			break;
		case 0x49:	//SU����SM�ڲ�������������					
		  Ariber_SetSysParaAndRecord();  	 				
			break;
	  case 0x4A:	//SU��ȡSM����Ϣ						
	   	Ariber_GetSysParaAndRecord(); 
			break;
		///////////////NewAdd���ڹ�˾����/////////////
		case 0x4B:	//SU����SM�ڲ�������������
		  Ariber_SetArmingPara();	
			break;
		case 0x4C:	//SU��ȡSM����Ϣ
		  Ariber_GetArmingPara();	
			break;	

		default:
			RTN= RTN_UNKNOW_CMD;     //��Ч����
			break; 
	}
}

