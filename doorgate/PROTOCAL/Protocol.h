#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"

#define PortIntervalTime     2000

///////////////////////////////////////////////////////////////////////////////

typedef struct	
{	
	u8 RHead;					//ͷ�ַ���
	u8 RSLenth;					//Э����̳��� ��βЭ��ʹ��
	u8 RTail;					//β�ַ���
}ProTypeStruct;
/////////////////////////////////////////////////////
//ö����
//��ͬ�˿�ʹ�õ�ͨ����Դ  PortRes
enum
{
	ProtocolRes_USART = 0,	//����1
  ProtocolRes_USART2 = 1, //����2 stm32f030 ����ģ��
#if CPU_Type == CPU_STM32F103
  ProtocolRes_USART3 = 1, //����3	stm32f103 ����ģ��
  ProtocolRes_USART5 = 1, //����3
#endif
//	ProtocolRes_TCP   = 2,	//����
};
//ͨ��״̬ö��
enum 
{
	ConnectState_IDLE = 0, 					//����״̬
	ConnectState_RECEIVE= 1,				//���ڽ���
	ConnectState_RECEIVE_FINISH= 2,	//�������
	ConnectState_DATA_DEAL= 3,			//���ݴ���״̬
	ConnectState_SEND= 4						//����״̬
};

#define     Port_Buffer_Count      2     //Э�黺�����
#define    	MAXPCReLen     	 	    1200 	//PC���ڽ��յ���󳤶�4000  to 2000

/*************���ݽṹ*************************/
/****************************************************************************************
// ���ܿ�  : ʱ�ι��ýṹ��  
****************************************************************************************/
#define	WEEK_TABLE_LIST_NUM		10					//������ ׼��ʱ�Σ��ܹ���ʱ�α�  ������16�ŵ�,������������
//����������ʱ��Ρ�������������������������������������
typedef struct
{
  //��ʼʱ��
  u8 StartTimeHH;
  u8 StartTimeMM;
  //����ʱ��
  u8 EndTimeHH;
  u8 EndTimeMM;
}Ariber_SegTime;
/*************�ʵ粿Э��**********************************************************************/
/****************************************************************************************
// ���ܿ�  : E3���� �����豸��ַ  
****************************************************************************************/
typedef struct
{	
  // ���͡��ꡢ�¡��ա�ʱ�����룬 ��־��������ʱ��  7�ֽ�
  u8	century;	//����
  u8	year;	  	//��
  u8 	month;		//��
  u8 	day;			//��
  u8	week;
  u8 	hour;			//ʱ
  u8 	minute;		//��
  u8 	second;		//��
}ReceCMD4EStruct;
/****************************************************************************************
// ���ܿ�  : E3���� �����豸��ַ  
****************************************************************************************/
typedef struct
{	
	u16 SetAddr;
}ReceCMDE3Struct;
/*******************************************************************************************
// ���ܿ�  : 48 ����  
******************************************************************************************/
typedef struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
  u8 Password[5];   
}ReceCMD48Struct;
/****************************************************************************************
// ���ܿ�  : 49 ����  
****************************************************************************************/
//������������������������������������������������
typedef  struct
{ // ���͡��ꡢ�¡��ա�ʱ�����룬 ��־��������ʱ��  7�ֽ�
	u8	century;	//����
	u8	year;	  	//��
	u8 	month;		//��
	u8 	day;			//��
	u8	week;
	u8 	hour;			//ʱ
	u8 	minute;		//��
	u8 	second;		//��
}Ariber_SetDateAndTimeStruct;
typedef struct
{
	u16  WritePoint;			
	u16  ReadPoint;
	u8 	 LoadpMF;     //��ע
	u8   ChkSum;			//ΪУ�������1 byte���ڴ�ռ�
}Record_Point;

