//zhj
//v1.0
//20151110

#include "protocol.h"
#include "globe.h"
#include "config.h"


//ͨ��Э������		
ProTypeStruct const ProType[2]=	
{
	{'~',1,0x0D},	  	//�ʵ粿Э��	��~�� 0x7E
	{'#',1,0x0D},	  	//�Ž�Э��		��#��	0x23
}; 

//�������ݽṹ��  
ProtocolResource  PortRes[Port_Buffer_Count];        //���ջ�������Դ ����ʹ��0�� ����ʹ��1	

//�ⲿʹ�õ�ָ��
ProtocolResource  *ptr_PortResP;	//ָ��ʹ�ý��ջ�������Դ 

////////////////////////////////////////////////////////////////////////

void Protocol_TimerDeal(void)
{
  u16 i;
  for(i=0;i<Port_Buffer_Count;i++)
  {
    switch(PortRes[i].Delay_ms)
    {
      case 0xffff:
        break;
      case 0:
        PortRes[i].Delay_ms = 0xffff;
        PortRes[i].ResState = ConnectState_IDLE;		//����ԴΪ����״̬
        break;
      default:
        if(PortRes[i].Delay_ms) PortRes[i].Delay_ms--;
        break;
    }
  }
}
/////////////////////////////////////////////////////////////////////////
// ��������  : ��ʼ��Э����Դ״̬�����ȡ�������
void Init_protocol(u8 PortBufferNum)
{ 	  	
	PortRes[PortBufferNum].ResState=ConnectState_IDLE;		//����ԴΪ����״̬
	PortRes[PortBufferNum].SendMode=0;
	Protocol_timer_close(PortBufferNum);  									  //�رն�ʱ��1
}
// ��������  : ���ݴ���״̬��ͨ����ʼ��
void Init_protocol_DATA_DEAL(void)
{ 
  u8 i;  	
	for(i=0;i<Port_Buffer_Count;i++)
	{
		if(PortRes[i].ResState==ConnectState_DATA_DEAL)//��ͨ�����ݴ���	
		{
			Init_protocol(i); 			
		}
	}
}
////////////////////���պ�������//////////////////////////////////////////
// ��������  : Э�����ݽ��ս�������
// �������  : data ���յ�һ���ֽ�.
void ReceMainDeviceFun0(u8 PortBufferNum,u8 data)
{	
	int i;
	// δ�յ�֡ͷ
	if(PortRes[PortBufferNum].Pro_Lenth >= MAXPCReLen)  //Э�鳬��
	{
		Init_protocol(PortBufferNum);
	}
	
  	switch(PortRes[PortBufferNum].Pro_Status)
	{
	case 0://û��ȷ��Э������
	  for(i=0;i<ProTypeCount;i++)
		{	  			
			if(ProType[i].RHead==data)
			{	
				PortRes[PortBufferNum].Pro_Type=i; 				//Э������
				PortRes[PortBufferNum].Pro_Lenth=0;	   		//Э���ʵ�ʳ��ȣ�ʵʱ����					
				PortRes[PortBufferNum].ProBuffer.DaBuf[PortRes[PortBufferNum].Pro_Lenth++]=data;	
				switch(PortRes[PortBufferNum].Pro_Type)
				{
					case 0:

					case 1:
					PortRes[PortBufferNum].Pro_Status=1;//��βЭ��
					break;
				}
				break;					
			}  			
		}		
	  break;
	  
	case 1:// ���յ�֡ͷ ��βЭ��
			PortRes[PortBufferNum].ProBuffer.DaBuf[PortRes[PortBufferNum].Pro_Lenth++]=data;
			if(data==ProType[PortRes[PortBufferNum].Pro_Type].RTail)
			{					
				Protocol_timer_close(PortBufferNum);  // �ؽ��ն�ʱ��	 						
				PortRes[PortBufferNum].ResState=ConnectState_RECEIVE_FINISH;  //��ʾ�ѽ�����һ��������Э��   														
			} 				
	  break;

		default:
			Init_protocol(PortBufferNum);
			break;
	}
}
// ��������  : Э��������ݽӿ� �����ж���ʹ��
// �������1  : PortNum  ʹ�õĶ˿������µĶ˿ڱ�� ���紮��1�ʹ���2����ʹ��ͬһ��������
// �������2  : chr		���յ����� 
void BackComm0_RX(u8 PortNum,u8 PortBufferNum,u8 data)
{ 
	if(PortRes[PortBufferNum].ResState==ConnectState_IDLE)    //��Դ����
	{
    Protocol_timer_open(PortBufferNum,PortIntervalTime);    //��PC����Ķ�ʱ����ʱ�䵽δ��ȫ���屾����������.������20000L 
	  PortRes[PortBufferNum].Port_Type = PortNum;             //��¼ʹ����Դ�Ķ˿�			
		PortRes[PortBufferNum].Port_BufferNum = PortBufferNum;
		PortRes[PortBufferNum].ResState=ConnectState_RECEIVE;		//����ԴΪ����״̬
    PortRes[PortBufferNum].Pro_Status = 0; 
    PortRes[PortBufferNum].Pro_Lenth=0;	   		              //Э���ʵ�ʳ��ȣ�ʵʱ����
		ReceMainDeviceFun0(PortBufferNum,data);
	}
	else if(PortRes[PortBufferNum].ResState==ConnectState_RECEIVE)
	{
		if(PortRes[PortBufferNum].Port_Type==PortNum) //�ǲ�������˿��ٽ�������
		{ 		    
      Protocol_timer_open(PortBufferNum,PortIntervalTime);
			ReceMainDeviceFun0(PortBufferNum,data);
    }
	} 	
}

