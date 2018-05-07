//zhj
//20170712
//F2.1V1.0

#include "globe.h"
#include "cardlibrary.h"
#include "config.h"

#if ReadCard_Open

CardLibraryStruct CardLibrary;

CardList_Pos    curReadIndexPos = {0};    	//�������ʱ��ʹ��:��ǰ����ȡ��λ��= {0}

//��������: ���ݿ������Eep��ַ��ÿ�����ʼ��ַ
//��ڲ�����	CardLibraryParaEepAddr	�������Eep��ַ
//���ز�����	eeprom�Ŀ�����ʼ��ַ
u32 CardLibrary_GetEEPCardRecordStartAddr(u32 CardLibraryParaEepAddr)
{
	u32 addr;
	if(CardLibraryParaEepAddr == CardLibraryPara_EepAddr)
		addr = EEP_CardRecord_StartAddr;
	if(CardLibraryParaEepAddr == CardLibraryPara2_EepAddr)
		addr = EEP_CardRecord2_StartAddr;
	if(CardLibraryParaEepAddr == CardLibraryPara3_EepAddr)
		addr = EEP_CardRecord3_StartAddr;
	if(CardLibraryParaEepAddr == CardLibraryPara4_EepAddr)
		addr = EEP_CardRecord4_StartAddr;
	return addr;
}
//��������: ���ݿ������Eep��ַ���flash�Ŀ�����ʼ��ַ
//��ڲ�����	CardLibraryParaEepAddr	�������Eep��ַ
//���ز�����flash�Ŀ�����ʼ��ַ 
u32 CardLibrary_GetFlashCardRecordStartAddr(u32 CardLibraryParaEepAddr)
{
	u32 ADDR;
	if(CardLibraryParaEepAddr == CardLibraryPara_EepAddr) 
		ADDR = FLASH_CardRecord_StartAddr;
  if(CardLibraryParaEepAddr == CardLibraryPara2_EepAddr) 
		ADDR = FLASH_CardRecord2_StartAddr;
  if(CardLibraryParaEepAddr == CardLibraryPara3_EepAddr) 
		ADDR = FLASH_CardRecord3_StartAddr;
  if(CardLibraryParaEepAddr == CardLibraryPara4_EepAddr) 
		ADDR = FLASH_CardRecord4_StartAddr;
	return ADDR;
}
//��eeprom�Ŀ�����ʼ��ַ�õ�flash�Ŀ�����ʼ��ַ
//���� EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
//���� flash�Ŀ�����ʼ��ַ 
u32 CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(u32 EEPCardRecordStartAddr)
{
	u32 ADDR;
	if(EEPCardRecordStartAddr == EEP_CardRecord_StartAddr) 
		ADDR = FLASH_CardRecord_StartAddr;
	if(EEPCardRecordStartAddr == EEP_CardRecord2_StartAddr) 
		ADDR = FLASH_CardRecord2_StartAddr;
	if(EEPCardRecordStartAddr == EEP_CardRecord3_StartAddr) 
		ADDR = FLASH_CardRecord3_StartAddr;
	if(EEPCardRecordStartAddr == EEP_CardRecord4_StartAddr) 
		ADDR = FLASH_CardRecord4_StartAddr;
	return ADDR;
}

