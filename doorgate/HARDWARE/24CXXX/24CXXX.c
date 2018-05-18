#include <string.h>	
#include "24CXXX.h"
#include "delay.h"
#include "IIC.h"	
#include "led.h"	
//���������_24C1024��ͨ��HardAddr��BIT0=0������=1�������ʸ�64K�͵�64K������BIT1=0��=1�����ʲ�ͬоƬ
//��������������ͨ������HardAddr��ѡ��ͬ��оƬ


/*********************************************************************************************************
** Function name:          EEPROM	   
** Descriptions:          �ʺ���AT24C02~AT24C1024                                                 
** input parameters:        ��
** output parameters:       ��      
** Returned value:          0	 
** Created by:		    �����
    
** Modified date:     2013.8.15     
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
unsigned char HardAddr;
  
extern void _24CXXX_AddrProcess(unsigned long Addr) ;

void _24CXXX_init(void)
{		
	IIC_Init();
	_24CXXX_WP_OUT();
	_24CXXX_WR_DIS();  //wp�������Ϊ��
	HardAddr = 0x0000;
}

const u8 *EEPROM_Test = "24CXXX Test";			
u8 _24CXXX_Buf[15];	 	
		
static u8 _24CXXX_Check(void)
{			
	u8 temp;
	temp=_24CXXX_ReadByte(255);   
	
	if(temp==0X55)
		return 0; 	   
	else
	{	
		_24CXXX_WriteByte(255,0X55);
		temp=_24CXXX_ReadByte(255);	  
		if(temp==0X55)	
			return 0;
	}	
	return 1;											  
}

void _24CXXX_Test(void)		
{		
	u16 len;
	u32 start;	

	len = strlen(EEPROM_Test)	;	
	start = 0;
	
	while(_24CXXX_Check())		
	{			
		delay_xms(500);	
		LED1=!LED1;					
	}
	_24CXXX_WriteBytes(0,(u8*)EEPROM_Test,len);			

	delay_xms(500);	

	_24CXXX_ReadBytes(0,_24CXXX_Buf,len);		

	delay_xms(500);	
}


//��ַ����������Ҫ�����оƬʱ�����ڵ�ַ�Ĵ���
#if DEV_24CXX_MODLE == _24C1024
	#if MAX_ADDR > 0xFFFF	//�����ַ����65535
	void _24CXXX_AddrProcess(unsigned long Addr)
	#else
	void _24CXXX_AddrProcess(unsigned int Addr)
	#endif	//#if MAX_ADDR > 0xFFFF
	{
		if( Addr>( (MEM_SIZE+1)/2-1 ) )	//�����ַ����65535
		{
			Addr /= (MEM_SIZE+1)/2;
		}
		else
		{
			Addr = 0x00;
		}

		HardAddr = Addr;
	}
#else
	#if DEV_24CXX_NUM > 1	//���ʹ�ö��оƬ
		#if MAX_ADDR > 0xFFFF		//�����ַ����65535
		void _24CXXX_AddrProcess(unsigned long Addr)
		#else
		void _24CXXX_AddrProcess(unsigned int Addr)
		#endif	//#if MAX_ADDR > 0xFFFF
		{
			if( Addr> MEM_SIZE )	//�����ַ���ڵ�оƬ��ַ��Χ
			{
				Addr /= MEM_SIZE+1;
			}
			else
			{
				Addr = 0x00;
			}

			HardAddr = Addr;
		}
	#else	//���ʹ�õ���оƬ
		#if MAX_ADDR > 0xFFFF		//�����ַ����65535
		void _24CXXX_AddrProcess(unsigned long Addr)
		#else
		void _24CXXX_AddrProcess(unsigned int Addr)
		#endif	//#if MAX_ADDR > 0xFFFF
		{
			HardAddr = 0x00;
		}
	#endif	//#if DEV_24CXX_NUM > 1
#endif	//#if DEV_24CXX_MODLE==_24C1024


#if MAX_ADDR > 0xFFFF	//�����ַ����65535
void _24CXXX_WriteByte(unsigned long Addr,unsigned char Data)
#else
void _24CXXX_WriteByte(unsigned int Addr,unsigned char Data)
#endif	//#if MAX_ADDR > 0xFFFF
{
	_24CXXX_AddrProcess(Addr);
	Addr &= MEM_SIZE;	//ע�������C1024�����������ݴ���65535������д���ַʱ�ᱻ��λ��ǿ��ת������
	_24CXXX_WR_EN();
	IIC_Start();
	IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE);
	IIC_WriteByte( ((unsigned char)(Addr>>8)) );
	IIC_WriteByte( ((unsigned char)Addr) );
	IIC_WriteByte( Data );
	IIC_Stop();
	_24CXXX_WR_DIS();
}

#if MAX_ADDR > 0xFFFF	//�����ַ����65535
unsigned char _24CXXX_ReadByte(unsigned long Addr)
#else
unsigned char _24CXXX_ReadByte(unsigned int Addr)
#endif	//#if MAX_ADDR > 0xFFFF
{
	unsigned char temp;
	_24CXXX_AddrProcess(Addr);
	Addr &= MEM_SIZE;			
		
	IIC_Start();
	IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE);
	IIC_WriteByte( ((unsigned char)(Addr>>8)) );
	IIC_WriteByte( ((unsigned char)Addr) );
	IIC_Start();
	IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_READ);
	temp = IIC_ReadByte(IIC_SEND_NOACK);
	IIC_Stop();		
	return temp;
}



//---------------------------------------------------------------------------
//�����������ַ������ָ�룬д�����ݸ�����<32768��
//---------------------------------------------------------------------------
#if MAX_ADDR > 0xFFFF	//�����ַ����65535
void _24CXXX_WriteBytes(unsigned long StartAddr,unsigned char *Data,int DataNum)
#else
void _24CXXX_WriteBytes(unsigned int StartAddr,unsigned char *Data,int DataNum)
#endif	//#if MAX_ADDR > 0xFFFF
{
	unsigned char Num;
	unsigned int i;
	unsigned int y;
#if MAX_ADDR > 0xFFFF	//�����ַ����65535
	unsigned long yy;
#else	
	unsigned int yy;
#endif	//#if MAX_ADDR > 0xFFFF

	_24CXXX_AddrProcess(StartAddr);
	yy = StartAddr & MEM_SIZE;
	
	y = yy&PAGE_SIZE_REM;	//=StartAddr/PAGE_SIZE
	y = PAGE_SIZE - y;				//Ҫд�ĵ�һҳ���ж��ٸ�����

	_24CXXX_WR_EN();
	
	if( y <= PAGE_SIZE )	//д��һҳ�����ܲ���һҳ����ʹҳ���룩
	{
		IIC_Start();
		IIC_WriteByte( DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE );
		IIC_WriteByte( ((unsigned char)(yy>>8)) );
		IIC_WriteByte( ((unsigned char)yy) );
		for(i=0;i<y;i++)
		{
			if( i>=DataNum )
			{
				break;
			}
			IIC_WriteByte( *(Data++) );
			
		}
	}
	IIC_Stop();
	delay_ms(10);

	DataNum -= y;		//���ж�������δд
	i = 0;
	Num = 0;			//���ڼ�¼�Ѿ�д�˶���ҳ
	while( DataNum>0 )	//����δд��
	{
		if( i == 0 )
		{
			yy = StartAddr+y+Num*PAGE_SIZE;
			_24CXXX_AddrProcess(yy);
			yy = yy & MEM_SIZE;
			IIC_Start();	
			IIC_WriteByte( DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE );
			IIC_WriteByte( ((unsigned char)(yy>>8)) );
			IIC_WriteByte( ((unsigned char)(yy)) );
		}
		IIC_WriteByte( *(Data++) );
		
		i++;
		if(i>=PAGE_SIZE )	//���һҳд��
		{
			i = 0;
			Num++;
			IIC_Stop();
			delay_ms(10);
		}
		DataNum--;
	}
	if(i!=0x0000)	//���д����ʱ����δ����ҳ���������Ҫ����ֹͣλ
	{
		IIC_Stop();
		delay_ms(10);
	}
	
	_24CXXX_WR_DIS();
}
	


//---------------------------------------------------------------------------
//�����������ַ������ָ�룬�������ݸ�����<32768��
//---------------------------------------------------------------------------
#if MAX_ADDR > 0xFFFF	//�����ַ����65535
void _24CXXX_ReadBytes(unsigned long StartAddr,unsigned char *Data,int DataNum)
#else
void _24CXXX_ReadBytes(unsigned int StartAddr,unsigned char *Data,int DataNum)
#endif	//#if MAX_ADDR > 0xFFFF
{
	unsigned char Num;
	unsigned int i;
	unsigned int y;
#if MAX_ADDR > 0xFFFF	//�����ַ����65535
	unsigned long yy;
#else
	unsigned int yy;
#endif	//#if MAX_ADDR > 0xFFFF

	_24CXXX_AddrProcess(StartAddr);
	yy = StartAddr & MEM_SIZE;

	y = yy&PAGE_SIZE_REM;	//=StartAddr/PAGE_SIZE
	y = PAGE_SIZE - y;				//Ҫд�ĵ�һҳ���ж��ٸ�����

	if( y <= PAGE_SIZE )	//д��һҳ�����ܲ���һҳ����ʹҳ���룩
	{
		IIC_Start();
		IIC_WriteByte( DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_WRITE );
		IIC_WriteByte( ((unsigned char)(StartAddr>>8)) );
		IIC_WriteByte( ((unsigned char)StartAddr) );
		IIC_Start();
		IIC_WriteByte(DEV_24CXX_MAIN_ADDR|(HardAddr<<1)|DEV_24CXX_READ);
		for(i=0;i<y;i++)
		{	
			if( i>=DataNum )
			{
				break;
			}
			if( (i==DataNum-1)||(i==y-1) )	//����Ǹ�ҳ�����һ�����ݣ���NOACK
				*Data = IIC_ReadByte(IIC_SEND_NOACK);
			else
				*Data = IIC_ReadByte(IIC_SEND_ACK);
			Data++;
			
		}
	}
	IIC_Stop();	

}