//�������������ӿ��š�������������������������������������
//D7	D6	D5��D4	D3��D2��D1��D0
//=0	=0	X��X	������һ��������׼���б����ʽ��������0��15
//1	0	X��X	������/�ǹ�����׼���б����ʽ�������ţ��ţ�0��3
//	1	X��X	����׼��ʱ������ 
typedef struct
{
  u8     CardNO[5];             //IC����
  u8     UserID[4];		        	//�û����
  u8     UserPassword[2];       //�û�����
  //��Ч����
  u8     Century;                //����
  u8     Year;                   //�� 0~99
  u8     Month;                  //��
  u8     Day;                    //��
  //�û�Ȩ��
  u8     UserPermission  ;       //�û�Ȩ��
}Ariber_AddCardListStruct;

enum
{
  DELMODE_BYUSERCARDNO        = 0x00,	//ɾ����Ӧ����
  DELMODE_BYUSERID            = 0x01,	//�����û�ID�������ɾ��
  DELMODE_ALL                 = 0x02  //ɾ��ȫ������   
};
typedef struct
{
  u8     DelMode;            //ɾ��ģʽ
  u8     CardNO[5];          //��ɾ���Ŀ���      
}Ariber_DelCardListStruct;

//�俨״̬������λ��Э������·�
enum
{
  CONFIG_STATE_START          = 0x00,
  CONFIG_STATE_INPROCES       = 0x01,
  CONFIG_STATE_END            = 0x02
};
typedef struct
{
  u8     ConfigState;          //�俨״̬ 
  u8     ConfigCount;          //��������
  Ariber_AddCardListStruct     NewAddCardList[3];     //�����������ſ�    
}Ariber_BatchAddCardStu;
//������������������׼��ʱ���������������������������������������
typedef   struct
{
  u8 				TableNO;            //���
  u8 				WeekDay;            //����    
  Ariber_SegTime  SegTime[6];   //���趨��ָ������ʱ��
}Ariber_SetWeekSegStu;
//�����ջ���Ϣ��ʱ��
#define		WORK_REST_SEG_NUM		4
typedef  struct
{
  u8              GroupNO ;      //���
  Ariber_SegTime  SegTime[4];    //֧��4��ʱ��        
}Ariber_WorkDayTimeSegStu;	 
//��������������Ϣ�ա�������������������������������������
typedef   struct
{
  u8 RestDay[2];     //��Ϣ�գ������BCD��ʽ�����ޣ�����0xFF���档���磺7��0XFF����ʾ����������Ϣ��
}Ariber_SetWeekRestDay;
//�����������ڼ���Ϣ�ա�������������������������������������
typedef   struct
{
  u8   Month;            //��
  u8   Day;              //��    
}Ariber_HolidayStu;  	
//���������������Ž������������������������������������������� 
typedef   struct
{
	u8 Channel;			//���õ�ͨ����
	u8 SetValue[2];	//���õ�ֵ	
}Ariber_SetDoorParaStu;

typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8             				      Data[20];           //��ʱ����
    Ariber_SetDateAndTimeStruct	SetDateAndTime;			//��������ʱ��E0
    Ariber_WorkDayTimeSegStu   	SetTimeSeg;         //����ʱ��E1��E2

    Ariber_AddCardListStruct  	NewAddCardList;     //�����õ��û�����E3
    Ariber_DelCardListStruct  	NewDelCardList;     //��ɾ�����û�E4
    //***************ʱ������ 
    Ariber_SetWeekRestDay       SetWeekRestDay;     //����������Ϣ��EA   
    Ariber_HolidayStu           Holiday;            //���ӻ�ɾ���Ľڼ���EB,EC
    Ariber_SetWeekSegStu        SetWeekListSeg;     //�������ڱ�F1
    //************�����Ž�����** 
    Ariber_SetDoorParaStu				SetDoorPara;  
  }Info; 
}ReceCMD49Struct;