//////////////////////��ʼ��///////////////////////////////////
//��������: ����EEP������Ϣ ��ʼ������
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
//��ڲ���	: SectorIndex ����
//��ڲ���	: pEEP_Sector	������Ϣ
//���ز���	: ���س�ʼ�����
u8 CardLibrary_SectorInit(u32 CardLibraryParaEepAddr,u8 SectorIndex,EEP_CardSectorStruct *pEEP_Sector)
{
   u32 ADDR;
   u32 EEPCardRecordStartAddr;
   EEPCardRecordStartAddr=CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);
   ADDR=EEPCardRecordStartAddr+(SectorIndex*EEP_CardSector_Size);//��ȡEEP������ַ
   WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)pEEP_Sector);
  
	 ADDR=CardLibrary_GetFlashCardRecordStartAddr(CardLibraryParaEepAddr);
   ADDR=ADDR+SectorIndex*FLASH_CardSector_Size; //��flash�е�������ַ
   SST_Earse_Area(ADDR);//����flash����
   return Right;
}
//��������	: ��ʼ������ 
//��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
//��ڲ���	: CardLibraryParaPointer �������
//���ز���	: ���س�ʼ�����
u8 CardLibrary_LibraryInit(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{
   u8 i,Ta;       
   EEP_CardSectorStruct EEP_Sector={0};

   EEP_Sector.Manage.UpSector=0xFF;
   EEP_Sector.Manage.NextSector=0xFF;
   EEP_Sector.Manage.CardCount=0;
   EEP_Sector.Manage.FLASHCardCount=0; 
   
   for(i=0;i<CardLibraryParaPointer->Hash;i++)
   {//hash������ʼ��
	   CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
   } 
   //���ÿ�ʼ������ʼ��    
   EEP_Sector.Manage.UpSector=0xFF;
   EEP_Sector.Manage.NextSector=i+1;//i=29
   EEP_Sector.Manage.CardCount=0;
   EEP_Sector.Manage.FLASHCardCount=0; 
   CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
	 i++;
	 //����������ʼ��
   Ta=EEP_CardSector_Count-1;
   for(;i<Ta;i++)//i=30
   {
			EEP_Sector.Manage.UpSector=i-1;
      EEP_Sector.Manage.NextSector=i+1; 
			CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
   }
   //�������������ʼ��
   EEP_Sector.Manage.UpSector=i-1;
   EEP_Sector.Manage.NextSector=0xFF; 
   CardLibrary_SectorInit(CardLibraryParaEepAddr,i,&EEP_Sector);
   return Right;
}
//��������: �ϵ� ��ʼ������
//��ڲ�����	CardLibraryParaEepAddr	�������Eep��ַ
//��ڲ���	: CardLibraryParaPointer �������
void CardLibrary_Init(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{	
  if(ReadEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash)!=Right)
	{//����û�г�ʼ��
		CardLibrary_Clear(CardLibraryParaEepAddr,CardLibraryParaPointer);
	}
	else if(CardLibraryParaPointer->Hash != CardLibraryParaHashInit)//�������ԣ���ʼ������
	{
		CardLibrary_Clear(CardLibraryParaEepAddr,CardLibraryParaPointer);
	}
}
//��������: �������
//��ڲ�����	CardLibraryParaEepAddr	�������Eep��ַ
//��ڲ���	: CardLibraryParaPointer �������
void CardLibrary_Clear(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{	
	CardLibraryParaPointer->Hash= CardLibraryParaHashInit;
	CardLibraryParaPointer->CardCount=0;//������
	CardLibraryParaPointer->SectorCount=(u8)(EEP_CardSector_Count); //��������
	CardLibraryParaPointer->UsableListCount=(u8)(EEP_CardSector_Count)-CardLibraryParaPointer->Hash;//δ����������
	CardLibraryParaPointer->UsableListNo=CardLibraryParaPointer->Hash;//��������ָ��
	WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash);

	CardLibrary_LibraryInit(CardLibraryParaEepAddr,CardLibraryParaPointer);//��ʼ������
}
//////////////////////��ʼ��end///////////////////////////////////

////////////////////////////������Ӻ�ɾ��///////////////////////////////////////
// ��������  : ���űȽ�
// �������1 : pCardNo1 ��һ�ſ���
// �������2 : pCardNo2 �ڶ��ſ���
// ���ز���  : ���رȽϽ�� 1 С�� 2 ���� 0 ����
u8 CardLibrary_CompareCardNo(u8 *pCardNo1, u8 *pCardNo2)
{
	u8 i = 0;
	
	for(i= 0;i<Card_Size;i++)
	{
		if(pCardNo1[i]<pCardNo2[i])
		{
			return 1;		//С��	
		}
		else if(pCardNo1[i]>pCardNo2[i])
		{
			return 2;		//����
		}
	} 	
	return 0;			    //����	
}
// ��������  : �������ڲ�ѯ
// �������1 : FLASHCardRecordStartAddr ������ʼ��ַ
// �������1 : Index ��������
// �������2 : pCardNo ����
// �������3 : SectorInfo ������������Ϣ
// �������4 : pRecord ���ļ�¼
// �������5 : pPos    �������еĵ�ַ
// ���ز���  : ���ز�ѯ���
u8 CardLibrary_CardSectorSearch(u32 FLASHCardRecordStartAddr,u8 Index,u8 *pCardNo,EEP_CardSectorStruct *pEEP_Sector,CardList_Record *pRecord,u8 *pPos )
{	 
	int16_t Result,Low,High,Mid;  	
	u8 i = 0;
	u32 ADDR;
	CardList_Record Record;

	if((*pEEP_Sector).Manage.CardCount < 1)		//�������
	{		
		*pPos = 0;
		return Error;
	}
	if((*pEEP_Sector).Manage.CardCount>Card_SectorRecordCount)
	{
		(*pEEP_Sector).Manage.CardCount = Card_SectorRecordCount;
	}
	
	Low = 0;
	High = (*pEEP_Sector).Manage.CardCount-1;
  //���ö��ַ����в���
	while(Low<=High)		//�۰����
	{  
 	 	Mid = (Low+High)/2; 
		ADDR=FLASHCardRecordStartAddr+Index*FLASH_CardSector_Size; //��flash�е�������ַ
		ADDR+= (*pEEP_Sector).CardRecord[Mid].FlashPos*FLASH_CardRecord_Size;//��flash�еľ��Ե�ַ
		Read_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&Record.CardNo);
		Result=CardLibrary_CompareCardNo(pCardNo, Record.CardNo);	   		
		if(Result==1)		//�����С��
		{
			High = Mid-1;
		}
		else if(Result==2)	//����Ǵ���
		{
			Low = Mid+1;
		}
		else if(Result==0 )
		{
		 	for(i= 0; i< sizeof(CardList_Record);i++)
				pRecord->CardNo[i] = Record.CardNo[i]; 		 	
		 	*pPos = Mid;
		 	return Right;;
		}
	}
	
	if(Result==1)		//��Ҫ���ڲ��빦��
	{
		*pPos = Mid;
	}
	else		//�������
	{
		*pPos = Mid+1;	
	}
	return ERROR;  	
}
// ��������  : ��ȡ���ŵ�Hash����
// �������  :	 CardLibraryParaPointer �������
// �������  : pCardNo ����ָ��
// ���ز���  : Index	Hash����
u8 CardLibrary_GetHashIndex(CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo)
{	 	
	uni_float ufloat; 
	u8 Index;
	ufloat.byte[3]= pCardNo[0];
	ufloat.byte[2]= pCardNo[1];
	ufloat.byte[1]= pCardNo[2];
	ufloat.byte[0]= pCardNo[3];
	Index=ufloat.uzlong % CardLibraryParaPointer->Hash; 
	return Index;
}
// ��������  : �����ѯ
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 : pCardNo ����
// �������2 : ����pEEP_Sector ����EEP������Ϣ
// �������3 : ����pRecord ���ļ�¼
// �������4 : ����Index   ��������
// �������5 : ����pPos    ��������λ��
// ���ز���  : ���ز�ѯ���
u8 CardLibrary_CardSearch(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo,EEP_CardSectorStruct *pEEP_Sector,CardList_Record *pRecord,u8 *pIndex,u8 *pPos)
{ 			
	u32 ADDR; 	
	
	*pIndex=CardLibrary_GetHashIndex(CardLibraryParaPointer,pCardNo);  	
	while(1)
	{
		ADDR = EEPCardRecordStartAddr+ ((*pIndex)*EEP_CardSector_Size);//��ȡEEP������ַ
		if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&(*pEEP_Sector).Manage.UpSector)==Error)
		{//��ȡУ�����
			return Error;
		} 
		if(((*pEEP_Sector).Manage.CardCount == 0xff)||((*pEEP_Sector).Manage.FLASHCardCount == 0xff))
		{//δ��ʽ��
			return Error;	
		}
		ADDR = CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr);
		if(CardLibrary_CardSectorSearch(ADDR,*pIndex,pCardNo,pEEP_Sector,pRecord,pPos)==Right)
		{//�鵽�� 
			return Right;
		} 		
		(*pIndex)=(*pEEP_Sector).Manage.NextSector;//��һ������
		if( (*pIndex)== 0xff)
		{//û���¸�����
		  return Error;
		}  		
	}	
} 
// ��������  : ��ɾ��
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������  : pCardNo ����
// ���ز���  : ����ɾ�����
u8 CardLibrary_DelCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 *pCardNo )
{	
	u8  i,Index,Pos;	
	u32 ADDR;
	CardList_Record Record;//����¼
	EEP_CardSectorStruct EEP_Sector;//EEP�п�����

	if(CardLibrary_CardSearch(EEPCardRecordStartAddr,CardLibraryParaPointer,pCardNo,&EEP_Sector,&Record,&Index,&Pos)==Right)
	{  
		for(i=Pos+1;i<EEP_Sector.Manage.CardCount;i++)
		{
      EEP_Sector.CardRecord[i-1]=EEP_Sector.CardRecord[i];
		}
		EEP_Sector.Manage.CardCount--;//������������һ
		ADDR=EEPCardRecordStartAddr+(Index*EEP_CardSector_Size);//��ȡEEP������ַ
		WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);

		CardLibraryParaPointer->CardCount--;//�����ܿ�����һ
    switch(EEPCardRecordStartAddr)
    {
      case EEP_CardRecord_StartAddr:
        SysData.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
        SetTimer(Door_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
        break;
      case EEP_CardRecord2_StartAddr:
        SysData2.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
        SetTimer(Door2_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
        break;
      case EEP_CardRecord3_StartAddr:
        SysData3.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
        SetTimer(Door3_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
        break;
      case EEP_CardRecord4_StartAddr:
        SysData4.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
        SetTimer(Door4_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
        break;
    }
	}
	else 
  {
    return Error;
  }
	return Right;		
}
// ��������  : ��������
// �������  :	 CardLibraryParaEepAddr	�������Eep��ַ
// �������  :	 CardLibraryParaPointer �������
// �������  : UpIndex �ϸ���������
// ���ز���  : SectorIndex	 �������������
u8 CardLibrary_ApplySector(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer,u8 UpIndex)
{
	u8 SectorIndex;
	u32 ADDR,EEPCardRecordStartAddr;
	EEP_CardSectorStruct EEP_Sector;

	SectorIndex=CardLibraryParaPointer->UsableListNo; //��������
	if(SectorIndex!=0xFF)
	{//�б�������
		EEPCardRecordStartAddr = CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);
	  ADDR=EEPCardRecordStartAddr+(SectorIndex*EEP_CardSector_Size);//��ȡ����EEP������ַ
	  ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);
		CardLibraryParaPointer->UsableListCount--;
		CardLibraryParaPointer->UsableListNo=EEP_Sector.Manage.NextSector;//�¸���������д�뱸������ָ��
		WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash); 
		EEP_Sector.Manage.UpSector=UpIndex;//�ϸ�������
		EEP_Sector.Manage.NextSector=0xFF;//�¸�������
		EEP_Sector.Manage.CardCount=0;//  ������
		EEP_Sector.Manage.FLASHCardCount=0;//�ռ�ʹ�õĸ���	
		WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 		
	}	
	return SectorIndex;		
}
// ��������  : ������
// �������  :	 CardLibraryParaEepAddr	�������Eep��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 : pRecord ���ļ�¼
// ���ز���  : ���ز�ѯ���
u8 CardLibrary_AddCard(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord)
{	
	u8 i,j,Pos,Index,AddCard=0;
	u32 ADDR,fADDR,EEPCardRecordStartAddr,FLASHCardRecordStartAddr;
	CardList_Record Record;
	EEP_CardSectorStruct EEP_Sector;//EEP�п�����
	
	EEPCardRecordStartAddr = CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);
	FLASHCardRecordStartAddr = CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr);
	if(CardLibrary_CardSearch(EEPCardRecordStartAddr,CardLibraryParaPointer,(*pRecord).CardNo,&EEP_Sector,&Record,&Index,&Pos )==Right)//��ѯ��
	{//�����Ѿ��������
		for(i=0;i<sizeof(CardList_Record);i++)
		{
			if(Record.CardNo[i]!= (*pRecord).CardNo[i])
			{//����Ϣ��ͬ������Ҫɾ���˿�
				for(j=Pos+1;j<EEP_Sector.Manage.CardCount;j++)
				{
				   EEP_Sector.CardRecord[j-1]=EEP_Sector.CardRecord[j];
				}
				EEP_Sector.Manage.CardCount--;
				CardLibraryParaPointer->CardCount--;
				ADDR=EEPCardRecordStartAddr+(Index*EEP_CardSector_Size);//��ȡEEP������ַ
				WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);
				AddCard=1;//��Ҫ���ӿ�					
				break;
			}
		}
	}
	else
	{
		AddCard=1;//��Ҫ���ӿ�
	}

	if(AddCard==0)
	{//����Ϣ��ͬ�������� ֱ�ӷ��ؽ��
		 return Right;
	}
	else
	{		  		
		Index=CardLibrary_GetHashIndex(CardLibraryParaPointer,(*pRecord).CardNo);			
		while(1)
		{
			ADDR=EEPCardRecordStartAddr+(Index*EEP_CardSector_Size);//��ȡEEP������ַ
			if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector)==Error)
			{//��ȡУ�����
				return Error;
			}
			if(EEP_Sector.Manage.FLASHCardCount<Card_SectorRecordCount)
			{//�˿�����δ��
				CardLibrary_CardSectorSearch(FLASHCardRecordStartAddr,Index,(*pRecord).CardNo,&EEP_Sector,&Record,&Pos);//���Ҳ����λ�� 
				for(i=EEP_Sector.Manage.CardCount;i>Pos;i--)
				{//�ȴ˿��Ŵ�� �����ƶ�
				   EEP_Sector.CardRecord[i]=EEP_Sector.CardRecord[i-1];
				}
				EEP_Sector.Manage.CardCount++;//������Ч������
				EEP_Sector.CardRecord[Pos].FlashPos=EEP_Sector.Manage.FLASHCardCount;//�����flash������λ��
				EEP_Sector.Manage.FLASHCardCount++;//flash�����п��ռ�ʹ�ø���  				
				WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);//д��EEP
				
				fADDR=FLASHCardRecordStartAddr+Index*FLASH_CardSector_Size; //��flash�е�������ַ
				fADDR+= (EEP_Sector.Manage.FLASHCardCount-1)*FLASH_CardRecord_Size;//��flash�еľ��Ե�ַ
				Write_SST_Cfg(fADDR,sizeof(CardList_Record),(u8 *)pRecord);//����Ϣ�洢��flash��
				CardLibraryParaPointer->CardCount++;
        switch(EEPCardRecordStartAddr)
        {
          case EEP_CardRecord_StartAddr:
            SysData.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
            SetTimer(Door_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
            break;
          case EEP_CardRecord2_StartAddr:
            SysData2.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
            SetTimer(Door2_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
            break;
          case EEP_CardRecord3_StartAddr:
            SysData3.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
            SetTimer(Door3_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
            break;
          case EEP_CardRecord4_StartAddr:
            SysData4.SysParamChange|=SysParam_CardLibrary;  //ϵͳ���������б仯
            SetTimer(Door4_MemoryTimer,Door_MemoryTime); //�����Ӻ� �����¼�仯
            break;
        }
				return Right;
			}			
			if(EEP_Sector.Manage.NextSector!= 0xff)
			{//�����¸���������
			   Index=EEP_Sector.Manage.NextSector;
			}
			else
			{//û���¸����� ��Ҫ�����¸�����				 
				 Index=CardLibrary_ApplySector(CardLibraryParaEepAddr,CardLibraryParaPointer,Index); 
				 if(Index==0xFF)
				 {//��������ʧ�� �������Ѿ�ʹ����
					 return Error;
				 }
				 EEP_Sector.Manage.NextSector=Index;//������¸�����
				 WriteEepromAndChecksum(ADDR,sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);//д��EEP					
			}  		
		}		
	} 	
} 
////////////////////////////������Ӻ�ɾ��end////////////////////////////////////

