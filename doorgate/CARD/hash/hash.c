#include "hash.h"	


//�洢HASH��������
#define FLASH_BASE_FUNC			0								//0x00000000
//�洢�������Ѿ�ʹ�õ��б�ĵ�ַ
#define FLASH_BASE_LIST			FLASH_BASE_FUNC+0x40000			//0x00040000
//�洢������δʹ�õ��б�ĵ�ַ
#define FLASH_BASE_EMPTY_LIST		FLASH_BASE_LIST+0x64000			//0x000A4000
//�洢���ŵ���Ϣ
#define FLASH_BASE_DATA			FLASH_BASE_EMPTY_LIST+0x19000	//0x000BD000
//�洢���ŵ�����
#define CARD_DATA_EDGE				FLASH_BASE_DATA+0x400000		//0x004BD000
//����ѹ����ַ
#define COMPRESS_OFFSET			CARD_DATA_EDGE+0x3000			//0x004C0000


#define CARD_DATA_SUM					51000					//��Ƭ����	
#define CARD_DATA_LEN					80						//��Ƭ��Ϣ������
#define CARD_DATA_SUM_PER_SECTOR		51						//ÿ�������ɴ濨Ƭ����(4096/80)
#define HASH_FUNC_SUM_PER_PACKAGE		256						//1024����4

#define HASH_FUNCTION_SUM				0x10000					//hashֵ��0��0xFFFF
#define HASH_EMPTY_LIST_SECTOR_SUM	25						//51000�ſ�Ƭ��Ҫ25������������ǿ��б�

#define USED_FLAG_TRUE					0								
#define USED_FLAG_FALSE				0xFF
		
#define ADDRESS2SECTOR(addr) (addr&0xFFFFF000)					//��flash��ַת��Ϊ������������ַ






/**
 * \brief   ��list�л�ȡ��Ƭ��Ϣ��ַƫ��
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   list_addr  	list��ַ
 * \param	*data		���ڱ����ȡ�Ŀ�Ƭ��Ϣ
 *
 * \return  none
 **/
static void get_card_info_addr(u16 list_addr,HASH_List *data)
{	
	read_hash_data(FLASH_BASE_LIST+sizeof(HASH_List)*list_addr,sizeof(HASH_List),(u8 *)data);

	LOG_INFO("%s,%d,list_addr:0x%x,N:0x%x,A:0x%x,F:0x%x\r\n",__func__,__LINE__,list_addr,
	data->HashList_Next,data->HashDataAddr,data->HashUsedFlag);
}




static u16 hash_function(u8 *CSN)
{	
	u16 value;		 

	value = (CSN[0]+(CSN[1]*2)+(CSN[2]*3)+(CSN[3]*4)+(CSN[4]*5))<<8;
	value += (CSN[0]*9)+(CSN[1]*8)+(CSN[2]*7)+(CSN[3]*6)+(CSN[4]*5);

	LOG_INFO("%s,%d,CSN:%x-%x-%x-%x-%x,hash:0x%x\r\n",__func__,__LINE__,
		      CSN[0],CSN[1],CSN[2],CSN[3],CSN[4],value);
	
	return value;		
}




/**
 * \brief   ���ļ�ϵͳ�ж�ȡָ������
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   scr	����Դƫ�Ƶ�ַ
 * \param	length	���ݳ���
 * \param	dst	�������ݿռ�
 *
 * \return  none
 **/	
static void  read_hash_data(u32 scr,u32 length,u8* dst)
{	
	//��flash��ȡָ�����ȵ����ݵ�ָ����ַ
	
}




/**
 * \brief   ������д���ļ�ϵͳָ��λ��
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   scr	����Դƫ�Ƶ�ַ
 * \param	length	���ݳ���
 * \param	dst	�������ݿռ�
 *
 * \return  none	
 **/
static void write_hash_data( u32 dst,u8* scr,u32 length)
{
	//��flashдָ�����ȵ����ݵ�ָ����ַ
	
}




/**
 * \brief   ��hash func�л�ȡlist��ַ
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   hash_value hashֵ
 *
 * \return  list��ַƫ��
 **/	
