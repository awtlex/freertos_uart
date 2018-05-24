//zhj
//20151028
//v1.0	
	
#include "basicfunc.h"
#include "param.h"	
#include "mp_osal_rtos.h"	


///////////////////BCD HEX ת��////////////////////////////////////////////////
// ��������  : BCDתHex
u8 BCDToHex(u8 TC_char)
{	
  	u8 Hbcd,Lbcd;	
	Hbcd = TC_char>>4;
	Lbcd = TC_char&0x0f;
	TC_char = Hbcd*10+Lbcd;
	return TC_char;
}
// ��������  : BCDתHex
u32 U32BCDToU32Hex(u32 TC_char)
{		
  u32 k,hex=0;
	u8 bcd,i;
	for(i=0,k=1;i<8;i++)
	{
		bcd = TC_char>>4*i;
		bcd = bcd&0x0f;
		hex += bcd*k;
		k = k*10;
	}
	
	return hex;
}
// ��������  : HexתBCD
// hexֵ<100(0x64)
u8 HexToBCD(u8 TC_char)
{	
  u8 Hbcd,Lbcd;
	Hbcd = TC_char/10;
	Hbcd %= 10;
	Lbcd = TC_char%10;
	TC_char = Hbcd*0x10+Lbcd;
	return TC_char;
}
// ��������  : HexתBCD
u16 HexToIntBCD(u8 HexVar)
{	
  u16  BcdVar = 0;
	
	BcdVar|=(HexVar/100)<<8;
	HexVar=HexVar%100;

	BcdVar|=(HexVar/10)<<4;
	BcdVar|=HexVar%10;
 	
 	return BcdVar;
}
// ��������  : 16λHexתBCD
// hexֵ<10000(0x2710)
u16 IntHexToIntBCD(u16 HexVar)
{
	u16  BcdVar=0;
	
	BcdVar|=(HexVar/1000)<<12;
	HexVar=HexVar%1000;

	BcdVar|=(HexVar/100)<<8;
	HexVar=HexVar%100;

	BcdVar|=(HexVar/10)<<4;

	BcdVar|=HexVar%10;

 	return BcdVar;
}
///////////////////////////////////////////////////////////////////////////////
// ��������  : �ֽڵĴ�С�˵�ת��
// �������1 : Count  ��������ݸ���
// �������2 : P      ���������ָ��
void ByteEndian(u16 Count ,u8 *P)
{
	u16 Ta,i = 0;
	Count--;
	for(i=0; i<Count;i++,Count--)
	{
		Ta=P[i];
		P[i]=P[Count];
		P[Count]=Ta;      
	}
}
// ��������  : ������char�ͺϳ�һ��int
u16 TwoCharToInt(u8 c1,u8 c2)
{	
  u16 tempcmd;
	tempcmd=c1;
	tempcmd<<=8;
	tempcmd|=c2;
	return tempcmd;
}
////////////////////hex ascii ��ת��///////////////////////////////////////////
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Hi_HexToAsc(u8 c1)
{	
	c1=c1>>4;	
	return(c1>=0xa? (c1+0x37):(c1+0x30));
}	
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Low_HexToAsc(u8 c1)
{	
	c1=c1&0xf;
	return(c1>=0xa? (c1+0x37):(c1+0x30));
}

//��һ���ֽڵ�����ת��Ϊ����ASCII ��
void CharToAsc(u8 c1,u8 *buf)	
{
	u8 t1,t2;
		
	t1 = Hi_HexToAsc(c1);
	t2 = Low_HexToAsc(c1);	

	*buf 	 = t1;	
	*(buf+1) = t2;
}

//�������ֽڵ�����ת��Ϊ����ASCII ��
void u16ToAsc(u16 c1,u8 *buf)
{
	u8 t1,t2;

	t1 = c1&0x0f;
	t2 = (c1>>8)&0x0f;
	
	CharToAsc(t2,buf);	
	CharToAsc(t1,buf+2);
}

//���ĸ��ֽڵ�����ת��Ϊ����ASCII ��
void u32ToAsc(u32 c1,u8 *buf)	
{	
	u16 t1,t2;

	t1 = c1&0xff;
	t2 = (c1>>16)&0xff;	

	u16ToAsc(t1,buf);
	u16ToAsc(t2,buf+4);	
}
// ��������  : ��ascii ��'0'~'f' ת���� 0~f һ��byte
u8 AscToHex(u8 c1)
{	
	if (c1>=0x30 && c1<=0x39) 		c1=c1-0x30;
	else if (c1>=0x41 && c1<=0x46)	c1=c1-0x37;
	else if (c1>=0x61 && c1<=0x66)	c1=c1-0x57;
	return(c1);
}
// ��������  : ������ASC��ϳ�һ��HEX�ַ�
u8 TwoAscTOHex(u8 TPA,u8 TPB)   
{    
	TPA = AscToHex(TPA);
	TPB = AscToHex(TPB);
	return((TPA<<4)|TPB);
}	