///////////////////////////////������//////////////////////////////
// ��������  : ��ȡ�����Ĺ�����Ϣ
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  : SectorIndex��������
// �������  : SectorManage�����Ĺ�����Ϣ
// ���ز���  : ��ȡ�Ľ��
u8 CardLibrary_GetSectorManage(u32 EEPCardRecordStartAddr,u8 SectorIndex,EEP_CardSectorManageStruct *SectorManage)
{	 
	u32 ADDR=0;	  	
	EEP_CardSectorStruct EEP_Sector;//EEP�п����� 
	
	ADDR  =EEPCardRecordStartAddr+ (SectorIndex*EEP_CardSector_Size);//��ȡEEP������ַ
	if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector)==Right)
	{ 	
		if(EEP_Sector.Manage.CardCount>Card_SectorRecordCount)
		{//���������
		  return Error;
		}
		else
		{
			SectorManage->UpSector=EEP_Sector.Manage.UpSector;
			SectorManage->NextSector=EEP_Sector.Manage.NextSector;
			SectorManage->CardCount=EEP_Sector.Manage.CardCount;
			SectorManage->FLASHCardCount=EEP_Sector.Manage.FLASHCardCount;
			return Right;
		} 		
	} 	
	return	Error;		
}
// ��������  : ��λ��ȡ�����¼����
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
u8 CardLibrary_ResetReadCardConfig(u32 EEPCardRecordStartAddr)
{
  EEP_CardSectorManageStruct SectorManage;
	 
	curReadIndexPos.Hash=0;
	curReadIndexPos.SectorIndex = 0;
	curReadIndexPos.Pos = 0; 
	curReadIndexPos.CardCount=0	;
	curReadIndexPos.NextSector=0xFF;     
	if(CardLibrary_GetSectorManage(EEPCardRecordStartAddr,0,&SectorManage)==Right)
	{
	   curReadIndexPos.CardCount= SectorManage.CardCount; //����������
	   curReadIndexPos.NextSector= SectorManage.NextSector;		
	}    
  return Right; 
}
// ��������  : ����ָ���ĵ�ַ��ȡ ����Ϣ
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 : pCardPos ����¼λ�� 
// �������2 : pRecord ����¼ 
// ���ز���  : ��ȡ��״̬
u8 CardLibrary_GetPosCard(u32 EEPCardRecordStartAddr,CardList_Pos *pCardPos,CardList_Record *pRecord)
{		
	u32 ADDR=0;
	EEP_CardSectorStruct EEP_Sector;

	ADDR = EEPCardRecordStartAddr + ((*pCardPos).SectorIndex*EEP_CardSector_Size);//��ȡEEP������ַ
	if(ReadEepromAndChecksum(ADDR, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector)==Right)
	{ 	
		ADDR=CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr)+((*pCardPos).SectorIndex*FLASH_CardSector_Size);
		ADDR+=EEP_Sector.CardRecord[(*pCardPos).Pos].FlashPos *FLASH_CardRecord_Size;
		Read_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&(*pRecord).CardNo);//��ȡflash�Ŀ���Ϣ
		return	Right;
	} 	
	return Error;	
}
// ��������  : ��ȡ����ָ��λ�õĿ���Ϣ
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������1 :  pRecord ����¼
u8 CardLibrary_GetNearPosCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord)
{     
	EEP_CardSectorManageStruct SectorManage;
	while(curReadIndexPos.Hash<CardLibraryParaPointer->Hash)
	{
		if(curReadIndexPos.Pos<curReadIndexPos.CardCount)//��ǰ����ȡ�����
		{
		   CardLibrary_GetPosCard(EEPCardRecordStartAddr,&curReadIndexPos,pRecord);//��ȡ����Ϣ
		   return Right;	 		  
		}
		else
		{	   
		   curReadIndexPos.Pos=0; 
		   while(curReadIndexPos.NextSector !=0xFF)
		   {//��ȡ�¸����ӿ�������Ϣ					   			
					if(CardLibrary_GetSectorManage(EEPCardRecordStartAddr,curReadIndexPos.NextSector,&SectorManage)==Right)
					{
						 curReadIndexPos.SectorIndex=curReadIndexPos.NextSector;//��ǰ����
						 curReadIndexPos.CardCount= SectorManage.CardCount; 	  //����������
						 curReadIndexPos.NextSector= SectorManage.NextSector;	  //�¸�����
						 if(curReadIndexPos.CardCount>0)
						 {
								CardLibrary_GetPosCard(EEPCardRecordStartAddr,&curReadIndexPos,pRecord);//��ȡ����Ϣ
								return Right;
						 }
					}
					else
					{
						 curReadIndexPos.NextSector= 0xFF;
					}    
		   }  		   	
		}
		curReadIndexPos.Hash++;//�¸�Hashֵ
		curReadIndexPos.SectorIndex=curReadIndexPos.Hash;//��ǰ����
		if(CardLibrary_GetSectorManage(EEPCardRecordStartAddr,curReadIndexPos.SectorIndex,&SectorManage)==Right)
		{		   
		   curReadIndexPos.CardCount= SectorManage.CardCount; 	  //����������
		   curReadIndexPos.NextSector= SectorManage.NextSector;	  //�¸�����
		}
		else
		{
		   curReadIndexPos.CardCount= 0; 	  //����������
		   curReadIndexPos.NextSector= 0xFF;	  //�¸�����
		}
	}   
    return Error; 
}
// ��������  : ��ǰ��ȡ���� ����curReadIndexPos�ṩ����Ϣ��ȡ
// ��ڲ���	: EEPCardRecordStartAddr	eeprom�Ŀ�����ʼ��ַ
// �������  :	 CardLibraryParaPointer �������
// �������  : pRecord ����¼ 
u8 CardLibrary_curReadCard(u32 EEPCardRecordStartAddr,CardLibraryParaStruct *CardLibraryParaPointer,CardList_Record *pRecord )
{	      
	if(CardLibraryParaPointer->CardCount > 0)			//����������¼
	{			 
		if(CardLibrary_GetNearPosCard(EEPCardRecordStartAddr,CardLibraryParaPointer,pRecord)==Right)//��ȡ����Ϣ
		{
		 	curReadIndexPos.Pos++;
			return Right;	
		}		
	} 	        		
	return Error;		
}
///////////////////////////������end/////////////////////