static u16 get_list_addr(u16 hash_value)
{
	HASH_Func data;
	LOG_INFO("%s,%d,hash_value:0x%x\r\n",__func__,__LINE__,hash_value);	
	
	read_hash_data(FLASH_BASE_FUNC+sizeof(HASH_Func)*hash_value,sizeof(HASH_Func),(u8 *)&data);
	
	LOG_INFO("%s,%d,data.UsedFlag:0x%x,data.Func2List:0x%x\r\n",__func__,__LINE__,data.UsedFlag,data.Func2List);
	
	if(USED_FLAG_TRUE == data.UsedFlag)
		return data.Func2List;
	else
		return 0xFFFF;

}


	
/**
 * \brief   ��ȡ���õ�empty list
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   *list		�����ȡ��list��Ϣ
 *
 * \return  listƫ�Ƶ�ַ
 **/
static int get_empty_list(HASH_List *list,u8 CardType)
{
	u16 data,temp;
	int i;
	HASH_FLAG data_edge;
	LOG_INFO("%s,%d\r\n",__func__,__LINE__);

	for(i=0;i<(2*CARD_DATA_SUM);i+=2)
	{
		read_hash_data(FLASH_BASE_EMPTY_LIST+i,2,(void *)&data);
		if(0xFFFF != data)
		{
			LOG_INFO("%s,%d,i:%d,data:0x%x\r\n",__func__,__LINE__,i,data);

			read_hash_data(FLASH_BASE_LIST+data*sizeof(HASH_List),sizeof(HASH_List),(u8 *)list);
			
			memset((void *)&temp,0xFF,2);
			
			write_hash_data(FLASH_BASE_EMPTY_LIST+i,(u8 *)&temp,2);

			return data;
		}			
	}

	//empty list û�п�����Դ,ֱ����data��		
	read_hash_data( CARD_DATA_EDGE,sizeof(HASH_FLAG),(u8 *)&data_edge);
	
	data_edge.card_sum++;
	
	if(TYPE_NORMAL_CARD == CardType)
	{
		data_edge.access_sum++;
	}
	else if(TYPE_PATROL_CARD == CardType)
	{
		data_edge.patrol_sum++;
	}
	else if(TYPE_LIFT_CARD == CardType)
	{
		data_edge.lift_sum++;
	}
		
	write_hash_data( CARD_DATA_EDGE,(u8 *)&data_edge,sizeof(HASH_FLAG));
	list->HashDataAddr = data_edge.card_sum;
	LOG_INFO("%s,%d,card_sum:0x%x\r\n",__func__,__LINE__,data_edge.card_sum);

	return data_edge.card_sum;
}




/**
 * \brief   ��ȡ��Ƭ��Ϣ
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   address		��Ƭƫ�Ƶ�ַ
 * \param   *Card		���ڱ��濨Ƭ��Ϣ
 *
 * \return  none
 **/
static void get_card_id(u16 address,CDINFO *Card)
{	
	u32 sec_num = address/CARD_DATA_SUM_PER_SECTOR;
	u32 sector_n = ADDRESS2SECTOR(FLASH_BASE_DATA+0x1000*sec_num);
	
	read_hash_data((sector_n+((address%CARD_DATA_SUM_PER_SECTOR)*CARD_DATA_LEN)),sizeof(CDINFO),(u8 *)Card);
	LOG_INFO("%s,%d,address:0x%x,data:%x-%x-%x-%x-%x\r\n",__func__,__LINE__,address,
			(u8 *)Card->CSN[0],(u8 *)Card->CSN[1],(u8 *)Card->CSN[2],(u8 *)Card->CSN[3],(u8 *)Card->CSN[4]);
}






/**
 * \brief   ���ҿ�Ƭ
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ�� 
 * \param   *CSN		����
 * \param   CardType	���ڱ�ǿ�Ƭ����
 * \param   *Card		���ڱ��濨Ƭ��Ϣ
 * \param   *list_c		��Ӧ��������list��Ϣ
 * \param   *list_prev	ǰһ����������list��Ϣ
 * \param   *list_status	�б�״̬
 *
 * \return  ��Ӧlistƫ�Ƶ�ַ
 **/