/*******************************************************************************************
// ���ܿ�  : 4A ����  
******************************************************************************************/
typedef   struct
{
  u8 TableNO;            //���
  u8 WeekDay;            //����    
}Ariber_ReadWeekStu;
typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8            	Data[20];           //��ʱ����
    //***************ʱ������ 
    Ariber_ReadWeekStu  ReadWeekListSeg;       //��ȡ���ڱ�
  }Info;      
}ReceCMD4AStruct;
/*******************************************************************************************
// ���ܿ�  : 4B ����  
******************************************************************************************/
//���ò��������Ĳ���
typedef   struct
{
	u8 Type;          	//��������  Ĭ��04
	u8 ShiDuanNumber; 	//��ֵĬ��48  16���� 0x30
	u8 NBytes;        	//Ĭ�� 01
	u8 ShiDuan[6];   	  //Ĭ�� 0xFFFFFFFFFFFF
	u8 HaveMan;      	  //00 ����ѡ�����豸 01��ѡ�����豸
	u8 Bftj;						//�������� 	
//��0λ =1��ѡԶ�̲��� 	=0����ѡ  
//��1λ	=1��ť���� 			=0����ѡ 
//��3λ	=1ʱ�ε㲼�� 		=0����ѡ 
//��4λ	=1ʱ�����Զ����� =0����ѡ ����λ0
	u8 LianDong[8];  	//LianDong[0];  ��0λ �Ƿ�ˢ�� ��1λ �Ƿ����� ��2λ ����澯 ��3λ �𶯸澯 ��4λ Կ�׸澯
                    //LianDong[1];  Ĭ��0
                    //LianDong[2];  =1 �ſ����Ź��� =2�ſ����Ź���ʱ =3 ����ˢ����ť����
                    //LianDong[3];  ��LianDong[2]=2ʱ Ϊ��ʱ��ʱ��  
                    //LianDong[4];  =0x00����� =0x01�����
	u8 Chk;    
}Ariber_BFLD;//��������
typedef   struct 
{
	u8			      ID_Flag;		//��ʶ���ݱ���Ϊ0x01	
 	Ariber_BFLD		BFLD_Para;
}Ariber_SetBFLDStruct;
typedef   struct 
{
	u8 		Reserved;		//
	u8		BF_Type;		//�������ͣ�0�������Զ���1�������ֶ���2�������Զ���3�������ֶ���
 	u16		BF_TimeM;		//����ʱ��
}Ariber_SetBFEnableStruct;
typedef   struct 
{
	u16		BF_TimeS;	      //����ʱ��
	u8 		ShieldAlarm;    //��������0���� 1����
	u8		CFLD_DoorKey;   //Կ�׿�����������0���� 1����
}Ariber_SetBFLDAlarmStruct;
typedef   struct 
{
	u8		groupNo;			//������� �˰汾ֻ֧��1������ �̶�Ϊ0x01
	u8 		useEnabled;		//��Ӧ����ʹ�� 1������ 0���ر�
	u8		password[3];	//6λ���� ASCII
	u8		backup[3];		//����
}Ariber_SetPasswordStruct;
typedef   struct 
{
	u8		data[64];			
}Ariber_BluetoothPasswordStruct;
typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8 			        			        Data[20];       	//��ʱ����
    Ariber_SetBFEnableStruct 		  SetBFEnable;     	//���ò���ʹ��
    Ariber_SetBFLDAlarmStruct 	  SetBLDFAlarm;     //���ñ�������
    Ariber_SetBFLDStruct 				  SetBFLDPara;     	//���ò��������Ĳ���
    Ariber_SetPasswordStruct		    SetPassword; 		//��������
    Ariber_BluetoothPasswordStruct  BluetoothPassword;
  }Info; 
}ReceCMD4BStruct;
/*******************************************************************************************
// ���ܿ�  : 4C ����  
******************************************************************************************/
typedef   struct
{	
  u8 Cmd_Group;
  u8 Cmd_Type;
    union
  {
    u8 		Data[20];       	//��ʱ����
  }Info;
}ReceCMD4CStruct;
typedef   struct
{	
	u32 USART_BaudRate;
}ReceCMDA1Struct;
/*******************************************************************************************
// ���ܿ�  : �ʵ粿Э������Ĺ�����ṹ(����)
******************************************************************************************/ 
typedef   union    
{	
	u8                      OtherDaBuf[MAXPCReLen-7];
  ReceCMD4EStruct         ReceCMD4E;
	ReceCMDE3Struct         ReceCMDE3;
	ReceCMD48Struct         ReceCMD48;
	ReceCMD49Struct         ReceCMD49;
	ReceCMD4AStruct         ReceCMD4A;
	ReceCMD4BStruct         ReceCMD4B;
	ReceCMD4CStruct         ReceCMD4C;
	ReceCMDA1Struct         ReceCMDA1;
}ProParaStruct;
// �ʵ粿Э��ṹ(~200160420000FDB1)	
typedef   struct
{	
	u8   Head;   				
	u8   Ver;  
	u8   Addr;
	u8   Cid1;
	u8   Cid2;
	u16  Length; 				
	ProParaStruct		ProPara;  	
}PostProProStruct; 
/*************��˾�Ž�Э��*************************/
//�洢������
typedef   struct
{	
  u8 	Storage_Type;		//0��EEP��1��Flash
  u8 	Cmd_Type;				//0������1��д��2������
	u8 	Storage_Add[4];	//�洢����ַ
	u8 	Info_Len;	 			//��Ϣ����
  u8 	Data[48];       //��Ϣ��
}MPDoorCMD37Struct;
typedef   struct
{	
  u16 SetAddr;	//��ַ
}MPDoorCMDA1Struct;
typedef   struct
{	
	u8 YearH; 				//
	u8 YearL; 				//
	u8 Month;				  //����
	u8 Day;				    //����
	//u8 Week;				//����               
	u8 Hour;	   			//24h��
	u8 Minute;				//����
	u8 Second;				//��	

}MPDoorCMDA3Struct;
typedef   struct
{	
  u8 RunState;
}MPDoorCMDA4Struct;
typedef   struct
{	
  u8  ResetSystem;
}MPDoorCMDA5Struct; /*ϵͳ��λ*/
typedef   union
{	
	u8 OthDaBuf[MAXPCReLen-9];
	MPDoorCMD37Struct       MPDoorCMD37;//�洢������	
	MPDoorCMDA1Struct       MPDoorCMDA1;//���õ�ַ
	MPDoorCMDA3Struct       MPDoorCMDA3;//����ʱ��
	MPDoorCMDA4Struct       MPDoorCMDA4;//��������״̬
	MPDoorCMDA5Struct       MPDoorCMDA5;//ϵͳ��λ
	    
}MPDoorProParaStruct;
// ��˾�Ž�Э��ṹ(#80FFFF02111A00008A)
typedef   struct
{	u8		Head;		  //Э��ͷ
	u8		VerInfor;	//������Ϣ 
	u16	  Addr;		  //��ַ
	u8		Style;		//�豸����
	u8		CMD;		  //����
	u8		CmdID;		//�����ʶ��
	u16	Lenth;		  //��Ϣ���ȵ�λ 				
	MPDoorProParaStruct   Para;					
}MakepowerDoorProStruct; 
/*************ͨ�Ŷ˿�Э����Դ*************************/
//�շ��������ṹ
typedef    union
{	
	u8  	                  DaBuf[MAXPCReLen]; 	
	PostProProStruct     	  PostProDa;   	//�ʵ粿Э��
	MakepowerDoorProStruct  MPDoorProDa;  //�����Ž�Э��
}ProtocolStruct;

typedef   struct
{	
	ProtocolStruct 	ProBuffer; 		//������
	//�˿�˵��
	u8 	    Port_Type;		    		//ͨ�Ŷ˿� =0Ϊ����1,=1Ϊ����2, =10.Ϊ����
	u8 	    Port_BufferNum;			//ʹ�õĻ������� ��ʱ��ͬͨ�Ŷ˿ڻ�������Ҫ����
	//Э��
	u8 	    ResState;			   //��Դʹ��״̬ =0.���� =1.���ڽ������� =2.���ڷ������� =3.׼����η������� =4.һ��Э������������׼�����
	u8      Pro_Status;           //��Դ����״̬
	u8 	    Pro_Type;        	   //Э������    ��˾Э�� �ʵ粿Э�� �Ž�Э��
	u16 		Pro_Lenth;	       //ͳ��Э����ֽڳ���
	u16     Pro_RealityLenth;     //�����峤�ȣ���βЭ��ʹ��
	//ָ��	
	u8 	    *ptr_sending;	        //ָ���ͻ�����

	//�ְ�����	
	u8			SendMode;						//����20�ֽ���ʱ200ms
	u8			SendCount;						//�����ֽڼ���
	u8			PacketCount;					//����
	//��ʱ��
	u16     Delay_ms;
	//
	u8      ReceiveStatus;        //AT����ģʽʹ��
}ProtocolResource;
//�������ݽṹ��  
extern ProtocolResource PortRes[Port_Buffer_Count];
//�ⲿʹ�õ�ָ��
extern ProtocolResource *ptr_PortResP;

