����FreeRTOS�Ĵ����շ����


���:�����ڷ�Ϊ����
	
	
mp_protocol_hostif.c��protocol.c��ϵ��һ��ʵ���ϲ�Э��Ľ���������Ӧ���ܵ�ִ�У�ͨ���ź�����mp_uart_task.cʵ�ֽ���

	
	
	
	
mp_uart_task.c	�����ϲ�Э��ͨ��
	
	tls_uart_port_init()Ӳ����ʼ��
		tls_uart_open
			tls_uart_rx_register()ע�ᴮ�ڽ����жϻص�(�����ź���)
			tls_uart_tx_register()ע�ᴮ�ڷ����жϻص�(�����ź���)
			
			tls_uart_1_rx_task()����������(�����ź���)
				uart_rx()
					parse_atcmd_line()��������������
						tls_hostif_cmd_handler()
							tls_cmd_exec()
								match->proc_func()ִ�������Ӧ�Ĺ���
			tls_uart_1_tx_task
				
	
	
mp_uart.c	�շ����������м䲿��





�����Э��Բ��ϵĵط�
ReadCard_CardNoReport()










usart.c		Ӳ�����ֵĳ�ʼ���ʹ��ڵĵײ��շ�










uart_rx();//���ڽ�������
	parse_protocol_line();
		tls_hostif_cmd_handler();
			tls_protocmd_parse();//Э�����ݽ���
			tls_protocmd_exec();//Э������ִ��
			tls_hostif_process_cmdrsp();//���ͻظ�����




tls_uart_init
	hif->uart_send_tx_msg_callback = uart_send_tx_msg;	
		//�����ｫ���������б���ʵ���൱�ڣ���tx_event_msg_list�ڵ����ݸ��Ƹ�tx_msg_pending_list���ٽ��з���
		dl_list_add_tail(&uart_st[0].tx_msg_pending_list, &tx_msg->list);
		tls_os_mailbox_send(uart_st[0].tx_mailbox, (void *)MBOX_MSG_UART_TX);//�����ź�����tls_uart_x_tx_task()=>uart_tx()=>dl_list_first()������ȡ��������
	

tx_msg_pending_list��ʾ�ȴ����͵������б���&tx_msg->list���ݴ���

uart_send_tx_msg_callback�����º����б�����	
1.tls_hostif_atcmd_loopback(������������ٴ���ʾ�ڴ�����)		NO
2.tls_hostif_process_cmdrsp(������ִ����ϵĻظ���Ϣ���)		YES
	tx_msg = tls_hostif_get_tx_event_msg(hif);//tx_msg��tls_hostif_get_tx_event_msg()��ã��ڴ����·��ĺ�����
	hif->uart_send_tx_msg_callback(hostif_type, tx_msg);	
3.tls_hostif_send_event(���������¼�������ʱ����)				NO
4.tls_hostif_tx_timeout(��ʱ���ж�ʱ����)						NO	
5.tls_hostif_recv_data(��ԭ���SDK���������紫�䣬������)		NO












//�����������ڲ��д�С���û�������
Ariber_GetArmyParam()
Ariber_GetAlarmParam()




//flash�洢��hash��ϵ
https://blog.csdn.net/u011855647/article/details/62215255

