#ifndef __CARDLibrary_H
#define __CARDLibrary_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
#include "globe.h"
	
#if ReadCard_Open

typedef __packed struct
{
	u8 	 TidyEnable;
}CardLibraryStruct;
extern CardLibraryStruct CardLibrary;

#define CardLibraryParaHashInit 29

//����������������������������������������������������������������������������������
typedef __packed struct  _CardListPos_
{
	u8	 Hash;						//Hash����
  u8   SectorIndex;     //��������
  u8   NextSector;      //�¸���������
	u8	 CardCount;			  //������������
  u8   Pos;             //����λ��
}CardList_Pos;
//������������������������������������ķ������������������������������������������������
#define FLASH_CardRecord_Size   		32     	//����FLASH�п���Ϣ�ṹ���С
#define	FLASH_CardSector_Size				4096   	//FLASH������С
#define FLASH_CardRecord_StartAddr 	0 			//�������ʼ��ַ
//�����洢����Ϣ����	   4096/32=128
#define Card_SectorRecordCount			(FLASH_CardSector_Size/FLASH_CardRecord_Size) 

#define EEP_CardRecord_Size 				2      			//����EEP�п�������¼�Ĵ�С
#define EEP_CardRecord_StartAddr		(2*1024) 		//�������ʼ��ַ 0x0860
#define EEP_CardRecord_EndAddr  		(32*1024) 	//����Ľ�����ַ 

#define EEP_CardRecord2_StartAddr		((32+2)*1024) 		//�������ʼ��ַ 0x0860
#define EEP_CardRecord2_EndAddr  		((32+32)*1024) 	  //����Ľ�����ַ 

#define EEP_CardRecord3_StartAddr		((32*2+2)*1024) 	//�������ʼ��ַ 0x0860
#define EEP_CardRecord3_EndAddr  		((32*2+32)*1024) 	//����Ľ�����ַ 

#define EEP_CardRecord4_StartAddr		((32*3+2)*1024) 	//�������ʼ��ַ 0x0860
#define EEP_CardRecord4_EndAddr  		((32*3+32)*1024) 	//����Ľ�����ַ 
//EEP������С (��Ϊ����)	 128*2+7+1=264
#define	EEP_CardSector_Size				(Card_SectorRecordCount*EEP_CardRecord_Size+8) 
//�洢���Ĵ�С	32*1024- 2*1024=30720;
#define EEP_CardStorage_Size     	(EEP_CardRecord_EndAddr-EEP_CardRecord_StartAddr) 
//��������	 30720/264=116
#define EEP_CardSector_Count    	(EEP_CardStorage_Size/EEP_CardSector_Size)
//��ʵ��С  116*264=30624
#define EEP_CardStorageReal_Size  (EEP_CardSector_Count*EEP_CardSector_Size)
//#define CardLibraryParaHashInit 29 Hash=29
//������ 	128*116=14848
//����	 	128*(116-Hash+1)=11264
//128*(116-Hash+1)>=10000 -> Hash<=117-78.125=38.875
 
//flash����Ľ�����ַ 0+ 116*4K= 464K    116*4096
#define FLASH_CardRecord_EndAddr 	    (FLASH_CardRecord_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //����Ľ�����ַ

#define FLASH_CardRecord2_StartAddr 	(512*1024) 			//�������ʼ��ַ
#define FLASH_CardRecord2_EndAddr 		(FLASH_CardRecord2_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //����Ľ�����ַ

#define FLASH_CardRecord3_StartAddr 	(512*2*1024) 			//�������ʼ��ַ
#define FLASH_CardRecord3_EndAddr 		(FLASH_CardRecord2_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //����Ľ�����ַ

#define FLASH_CardRecord4_StartAddr 	(512*3*1024) 			//�������ʼ��ַ
#define FLASH_CardRecord4_EndAddr 		(FLASH_CardRecord2_StartAddr+EEP_CardSector_Count*FLASH_CardSector_Size) //����Ľ�����ַ


//����������=����������-Hash����+1��*�����洢����=��116-29+1��*128=11264
//������������=��������*�����洢����=14848
/**********************************************EEP������������**************************************/ 
typedef __packed struct 
{//�ڿռ���ռ��7���ֽ�
	u8    UpSector;				//�ϸ�����
	u8    NextSector;			//�¸�����
	u8    CardCount;			//����¼����
	u8    FLASHCardCount;	//FLASH�п��ļ�¼����
	u8		EmptyByte[3];		//���� ����3���ֽ�
}EEP_CardSectorManageStruct;                                 
typedef __packed struct 
{  	
	u8     State;	      //��״̬
	u8     FlashPos;	  //��flashλ��			
}EEP_CardRecordStruct;   
typedef __packed struct 
{	 
	EEP_CardSectorManageStruct	Manage;//����������Ϣ
	EEP_CardRecordStruct      	CardRecord[Card_SectorRecordCount];//һ�������洢�Ŀ���Ϣ		
}EEP_CardSectorStruct;                                 
/**********************************************�����ṹ��**************************************/
#define Card_Size       4  	    //���ŵĴ�С

typedef __packed struct 
{  		 	 
	u8     CardNo[Card_Size];			//0 4���ֽڵĿ���
	u32    CardUsefulLife;       	//4 ��Ч��Ϊʱ�䣺4���ֽ�
	u8     CardType;              //8 ������ =0��Ȩ�� =1�û��� =2Ѳ���� =3����
	u8     CardState;             //9 ��״̬ =0��Ч�� =1��ʧ�� =2���Ῠ
	u8     LimitEffective;		    /*10 Ȩ����Ч��˵��: 00000000
																			λ0: ʱ���Ƿ���Ч,
																			λ1: �ڼ����Ƿ���Ч,
																			λ2: ��Ч�����Ƿ���Ч,
																			λ3: �������Ƿ���Ч,
																			λ4-6: ����,
                            	        λ7:��ΪȨ�������б�==1Ϊ������TimeSegSelectֵΪ�����(��10��)��
                             	                           ==0Ϊ�Զ��壬��TimeSegSelectֵΪʱ��
                                 */
	u16    TimeSegSelect;		    	 //11 ʱ��ѡ��ͨ��λ��ѡ����Ӧ��ʱ�Σ���16λ,ÿ�������ѡ��3��ʱ�Σ�4��7
	u8     CardToDoorState;        //13 ���Ŷ���״̬����λ����ʾ�����֧��8����
	u8     CardToPCDAntState;      //14 ���ŶԶ���ͷ״̬����λ����ʾ�����֧��8������ͷ
	u8     CardUseID[4];    			 //15 �û����
	u8     CardUserKey[2];	       //19 ����
	u8     WhichProtocol;          //21 ���ĸ�Э������
	u8     UserVIP	;							 //22 Ȩ��
	u8     No_Use[8];							 //30 δ���ֽڣ����� 			
}CardList_Record;//31�ֽ�


//////////////////////��ʼ��///////////////////////////////////
//��������: �ϵ� ��ʼ������
//��ڲ�����	CardLibraryParaEepAddr	�������Eep��ַ
//��ڲ���	: CardLibraryParaPointer �������
extern void  CardLibrary_Init(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer);
// ��������: �������
//��ڲ�����	CardLibraryParaEepAddr	�������Eep��ַ
//��ڲ���	: CardLibraryParaPointer �������
extern void  CardLibrary_Clear(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer);
//////////////////////��ʼ��end///////////////////////////////////

////////////////////////////������Ӻ�ɾ��//////////////////////////////////////
// ��������  : �����ѯ
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 : pCardNo ����
// �������2 : ����pEEP_Sector ����EEP������Ϣ
// �������3 : ����pRecord ���ļ�¼
// �������4 : ����Index   ��������
// �������5 : ����pPos    ��������λ��
// ���ز���  : ���ز�ѯ���
extern u8 CardLibrary_CardSearch(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo,EEP_CardSectorStruct *pEEP_Sector,CardList_Record *pRecord,u8 *pIndex,u8 *pPos);
// ��������  : ��ɾ��
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������  : pCardNo ����
// ���ز���  : ����ɾ�����
extern u8 CardLibrary_DelCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo );
// ��������  : ������
// �������  :	 CardLibraryParaEepAddr	�������Eep��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 : pRecord ���ļ�¼
// ���ز���  : ���ز�ѯ���
extern u8 CardLibrary_AddCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord);
////////////////////////////������Ӻ�ɾ��end////////////////////////////////////

///////////////////////////////������//////////////////////////////
// ��������  : ��λ��ȡ�����¼����
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
extern u8 CardLibrary_ResetReadCardConfig(u32 EEPCardRecordStartAddr);
// ��������  : ����ָ���ĵ�ַ��ȡ ����Ϣ
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������1 : pCardPos ����¼λ�� 
// �������2 : pRecord ����¼ 
extern u8 CardLibrary_GetPosCard(u32 EEPCardRecordStartAddr,CardList_Pos *pCardPos,CardList_Record *pRecord);
// ��������  : ��ȡ����ָ��λ�õĿ���Ϣ
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 :  pRecord ����¼ 
extern u8 CardLibrary_GetNearPosCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord);
// ��������  : ��ǰ��ȡ���� ����curReadIndexPos�ṩ����Ϣ��ȡ
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������  : pRecord ����¼ 
extern u8 CardLibrary_curReadCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord );
/////////////////////////////������end////////////////////////////////
// ��������  : ����һ����ϣֵ�Ŀ��⣬���ղ�ʹ�õ�����������flash�����ķϵĿռ�
// �������  :	 CardLibraryParaEepAddr	�������Eep��ַ
// �������  :	 CardLibraryParaPointer �������
extern u8 CardLibrary_Tidy(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer);

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
