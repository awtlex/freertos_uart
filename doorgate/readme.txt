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









usart.c		Ӳ�����ֵĳ�ʼ���ʹ��ڵĵײ��շ�










uart_rx();//���ڽ�������
	parse_protocol_line();
		tls_hostif_cmd_handler();
			tls_protocmd_parse();//Э�����ݽ���
			tls_protocmd_exec();//Э������ִ��
			tls_hostif_process_cmdrsp();//���ͻظ�����








