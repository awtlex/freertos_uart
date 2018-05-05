//zhj
//v1.0
//20151219

#include "PostProtocolDeal.h"
#include "globe.h"
#include "config.h"

#define   POST_VER		   	0x10 //Э��汾	  
u8    RTN=0;
u8    Var;
u16	  CallSystemAddr;

// ��������  : �ʵ粿Э�鷢������֡ͷ����
// �������  : RTN ����״̬.
void SendHead(void)
{	
	u8 CID1;
	ResetBuffer();
	SendByte('~');
//  if(Var == 0x20)
//  {
//    SendOneHexToTwoAsciiByte(Var);
//  }
//  else
//  {
//    SendOneHexToTwoAsciiByte(POST_VER);
//  }
	SendOneHexToTwoAsciiByte(Var);
	if( (CallSystemAddr & 0x00ff) == 0x00ff ) CallSystemAddr = SysConfig.ModAddress;
	SendOneHexToTwoAsciiByte(CallSystemAddr);
  if(Var == 0x20)
  {
    CID1=0xD0;
  }
  else
  {
    CID1=0x80+((CallSystemAddr>>8)&0x0F);
  }
	SendOneHexToTwoAsciiByte(CID1);
	SendOneHexToTwoAsciiByte(RTN);  //RTN
	SendOneHexToTwoAsciiByte(0);	  //���ⷢ�������峤��
	SendOneHexToTwoAsciiByte(0);
}
// ��������  : �ʵ粿Э��ط���У��ͺ�����Ҫ��HEXת��ASC����У��ͣ�
// �������  : len У�����ݳ���.
void SendPostChksum(void)  
{
	u8 *p;
	u16 i,ChksumData=0;
	u8 tempSum;	

	p=&(*ptr_PortResP).ProBuffer.DaBuf[1];   
	for(i=0;i<(*ptr_PortResP).Pro_Lenth-1;i++)
	{	
		ChksumData+=*p++;
	}
	ChksumData=~ChksumData+1;

	tempSum=(u8)(ChksumData>>8);
	SendOneHexToTwoAsciiByte(tempSum);
	SendOneHexToTwoAsciiByte((u8)ChksumData);	 
}
// ��������  : �������ͻ���̶�λ�ò���
// �������  : FixupNo λ�� valus ����.
void SendFixupByte(u8 FixupNo,u8 valus)
{
	(*ptr_PortResP).ProBuffer.DaBuf[FixupNo]=Hi_HexToAsc(valus);
	(*ptr_PortResP).ProBuffer.DaBuf[FixupNo+1]=Low_HexToAsc(valus);
}
// ��������  : �ʵ粿Э�鷢�����ݳ���
void SendDataLen(void)
{
	Taglchksum  tmplen;
	u16 word;
	u8 temp;

	tmplen.word = (*ptr_PortResP).Pro_Lenth-13;
	tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
	tmplen.div.lChksum=~tmplen.div.lChksum+1;
	word=tmplen.word;

	temp=word>>8;
	SendFixupByte(9,(u8)temp);
	SendFixupByte(11,(u8)word);
}
// ��������  : �ʵ粿Э�鷢��֡β
void SendTail(void)
{	
	SendByte(0x0d);
}
// ��������  : �ʵ粿Э�����ݳ���У��
u8 ChkPostDataSum(void)
{
	Taglchksum  tmplen;
	u16  word;

	word=TwoCharToInt((*ptr_PortResP).ProBuffer.DaBuf[5],(*ptr_PortResP).ProBuffer.DaBuf[6]);	//�յ�����Ϣ�峤��
	tmplen.word = (*ptr_PortResP).Pro_Lenth-18;					//ʵ���յ�����Ϣ���ۼӵĳ���,�ٶ�Щ����У�� 
	tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
	tmplen.div.lChksum=~tmplen.div.lChksum+1;
	
	if(word==tmplen.word)
		return 1;
	else 
		return 0;
}
// ��������  : �ʵ粿Э��Ӧ��������������
void SendDefault(void)
{	
	SendHead();
	SendDataLen();
	SendPostChksum();
	SendTail();
	StartSend();
}
// ��������  : �ʵ粿Э��Ӧ��Ľ�β���� �������� ����У�������β ����������
void SendTaiDeal(void)
{	
	SendDataLen();
	SendPostChksum();
	SendTail();
	StartSend();
}
// ��������  : �ʵ粿Э��������ݵ�У����㷨
// �������  : p1 ҪУ������� ReDaLen ���ݳ���.
u8 chk_post_chksum(u8  *p1,u16  ReDaLen)  
{	
  u16 i,csA,csB,Sum=0;
	u8 TA,TB,TeC;

	for(i=0;i<ReDaLen;i++)
	{	
		Sum += *p1++;
	}
	Sum = ~Sum+1;
	TA = *p1++;
	TB = *p1++;
	TeC=TwoAscTOHex(TA,TB);
	csA = (u16)TeC;
	csA = csA<<8;
	TA = *p1++;
	TB = *p1++;
	TeC=TwoAscTOHex(TA,TB);
	csB =(u16)TeC;
	if((u16)Sum==(u16)(csA+csB))
	{	
	  return 1;
	}
	else
	{	
	  return 0;
	}
}
// ��������  : �ʵ粿Э�����
void DealPostProtocol(void)
{ 		  
	u8 utemp;	
	//u8 Var; 	
	u8 Cid1;	 			
	Var = TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).ProBuffer.DaBuf[2]);//Э��汾	
	CallSystemAddr=TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[3],(*ptr_PortResP).ProBuffer.DaBuf[4]);//��λ�����е�ַ 
	Cid1=	TwoAscTOHex((*ptr_PortResP).ProBuffer.DaBuf[5],(*ptr_PortResP).ProBuffer.DaBuf[6]);
	CallSystemAddr+=(Cid1&0x0F)*0x100;
	Cid1&=0xF0;	
	//���ŵ�ַ
	switch(SysConfig.DeviceType)
	{
		case OneDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		case TwoDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+1)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		case ThreeDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+1
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+2)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		case FourDoorController:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+1
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+2
				&& CallSystemAddr!=(SysConfig.ModAddress&0x0fff)+3)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
		default:
			if(CallSystemAddr!=(SysConfig.ModAddress&0x0fff) && (CallSystemAddr&0x00ff)!=0x00ff)
			{
				Init_protocol_DATA_DEAL();
				return;
			}
			break;
	}	 
	if((Cid1==0x80)||(Cid1==0x90)||(Cid1==0xD0))
	{	
    utemp=chk_post_chksum((u8 *)&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-6);		
    //��У����Ƿ���ȷ
    if(utemp==1) 		
    {  
      // ��ASCIIת����HEX
      BufAscToHex(&(*ptr_PortResP).ProBuffer.DaBuf[1],(*ptr_PortResP).Pro_Lenth-2);
      if((*ptr_PortResP).ProBuffer.PostProDa.Cid2==0xF8)
      {// ISP���س���
        iap_data.Address = SysConfig.ModAddress;
        goto_iap();	
      }        
      else if((*ptr_PortResP).ProBuffer.PostProDa.Cid2==0x50 || (*ptr_PortResP).ProBuffer.PostProDa.Cid2 == 0x48)	//��ȡ��ַ �㲥ֱ����ͨ�� ����Ҫ���Ȩ��
      {	
        RTN=0;		
        DealPostPro();	
      }  
      else
      {		 				    
        //if((Var==0x10)||(Var==0x20)||(Var==0x21)) //���ж�Э��汾��
        {	
          if(ChkPostDataSum())
          { 
            if(CallSystemAddr==(SysConfig.ModAddress&0x0fff) || (CallSystemAddr&0x00ff)==0x00ff)
            {
              if(SysData.SysState[0]!=SysState_REALTIME || DoorPara.ProtocolPasswordEnableFlag == 0)
              {
                RTN=0;      
                if(SysData.SysState[0] == SysState_CONFIG)
                  SetSecondTimer(Door_SysStateSecondTimer,Door_SysStateSecondTime);
                DealPostPro();
              }
              else
              {
                RTN = RTN_NOACCESS;
              }
            }
						else if(CallSystemAddr==(SysConfig.ModAddress&0x0fff)+1)
						{
							if(SysData2.SysState[0]!=SysState_REALTIME || DoorPara2.ProtocolPasswordEnableFlag == 0)
							{
								RTN=0;       
								if(SysData2.SysState[0] == SysState_CONFIG)
									SetSecondTimer(Door2_SysStateSecondTimer,Door_SysStateSecondTime);
								DealPostPro();
							}
							else
							{
								RTN = RTN_NOACCESS;
							}
						}
						else if(CallSystemAddr==(SysConfig.ModAddress&0x0fff)+2)
						{
							if(SysData3.SysState[0]!=SysState_REALTIME || DoorPara3.ProtocolPasswordEnableFlag == 0)
							{
								RTN=0;  
								if(SysData3.SysState[0] == SysState_CONFIG)
									SetSecondTimer(Door3_SysStateSecondTimer,Door_SysStateSecondTime);
								DealPostPro();
							}
							else
							{
								RTN = RTN_NOACCESS;
							}
						}
						else if(CallSystemAddr==(SysConfig.ModAddress&0x0fff)+3)
						{
							if(SysData4.SysState[0]!=SysState_REALTIME || DoorPara4.ProtocolPasswordEnableFlag == 0)
							{
								RTN=0;
								if(SysData4.SysState[0] == SysState_CONFIG)
									SetSecondTimer(Door4_SysStateSecondTimer,Door_SysStateSecondTime);
								DealPostPro();
							}
							else
							{
								RTN = RTN_NOACCESS;
							}
						}
          }
          else
          {	
            RTN = RTN_PARACHAK_ERR;	//���������ۼӺͼ�鲻��
          }									
        }
//        else
//        {
//          RTN=RTN_VER_ERR;//�汾������
//        }
      }
    }
    else
    {
      RTN=RTN_CMDCHK_ERR;//�ۼӺͼ�鲻��
    }

		if(RTN)
		{	//��Ϣ�����
			SendDefault();
		}
	}
	else
	{
	  Init_protocol_DATA_DEAL(); 
	}
}
// ��������  : �ʵ粿Э�鴦��
void  DealPostPro(void)
{	
  switch((*ptr_PortResP).Port_Type)
  {
    case COM0:
			Common_PostProtocol();
      #if DOORACTION_OPEN
      if(RTN != RTN_UNKNOW_CMD) return; 
      Door_PostProtocol();
      #endif
			#if ReadCard_Open
			if(RTN != RTN_UNKNOW_CMD) return; 
			Card_PostProtocol();
			#endif
      break;
    case COM1:
		#if CPU_Type == CPU_STM32F103
		case COM3:
		#endif
			Common_PostProtocol();
			break;
    default:
      break;
  }
}