///////////////////////////��������////////////////////////////////////////////////
// ��������  : ����һ����ϣֵ�Ŀ��⣬���ղ�ʹ�õ�����������flash�����ķϵĿռ䡣
// �������  :	 CardLibraryParaEepAddr	�������Eep��ַ
// �������  :	 CardLibraryParaPointer �������
u8 CardLibrary_Tidy(u32 CardLibraryParaEepAddr,CardLibraryParaStruct *CardLibraryParaPointer)
{	 
  u8 static HashIndex=0;
  u16 static CardCount=0;
  u8  status=0;
  
	u32 ADDREEp=0,ADDRFlash=0,ADDR=0,EEPCardRecordStartAddr=0;
	u8 i,UpSector,NextSector,Index;	
	EEP_CardSectorStruct 		EEP_Sector;//EEP�п�����
	//CardList_Record RecordG[128];   //һ�������Ŀ���Ϣ 31*128=3968=4096-128=4k-128 Ҫ���ջ����4k������ʹ��ȫ�ֱ��� 
	CardList_Record RecordG[64];   //һ�������Ŀ���Ϣ 31*64
	
	EEPCardRecordStartAddr = CardLibrary_GetEEPCardRecordStartAddr(CardLibraryParaEepAddr);	
  Index=HashIndex;
  while(Index<EEP_CardSector_Count)
  {
    ADDREEp = EEPCardRecordStartAddr + (Index*EEP_CardSector_Size);//��ȡEEP������ַ	
    ReadEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //��ȡEEP�Ŀ�����
    UpSector = EEP_Sector.Manage.UpSector;      //�˿������ϸ�������
    NextSector = EEP_Sector.Manage.NextSector;  //�˿������¸�������
    CardCount += EEP_Sector.Manage.CardCount;   //ͳ�ƿ���Ŀ�����
    if(EEP_Sector.Manage.CardCount==0)//�������Ѿ�û�п� ��Ҫ���
    { 				
      if(Index>=CardLibraryParaPointer->Hash) //��������Ҫ����
      {	
        ADDRFlash = CardLibrary_GetFlashCardRecordStartAddr(CardLibraryParaEepAddr)+(Index*FLASH_CardSector_Size);
        SST_Earse_Area(ADDRFlash);//����flash����
        //���մ�����  ���ڶ��������ұ�����������˱���������UpSectorû��ʵ�����壬���Բ����������е�UpSector
        //EEP������Ϣ
        EEP_Sector.Manage.UpSector=0xFF;
        EEP_Sector.Manage.NextSector=CardLibraryParaPointer->UsableListNo;//������������					
        EEP_Sector.Manage.FLASHCardCount=0;
        WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 
        //������Ϣ
        CardLibraryParaPointer->UsableListNo=Index;
        CardLibraryParaPointer->UsableListCount++;
        WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash);
        //����������ǰ����������������Ϣ								
        ADDREEp=EEPCardRecordStartAddr+ (UpSector*EEP_CardSector_Size);//��ȡ�ϸ�EEP������ַ	
        ReadEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //
        EEP_Sector.Manage.NextSector=NextSector;
        WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //
        //��������������������������Ϣ
        if(NextSector<EEP_CardSector_Count)
        {
          ADDREEp=EEPCardRecordStartAddr+ (NextSector*EEP_CardSector_Size);//��ȡ�ϸ�EEP������ַ	
          ReadEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector);
          EEP_Sector.Manage.UpSector=UpSector;
          WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 
        }				
      }
      else //���flash
      {
        if(EEP_Sector.Manage.FLASHCardCount != 0)
        {
          EEP_Sector.Manage.FLASHCardCount=0;
          WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); 
          ADDRFlash=CardLibrary_GetFlashCardRecordStartAddrFromEEPCardRecordStartAddr(EEPCardRecordStartAddr)+(Index*FLASH_CardSector_Size);
          SST_Earse_Area(ADDRFlash);//����flash����
        }
      }
    }
    else if((EEP_Sector.Manage.FLASHCardCount-EEP_Sector.Manage.CardCount)>(Card_SectorRecordCount/2)
      && EEP_Sector.Manage.CardCount<=64)
    {//��������Ҫ����
      ADDRFlash = CardLibrary_GetFlashCardRecordStartAddr(CardLibraryParaEepAddr)+(Index*FLASH_CardSector_Size);
      for(i=0;i<EEP_Sector.Manage.CardCount;i++)
      {			   	
        ADDR=ADDRFlash+EEP_Sector.CardRecord[i].FlashPos*FLASH_CardRecord_Size;
        EEP_Sector.CardRecord[i].FlashPos=i;
        Read_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&RecordG[i]);//��ȡflash�Ŀ���Ϣ
      }
      SST_Earse_Area(ADDRFlash);//����flash����
      for(i=0;i<EEP_Sector.Manage.CardCount;i++)
      {//д��flash			   	
        ADDR=ADDRFlash+i*FLASH_CardRecord_Size;					
        Write_SST_Cfg(ADDR,sizeof(CardList_Record),(u8 *)&RecordG[i]);//дflash�Ŀ���Ϣ
      }
      EEP_Sector.Manage.FLASHCardCount=i;
      WriteEepromAndChecksum(ADDREEp, sizeof(EEP_CardSectorStruct),(u8 *)&EEP_Sector.Manage.UpSector); //дEEP�Ŀ�����
    }
    if(NextSector==0)
    {	
      NextSector=0xff;
    }
    Index=NextSector;
  }
  HashIndex++;
  if(HashIndex>=CardLibraryParaPointer->Hash)
  {
    if(CardLibraryParaPointer->CardCount!=CardCount) //���⿨����ͳ�ƴ���
    {
      CardLibraryParaPointer->CardCount=CardCount;	
      WriteEepromAndChecksum(CardLibraryParaEepAddr,sizeof(CardLibraryParaStruct),(uchar *)&CardLibraryParaPointer->Hash); 
    } 
    CardCount = 0;  
    HashIndex = 0;
    status = 1;
  }
	return status;	
}

#endif