/////////////////////////////////////////
void Protocol_timer_open(u8 PortBufferNum,u16 time);
void Protocol_timer_close(u8 PortBufferNum);
void Protocol_TimerDeal(void);
/////////////////////////////////////////
// ��������  : ��ʼ��Э����Դ״̬�����ȡ�������
void Init_protocol(u8 PortBufferNum);
// ��������  : ���ݴ���״̬��ͨ����ʼ��
void Init_protocol_DATA_DEAL(void);
/////////////////////////////////////////
// ��������  : Э��������ݽӿ�
// �������1  : PortNum  ʹ�õĶ˿������µĶ˿ڱ�� ���紮��1�ʹ���2����ʹ��ͬһ��������
// �������2  : chr		���յ����� 
void BackComm0_RX(u8 PortNum,u8 PortBufferNum,u8 data);
///////////////////////////////////////////
// ��������  : ͨ��0��Э�鷢�����ݽӿ�
// �������  : ComNum.//�˿ں�
void BackComm0_TX(u8 ComNum,u8 PortBufferNum);
//////////////////////////////////////////////
// ��������  : ��λ���ͻ�����
void ResetBuffer(void);
// ��������  : ��Ҫ���͵��ֽ����뷢�ͻ�����
// �������  : TempChar ��������.
void SendByte(u8 TempChar);
// ��������  : ����n���ֽ�
// �������  : pchar �������� count ����.
void SendHexBytes(u8 *pchar,u8 count);
// ��������  : ����һ���ֽڣ�һ���ֽ���ֽ������ASCII�뷢�ͣ��ȷ���High 4λ������Low 4λ
// �������  : c1 ��������.
void SendOneHexToTwoAsciiByte(u8 c1);
// ��������  : �������ַ���
void Send_Word(u16 TemInt);
// ��������  : ����n���ֽڣ���2*n��ASCII��
// �������  : pchar �������� count ����.
void SendAsciiBytes(u8 *pchar,u8 count);
// ��������  : ����һ��Float������
void SendOCEDataFloat(float ocedt);
// ��������  : ��ʼ���ͻ���������
void StartSend(void);
// ��������  : ��ʼ���ͻ���������
void TimerDelayStartSend(u8 ComNum,u8 PortBufferNum);
// ��������  : ��ʼ���ͻ���������
void TimerDelaySendReply(u8 ComNum,u8 PortBufferNum);

// ��������  : Э�鴦��
void DealProtocol(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