// ��������  : ��ASC�뻺��ת����HEX������
void BufAscToHex(u8 *PTemp,u16 Len)  
{	
  	u8  Ta,*PsouA;
	u16   i;
	PsouA = PTemp;
	Len >>=1;
	for(i=0; i<Len; i++)
	{	
		Ta = *PTemp++;
		*PsouA++ = TwoAscTOHex(Ta,*PTemp++);
	}
}
//////////////////////////////////////////////////////////////////////////////
//����д��ĸת��ΪСд
u8 CapitalTo(u8 cl)
{
	if (cl>=0x41 && cl<=0x5a)
	{
		cl +=0x20;
	}
	return cl;
}
//����д��ĸת��ΪСд
void BufCapitalTo(u8 *p,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		*p = CapitalTo(*p);
		p++;
	}
}
////////////////////У��ͼ���/////////////////////////////////////////////////
// ����У���
u16 BasicFunc_check_sum(u8 *data,u16 len)
{
  u16 i,sum = 0;
  for(i=0;i<len;i++)
  {
    sum += *data;
    data++;
  }
  
  return sum;
}
//ͨ�õ�CRC16λУ����㷨
//Quantic ����ʽ0xA001,
//intData ��ʼֵ��0xFFFF
//prt			����ָ��
//Lenth		���ݳ���
u16 GeneralCRCFun(u16 Quantic,u16 intData,u8 *ptr,int Lenth)
{
	int  i,j;
	u8 byteData;
	for(i=0;i<Lenth;i++)
	{
		intData ^=*ptr++;
		for(j=0;j<8;j++)
		{
			byteData = intData & 0x0001;
			intData = intData >> 1;
			if(byteData == 1)
			{
				intData ^=Quantic;
			}
		}
	}
	return intData;
}
//����ͼ���
u8 ModBusCRC16ChkSum(u8 Ta,u16 Quantic,u16 CRCInit,u8 *ptr,int Lenth)
{	
	u16  Crc,Crcsum;
	Crc = GeneralCRCFun(Quantic,CRCInit,ptr,Lenth-2);
	if(Ta==0)   //==0 CRC У��͸ߵ�����
	{	
		Crcsum = (u16 )ptr[Lenth-1];
		Crcsum <<=8;
		Crcsum += (u16 )ptr[Lenth-2];
	}
	else    	//==1 CRCУ����Ƿ�͸�����
	{	
		Crcsum = (u16 )ptr[Lenth-2];
		Crcsum <<=8;
		Crcsum += (u16 )ptr[Lenth-1];
	}
	if(Crc==Crcsum)
	{		
		return  TRUE;	
	}
	return FALSE;	
}

#if 0

////////////////////////////////////////////////////////////
//��ȡ�����
u16 BasicFunc_GetRadom(void)
{
	static u16 RandomData=0;
	srand(RandomTime+RandomData);
	RandomData = rand();
	return RandomData;
}

	

/////////////////////////////////////////////////////////////
/***************************************************************************************
  ���ܣ�ʱ�����ʽ ��ʱ��ת����һ����������
  ������*pdate----ָ�������յĽṹ��*ptime---ָ��ʱ��Ľṹ
  ˵����ʱ�����ָ��������ʱ��1970��01��01��00ʱ00��00��(����ʱ��1970��01��01��08ʱ00��00��)�������ڵ�������
*****************************************************************************************/
#define Timestamp_Base_Year					2000
#define Timestamp_Base_Year_data 		946656000 //����ʱ�� 2000��1��1��0ʱ0��0���ʱ�����ֵ
#define BeijingTime									1

//�ж��Ƿ�Ϊ���꣬�����2����29��;������Ϊ28��
u8 JustYear(u16 year,u8 *pMonth)
{   
	u8 leap;	 
	if(((year%4==0)&&(year%100!=0))||(year%400==0))
	{
		*pMonth=29;
		leap=1;
	}
	else
	{
		*pMonth=28;
		leap=0;
	}
	return leap;
}



//�������ĵڼ���
u16 ComputeDays(Date_Time *pDate)
{   
	u8 months[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	u16 DayCount=0,i;
    
	if(pDate->StuDate.Month <= 12)
	{
		JustYear(pDate->StuDate.Year,&months[1]);
		
		for(i=1;i<pDate->StuDate.Month;i++)		//�����·�֮ǰ������
		{       
			DayCount +=months[i-1];
		}
		DayCount+=pDate->StuDate.Day;         //�������ǰҪת��������������ļ���
	}
	else
	{
		return 0;
	}
	return (DayCount-1);             
}



//����ʱ���
u32 TimeTransform32bit(Date_Time *date)
{
	u32 DateTime = 0;
	u32 days = 0;
	u16 years = 0;
	u8  M;        
	u16 i;
	
	years = date->StuDate.Year;
	for(i=0;i<years;i++)
	{
		if(JustYear(Timestamp_Base_Year+i,&M))
		{
			days+=366;   
		}
		else  
		{
			days+=365; 
		}
	}
	days += ComputeDays(date);    //����û��һ�������������������������

	DateTime = days*86400;
	DateTime += (u32)(((u32)date->StuDate.Hour)*3600+(u32)(date->StuDate.Minute)*60+date->StuDate.Second);
	#if BeijingTime
	DateTime += Timestamp_Base_Year_data;
	#else
	DateTime += Timestamp_Base_Year_data + 8*3600;//��������ʱ���8��Сʱ
	#endif

	return  DateTime;
}

#endif 