static u16 SearchCard( u8 *CSN,u8 CardType,CDINFO *Card,HASH_List *list_c,u16 *list_prev,LIST_STATUS *list_status)
{	
	u16 list_addr;	
	u16 hash_value = hash_function(CSN);
	LOG_INFO("%s,%d\r\n",__func__,__LINE__);
	
	list_addr = get_list_addr( hash_value);
	
	if(0xFFFF == list_addr)
	{
		LOG_INFO("%s,%d\r\n",__func__,__LINE__);
		return 0xFFFF;
	}

	*list_status = LIST_STATUS_ONLY_ONE;

	do{
		//hash֮��õ���list���ٻ�ȡ��Ƭ��Ϣ
		get_card_info_addr( list_addr,list_c);

		switch(*list_status)
		{
			case LIST_STATUS_ONLY_ONE:
			{
				if(0xFFFF == list_c->HashList_Next)
				{
					*list_status = LIST_STATUS_ONLY_ONE;
				}
				else
				{
					*list_status = LIST_STATUS_FIRST;
				}
				break;
			}
			case LIST_STATUS_MIDDLE:
			{
				if(0xFFFF == list_c->HashList_Next)
				{
					*list_status = LIST_STATUS_LAST;
				}
				else
				{
					*list_status = LIST_STATUS_MIDDLE;
				}
				break;

			}
			default:
				break;
		}
				

		if(USED_FLAG_TRUE == list_c->HashUsedFlag)//��ǰԪ���Ѿ�����
		{
			//��list�л�ȡ��Ӧ��ַ�µĿ���
			get_card_id( list_c->HashDataAddr,Card);
			
			LOG_INFO("CSN :%x-%x-%x-%x-%x,type:%d\r\n",CSN[0],CSN[1],CSN[2],CSN[3],CSN[4],CardType);
			LOG_INFO("Card:%x-%x-%x-%x-%x,type:%d\r\n",Card->CSN[0],Card->CSN[1],Card->CSN[2],Card->CSN[3],Card->CSN[4],list_c->CardType);

			//������ڵ�ǰ���ţ����ص�ַ
			if( (0==memcmp((void *)CSN,(void *)Card->CSN,CSN_LEN)) &&
				(CardType == list_c->CardType) )
			{				
				LOG_INFO("%s,%d,Card found\r\n",__func__,__LINE__);
				return list_addr;
			}
			//
			else
			{
				*list_status 	= LIST_STATUS_MIDDLE;			
				*list_prev 		= list_addr;
				
				if(list_addr == list_c->HashList_Next)
				{
					LOG_INFO("%s,%d,search finish\r\n",__func__,__LINE__);
					return 0xFFFF;
				}
				else
				{
					list_addr = list_c->HashList_Next;
					LOG_INFO("%s,%d,CSN or Type different\r\n",__func__,__LINE__);
				}
			}
		}
		else if(USED_FLAG_FALSE == list_c->HashUsedFlag)//��ǰԪ��Ϊ��
		{	
			LOG_INFO("%s,%d\r\n",__func__,__LINE__);
			return 0xFFFF;
		}

	}while(1);
	
}





/**
 * \brief   ��ȡlist��ͻ�б������һ����Աƫ�Ƶ�ַ
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   first_address		��ͻ�б��е�һ����Աƫ�Ƶ�ַ
 *
 * \return  ���һ����Աƫ�Ƶ�ַ
 **/
static u16 get_last_list_addr(u16 first_address)
{
	HASH_List list;	
	u16 list_addr = first_address;
	LOG_INFO("%s,%d,first:0x%x\r\n",__func__,__LINE__,first_address);

	do{	
		get_card_info_addr(list_addr,&list);

		if(0xFFFF != list.HashList_Next)//��ǰԪ�ز�����β
		{
			list_addr = list.HashList_Next;//�ȴ����²���
		}
		else
		{
			return list_addr;
		}
		
	}while(0==list.HashUsedFlag);

	return 0xFFFF;
}





/**
 * \brief   ���µ�list���ӵ�������
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   current		��ǰ��Ա
 * \param   next		��һ����Ա
 * \param   data_address ��Ƭ���ݵ�ַ
 *
 * \return  none
 **/
static void link_empty_list(u16 current,u16 next,u16 data_address,u8 CardType)
{
	HASH_List tmp;
	LOG_INFO("%s,%d,c:0x%x,n:0x%x\r\n",__func__,__LINE__,current,next);

	memset(&tmp,0xFF,sizeof(HASH_List));

	tmp.CardType = CardType;

	if(0xFFFF == current)
	{
		tmp.HashUsedFlag = 0;
		tmp.HashDataAddr = data_address;
		write_hash_data(FLASH_BASE_LIST+sizeof(HASH_List)*next,(u8 *)&tmp,sizeof(HASH_List));
	}
	else
	{
		write_hash_data(FLASH_BASE_LIST+sizeof(HASH_List)*current,(u8 *)&next,2);
		
		tmp.HashUsedFlag = 0;
		tmp.HashDataAddr = data_address;
		write_hash_data(FLASH_BASE_LIST+sizeof(HASH_List)*next,(u8 *)&tmp,sizeof(HASH_List));
	}
}



