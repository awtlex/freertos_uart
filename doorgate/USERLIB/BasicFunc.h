#ifndef	__BASICFUNC_H
#define __BASICFUNC_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
	

// ��������  : BCDתHex
u8 BCDToHex(u8 TC_char);
// ��������  : BCDתHex
u32 U32BCDToU32Hex(u32 TC_char);
// ��������  : HexתBCD
// hexֵ<100(0x64)
u8 HexToBCD(u8 TC_char);
// ��������  : HexתBCD
u16 HexToIntBCD(u8 HexVar);
// ��������  : 16λHexתBCD
// hexֵ<10000(0x2710)
u16 IntHexToIntBCD(u16 HexVar);
////////////////////////////////////////////////////////////////////////////////////
// ��������  : �ֽڵĴ�С�˵�ת��
// �������1 : Count  ��������ݸ���
// �������2 : P      ���������ָ��
void ByteEndian(u16 Count ,u8 *P);
// ��������  : ������char�ͺϳ�һ��int
u16 TwoCharToInt(u8 c1,u8 c2);
////////////////////////////////hex ascii ��ת��/////////////////////////////////////
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Hi_HexToAsc(u8 c1);
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Low_HexToAsc(u8 c1);
//��һ���ֽڵ�����ת��Ϊ����ASCII ��
void CharToAsc(u8 c1,u8 *buf);
//�������ֽڵ�����ת��Ϊ����ASCII ��
void u16ToAsc(u16 c1,u8 *buf);
//���ĸ��ֽڵ�����ת��Ϊ����ASCII ��
void u32ToAsc(u32 c1,u8 *buf);		
// ��������  : ��ascii ��'0'~'f' ת���� 0~f һ��byte
u8 AscToHex(u8 c1);
// ��������  : ������ASC��ϳ�һ��HEX�ַ�
u8 TwoAscTOHex(u8 TPA,u8 TPB);
// ��������  : ��ASC�뻺��ת����HEX������
void  BufAscToHex(u8 *PTemp,u16 Len);
/////////////////////////////////////////////////////////////////////////////////////
//����д��ĸת��ΪСд
u8 CapitalTo(u8 cl);
//����д��ĸת��ΪСд
void BufCapitalTo(u8 *p,u16 len);

//////////////////////////////////У��ͼ���/////////////////////////////////////////
// ����У���
u16 BasicFunc_check_sum(u8 *data,u16 len);
//ͨ�õ�CRC16λУ����㷨
//Quantic ����ʽ0xA001,
//intData ��ʼֵ��0xFFFF
//prt			����ָ��
//Lenth		���ݳ���
u16 GeneralCRCFun(u16 Quantic,u16 intData,u8 *ptr,int Lenth);
//����ͼ���
u8 ModBusCRC16ChkSum(u8 Ta,u16 Quantic,u16 CRCInit,u8 *ptr,int Lenth);
//////////////////////////////////////////////////////////////////////////////////////
u16 BasicFunc_GetRadom(void);
//////////////////////////////////////////////////////////////////////////////////////
//u32 TimeTransform32bit(Date_Time *date);	

#ifdef __cplusplus
}
#endif

#endif
