#include "mp_uart_task.h"	
#include "mp_idle_task.h"
#include "mp_memory_task.h"	
#include "mp_door_task.h"	
#include "mp_card_task.h"			

#include "mp_protocol_hostif.h"	
#include "mp_card_task.h"	


#include "app.h"

void app_init(void)
{		
	//�������ݽ���(	�ײ�)		
	tls_uart_init();				

	//�������ݽ���(	Ӧ�ò�)		
	tls_hostif_init();			

	//���ݴ洢����
	tls_memory_init();			

	//�Ž���ʶ������
	tls_card_init();	

	//������������
	tls_door_init();		
		
	//��������
	tls_idle_init();	
	
}