/**
 * \brief   ����ͻlist��ӵ�func�б���
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   hash_value		hashֵ
 * \param   list_addr		hashֵ��Ӧ��list��ַƫ��
 *
 * \return  none
 **/
static void add_list_2_func(u16 hash_value,u16 list_addr)
{
	HASH_Func tmp;	
	LOG_INFO("%s,%d\r\n",__func__,__LINE__);

	tmp.Func2List = list_addr;
	tmp.UsedFlag  = 0;
	write_hash_data(hash_value*sizeof(HASH_Func),(u8 *)&tmp,sizeof(HASH_Func));
}



/**
 * \brief   ����Ƭ��Ϣд��ָ����ַ
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param   *Card		��Ƭ��Ϣ
 * \param   address		���ݿռ�ƫ�Ƶ�ַ
 *
 * \return  none
 **/
static void write_data_2_mem(CDINFO *Card,u16 address)
{
	u32 sec_num = address/CARD_DATA_SUM_PER_SECTOR;
	u32 sector_n = ADDRESS2SECTOR(FLASH_BASE_DATA+0x1000*sec_num);
	LOG_INFO("%s,%d,address;0x%x,sec_num:0x%x,sector_n:0x%x\r\n",__func__,__LINE__,address,sec_num,sector_n);

	write_hash_data(sector_n+((address%CARD_DATA_SUM_PER_SECTOR)*CARD_DATA_LEN),(u8 *)Card,sizeof(CDINFO));

}



//hash_value->list_addr


bool WriteCardInfo( CDINFO *Card,u8 CardType)
{	
	HASH_List list,tmp;
	CDINFO card_tmp;
	LIST_STATUS status;
	u16 list_addr,list_p,last_addr,empty_addr;
	u8 data[5];		
	u16 hash_value;

	LOG_INFO("\r\n%s,%d\r\n",__func__,__LINE__);
	
	memcpy((void *)data,(void *)Card->CSN,CSN_LEN);

	//hash_value���������0--65535		
	hash_value = hash_function(data);	
	
	//��ȡhash��ֵ��Ӧ�ĵ�ַ
	list_addr = get_list_addr( hash_value);
	
	if(0xFFFF != SearchCard( data,CardType,&card_tmp,&list,&list_p,&status))
	{	
		LOG_INFO("%s,%d,Card exist\r\n",__func__,__LINE__);
		return FALSE;
	}	

	empty_addr = get_empty_list( &tmp,CardType);

	LOG_INFO("%s,%d,hash_value:0x%x,list_addr:0x%x,empty_addr:0x%x\r\n",__func__,__LINE__,hash_value,list_addr,empty_addr);

	if(0xFFFF == list_addr)//func���޴˳�Ա
	{
		LOG_INFO("%s,%d\r\n",__func__,__LINE__);
		add_list_2_func( hash_value,empty_addr);
		link_empty_list( 0xFFFF,empty_addr,tmp.HashDataAddr,CardType);
		write_data_2_mem( Card,tmp.HashDataAddr);
	}	
	else
	{	
		LOG_INFO("%s,%d\r\n",__func__,__LINE__);
		last_addr = get_last_list_addr( list_addr);
		link_empty_list( last_addr,empty_addr,tmp.HashDataAddr,CardType);
		write_data_2_mem( Card,tmp.HashDataAddr);
	}	
	return TRUE;
}










/**
 * \brief   ���ӿ�Ƭ��Ϣ
 *
 * \param   pCardHandle ��Ƭ�ļ�����ָ��
 * \param	nCardType	��Ƭ����
 * \param	pCardInfo 	��Ƭ������Ϣ
 * \param	nCardNumber	��Ƭ����
 *
 * \return  �ӿ��Ƿ�ɹ�
 **/
bool AddCard( int nCardType,void * pCardInfo,int nCardNumber)
{	
	int i;
	
	for(i=0;i<nCardNumber;i++)
	{	
		WriteCardInfo( (CDINFO *)(pCardInfo+(sizeof(CDINFO)*i)),(u8)nCardType);
	}
	
	return TRUE;
}









