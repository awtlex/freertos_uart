#include "delay.h"  	
#include "includes.h"
#include "bsp.h"	


/*
**************************************************************************
*                              �����ջ
**************************************************************************
*/

/* ����ÿ������Ķ�ջ�ռ䣬app_cfg.h�ļ��к궨��ջ��С */
static OS_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
static OS_STK AppTaskGUIStk[APP_TASK_GUI_STK_SIZE];	
static OS_STK AppTaskCOMStk[APP_TASK_COM_STK_SIZE];	
static OS_STK AppTaskUpdateStk[APP_TASK_UPDATE_STK_SIZE];	




int main(void)
{ 			
	INT8U  err;

	/* ��ʼ��"uC/OS-II"�ں� */
	OSInit();
  
	/* ����һ����������Ҳ���������񣩡���������ᴴ�����е�Ӧ�ó������� */
	OSTaskCreateExt(AppTaskStart,	/* ����������ָ�� */
                    (void *)0,		/* ���ݸ�����Ĳ��� */
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1], /* ָ������ջջ����ָ�� */
                    APP_TASK_START_PRIO,	/* ��������ȼ�������Ψһ������Խ�����ȼ�Խ�� */
                    APP_TASK_START_PRIO,	/* ����ID��һ����������ȼ���ͬ */
                    (OS_STK *)&AppTaskStartStk[0],/* ָ������ջջ�׵�ָ�롣OS_STK_GROWTH ������ջ�������� */
                    APP_TASK_START_STK_SIZE, /* ����ջ��С */
                    (void *)0,	/* һ���û��ڴ�����ָ�룬����������ƿ�TCB����չ����
                       	      ���������л�ʱ����CPU����Ĵ��������ݣ���һ�㲻�ã���0���� */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* ����ѡ���� */


	/* ָ����������ƣ����ڵ��ԡ���������ǿ�ѡ�� */
	OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)APP_TASK_START_NAME, &err);

	/*ucosII�Ľ��ļ�������0    ���ļ�������0-4294967295*/ 
	OSTimeSet(0);	

	/* ����������ϵͳ������Ȩ����uC/OS-II */
	OSStart();

}





/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(��BSP_Init��ʵ��)
*             ʵ�ִ����Ͱ�����⡣
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����1
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg)
{	
	uint8_t  ucCount = 0;
	uint8_t  ucCount1 = 0;
    /* �����ڱ���������澯����������������κ�Ŀ����� */	
    (void)p_arg;   
	 
	/* BSP ��ʼ���� BSP = Board Support Package �弶֧�ְ����������Ϊ�ײ�������*/
	bsp_Init();      

	/* ���CPU������ͳ��ģ���ʼ�����ú�����������CPUռ���� */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif	
		
	/* ����Ӧ�ó�������� */
	AppTaskCreate();

	/* �������壬������һ����ѭ�� */
	while (1)     
	{   
		//�������ɨ��
		
	
		ucCount++;
		if(ucCount == 5)
		{
			ucCount = 0;
			//��������ɨ��
			bsp_KeyScan();
		}	
		
		OSTimeDly(1);										
	}      
}





void watch_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0; 
 	u8 t=0;	   
 	u8 rerreturn=0; 
	u8 res; 
	u8 key;
 	pdata=pdata;
	while(1)
	{			     
		t++;	 	 
		if((t%60)==0)//900ms���Ҽ��1��
		{ 
			//SD����λ���
			if((DCMI->CR&0X01)==0)//����ͷ��������ʱ��,�ſ��Բ�ѯSD��
			{
				OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
				res=SD_GetState();	//��ѯSD��״̬
				OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��) 
				if(res==0XFF)
				{
					gui_phy.memdevflag&=~(1<<0);//���SD������λ 
					OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
					SD_Init();			//���¼��SD�� 
					OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��) 
				}else if((gui_phy.memdevflag&(1<<0))==0)//SD����λ?
				{
					f_mount(fs[0],"0:",1);		//���¹���sd��
					gui_phy.memdevflag|=1<<0;	//���SD����λ��		
				} 
			}
			//U����λ���   
			if(usbx.hdevclass==1)
			{
				if((gui_phy.memdevflag&(1<<2))==0)//U�̲���λ?
				{
					fs[2]->drv=2;
					f_mount(fs[2],"2:",1);		//���¹���U��
					gui_phy.memdevflag|=1<<2;	//���U����λ	
				}  
			}else gui_phy.memdevflag&=~(1<<2);	//U�̱��γ���  
			//SIM900A���
			sim900a_status_check();  
		}  	
		//sim900a_cmsgin_check();					//����/���� ���
		if(usbx.mode==USBH_MSC_MODE)			//U��ģʽ,�Ŵ���
		{
			while((usbx.bDeviceState&0XC0)==0X40)//USB�豸������,���ǻ�û���ӳɹ�,�Ͳ�ѯ.
			{
				usbapp_pulling();	//��ѯ����USB����
				delay_us(1000);		//������HID��ô��...,U�̱Ƚ���
			}
			usbapp_pulling();//����USB����
		}
		delay_ms(10);
	}
}




/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	
}