/////////////////////���ͺ�������/////////////////////////////////
// ��������  : ͨ��0��Э�鷢�����ݽӿ� �����ж���ʹ��
// �������  : ComNum.//�˿ں�
void BackComm0_TX(u8 ComNum,u8 PortBufferNum)
{	 
	switch(PortRes[PortBufferNum].SendMode)
	{
		case 1://�ְ�����
			if(PortRes[PortBufferNum].Pro_Lenth==0)//���ͽ���
			{	
				Init_protocol(PortBufferNum);
				STM32_USARTx_TX_Finish(ComNum);
			}
			else
			{ 			
				if(PortRes[PortBufferNum].SendCount % 20 == 0)
				{
					if(PortRes[PortBufferNum].Pro_Lenth==0)//���ͽ���
					{	
						Init_protocol(PortBufferNum);
						STM32_USARTx_TX_Finish(ComNum);
					}
					else
					{
						STM32_USARTx_TX_Finish(ComNum);
						SetTimer(PortDelaySendTimer,PortDelaySendTime);
					}
				}
				else
				{
					STM32_USART_SendData(ComNum,*PortRes[PortBufferNum].ptr_sending++);
					PortRes[PortBufferNum].Pro_Lenth--;
					PortRes[PortBufferNum].SendCount++;
				}
			}  
			break;
    #if Bluetooth4_Open
		case 2:
			switch(PortRes[PortBufferNum].SendCount)
			{
				case 1:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0x24);
				break;
				case 2:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0x00);
				break;
				case 3:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0x01);
				break;
				case 4:
					PortRes[PortBufferNum].SendCount++;
					STM32_USART_SendData(ComNum,0xFF);
				break;
				case 5:
					PortRes[PortBufferNum].SendMode=0;
					STM32_USARTx_TX_Finish(ComNum);
				break;
			}
			break;
    #endif
		default:
			if(PortRes[PortBufferNum].Pro_Lenth==0)//���ͽ���
			{	
				Init_protocol(PortBufferNum);
				STM32_USARTx_TX_Finish(ComNum);
			}
			else
			{ 			
				STM32_USART_SendData(ComNum,*PortRes[PortBufferNum].ptr_sending++);
				PortRes[PortBufferNum].Pro_Lenth--;
			}  
			break;
	}
}
///////////////////////////////////////////
// ��������  : ��λ���ͻ�����
void ResetBuffer(void)
{		
	(*ptr_PortResP).ptr_sending = &(*ptr_PortResP).ProBuffer.DaBuf[0];
	(*ptr_PortResP).Pro_Lenth = 0;
	(*ptr_PortResP).SendCount = 0;
}
// ��������  : ��Ҫ���͵��ֽ����뷢�ͻ�����
// �������  : TempChar ��������.
void SendByte(u8 TempChar)
{	
  if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
  {
    *(*ptr_PortResP).ptr_sending++=TempChar;
    (*ptr_PortResP).Pro_Lenth++;
  }
}
// ��������  : ����n���ֽ�
// �������  : pchar �������� count ����.
void SendHexBytes(u8 *pchar,u8 count)
{
	if(count)
	{
		while(count--)	
			SendByte(*pchar++);
	}
}
/////////////
// ��������  : ����һ���ֽڣ�һ���ֽ���ֽ������ASCII�뷢�ͣ��ȷ���High 4λ������Low 4λ
// �������  : c1 ��������.
void SendOneHexToTwoAsciiByte(u8 c1)
{
	SendByte(Hi_HexToAsc(c1));
	SendByte(Low_HexToAsc(c1));
}
// ��������  : �������ַ���
void Send_Word(u16 TemInt)  
{	
	u8  Te;
	Te = (u8 )(TemInt>>8);
	SendOneHexToTwoAsciiByte(Te);
	SendOneHexToTwoAsciiByte(TemInt);
}
// ��������  : ����n���ֽڣ���2*n��ASCII��
// �������  : pchar �������� count ����.
void SendAsciiBytes(u8 *pchar,u8 count)
{
	if(count)
	{
		while(count--)	
			SendOneHexToTwoAsciiByte(*pchar++);
	}
}
// ��������  : ����һ��Float������
void SendOCEDataFloat(float ocedt)   
{
	uni_float ddd;
	uni_float *pidat=&ddd;
	pidat->zfloat=(float)ocedt;
	{	
		SendOneHexToTwoAsciiByte(pidat->byte[0]);
		SendOneHexToTwoAsciiByte(pidat->byte[1]);
		SendOneHexToTwoAsciiByte(pidat->byte[2]);
		SendOneHexToTwoAsciiByte(pidat->byte[3]);
	}
}
//////////////////////////////////////////////////////
// ��������  : ��ʼ���ͻ���������
void StartSend(void)
{	 
  u8 i;
  for(i=0;i<Port_Buffer_Count;i++)
  {
    if(PortRes[i].ResState==ConnectState_DATA_DEAL)//��һͨ�� ��ͨ�������Ѿ�����	
    {	   		
      PortRes[i].ResState=ConnectState_SEND;  /*�����ڷ���״̬*/ 	   			
      switch(PortRes[i].Port_Type)
      {
        case COM0://����1
          if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
          {
            PortRes[i].ptr_sending = &PortRes[i].ProBuffer.DaBuf[0];				    
            GPIO485Direction_Send(); 		
            STM32_USART_SendData(COM0,*PortRes[i].ptr_sending++);
            STM32_USART_enable(COM0);
            (*ptr_PortResP).Pro_Lenth--;
						(*ptr_PortResP).SendCount++;
          }
          else
          {
            Init_protocol(i);
          }
          break;
        case COM1://����2
          if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
          {
            PortRes[i].ptr_sending = &PortRes[i].ProBuffer.DaBuf[0];				     		
            STM32_USART_SendData(COM1,*PortRes[i].ptr_sending++);
            STM32_USART_enable(COM1);
            (*ptr_PortResP).Pro_Lenth--;
						(*ptr_PortResP).SendCount++;
          }
          else
          {
            Init_protocol(i);
          }
          break;
				#if CPU_Type == CPU_STM32F103	
				case COM2://����3
          if((*ptr_PortResP).Pro_Lenth < MAXPCReLen)
          {
            PortRes[i].ptr_sending = &PortRes[i].ProBuffer.DaBuf[0];				     		
            STM32_USART_SendData(COM2,*PortRes[i].ptr_sending++);
            STM32_USART_enable(COM2);
            (*ptr_PortResP).Pro_Lenth--;
						(*ptr_PortResP).SendCount++;
          }
          else
          {
            Init_protocol(i);
          }
          break;
				#endif
        default:
          PortRes[i].ResState=ConnectState_IDLE;  /*����״̬*/
          break;
      }
    } 
  }
}
// ��������  : ��ʼ���ͻ���������
void TimerDelayStartSend(u8 ComNum,u8 PortBufferNum)
{	 
	if(PortRes[PortBufferNum].ResState==ConnectState_SEND)//��һͨ�� ��ͨ�������Ѿ�����	
	{	   			   	
		if(ComNum == COM0)
		{
			GPIO485Direction_Send();
		}
		STM32_USART_SendData(ComNum,*PortRes[PortBufferNum].ptr_sending++);
		STM32_USART_enable(ComNum);
		PortRes[PortBufferNum].Pro_Lenth--;
		PortRes[PortBufferNum].SendCount++;
  }
}
// ��������  : ��ʼ���ͻ���������
void TimerDelaySendReply(u8 ComNum,u8 PortBufferNum)
{	 
	if(PortRes[PortBufferNum].ResState==ConnectState_RECEIVE)//��һͨ�� ��ͨ�������Ѿ�����	
	{	 
		if(ComNum == COM0)
		{
			GPIO485Direction_Send();
		}
		PortRes[PortBufferNum].SendCount = 1;
		STM32_USART_SendData(ComNum,0xAA);
		STM32_USART_enable(ComNum);
	}
	else
	{
		SetTimer(PortDelaySendTimer,PortDelaySendTime);
	}
}
/////////////////////////////////////////////////////////////////////
// ��������  : Э�鴦��
void DealProtocol(void)
{	
  if(PortRes[0].ResState==ConnectState_RECEIVE_FINISH)//��һ��Э���ѽ��������Ϊ����ʱ��׼�����
  {	
    PortRes[0].ResState=ConnectState_DATA_DEAL;//��ͨ�����ݴ���
    ptr_PortResP = &PortRes[0];		
	
    switch(PortRes[0].Pro_Type)
    {
      case 0:				//�ʵ粿Э�� 					
        DealPostProtocol();	 
        break;
		
      case 1:				//�Ž�Э��
        DealCLDoorProtocol();  
        break;
     
      default:
        PortRes[0].ResState=ConnectState_IDLE;
        break;
    }	
	
    if(PortRes[0].ResState==ConnectState_DATA_DEAL)
    {
      PortRes[0].ResState=ConnectState_IDLE;
    } 			
  }
}  



