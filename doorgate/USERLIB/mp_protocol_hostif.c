#include "mp_protocol_hostif.h"
#include "list_base.h"
#include "debug.h"	


/* data msg */
struct tls_hostif_tx_msg       g_hostif_tx_msg[HOSTIF_TX_MSG_NUM];
/* event, cmdrsp msg */
struct tls_hostif_tx_msg       g_hostif_tx_event_msg[HOSTIF_TX_EVENT_MSG_NUM];

struct tls_hostif g_hostif;

struct tls_hostif *tls_get_hostif(void)
{
	return &g_hostif;
}	

		
int tls_hostif_init(void)
{
	
}



struct tls_hostif_tx_msg *tls_hostif_get_tx_event_msg(struct tls_hostif *hif)
{
    u32 cpu_sr;
    struct tls_hostif_tx_msg *tx_msg;

    cpu_sr = tls_os_set_critical();
    if (dl_list_empty(&hif->tx_event_msg_list)) {
        tx_msg = NULL;
    } else {
        tx_msg = dl_list_first(&hif->tx_event_msg_list,
                struct tls_hostif_tx_msg, list);
        dl_list_del(&tx_msg->list);
    }
    tls_os_release_critical(cpu_sr); 

    return tx_msg; 
}


struct tls_hostif_tx_msg *tls_hostif_get_tx_msg(void)
{
    u32 cpu_sr;
    struct tls_hostif_tx_msg *tx_msg;

    cpu_sr = tls_os_set_critical();
    if (dl_list_empty(&g_hostif.tx_msg_list)) {
        tx_msg = NULL;
    } else {
        tx_msg = dl_list_first(&g_hostif.tx_msg_list,
                struct tls_hostif_tx_msg, list);
        dl_list_del(&tx_msg->list);
    }
    tls_os_release_critical(cpu_sr); 

    return tx_msg; 
}


int tls_hostif_recv_data(struct tls_hostif_tx_msg *tx_msg) 
{
	
}



int tls_hostif_process_cmdrsp(u8 hostif_type, char *cmdrsp, u32 cmdrsp_size)
{
    struct tls_hostif_tx_msg *tx_msg;
    struct tls_hostif *hif = tls_get_hostif();
    

    //TLS_DBGPRT_INFO("===>\n");
    //printf("**\n");

    if (cmdrsp == NULL || cmdrsp_size == 0)
        return -1;

    switch (hostif_type) 
	{
        case HOSTIF_MODE_UART0:
            tx_msg = tls_hostif_get_tx_event_msg(hif);
            if (tx_msg == NULL) 
			{
                TLS_DBGPRT_ERR("event msg is not avaible \n");
                return -1;
            }
            tx_msg->offset = 0;
            tx_msg->u.msg_cmdrsp.buf = cmdrsp;
            tx_msg->type = HOSTIF_TX_MSG_TYPE_CMDRSP;
            tx_msg->u.msg_cmdrsp.buflen = cmdrsp_size;

            if(hif->uart_send_tx_msg_callback != NULL)
                hif->uart_send_tx_msg_callback(hostif_type, tx_msg, FALSE);
            break;
        case HOSTIF_MODE_UART1:	
            tx_msg = tls_hostif_get_tx_event_msg(hif);
            if (tx_msg == NULL)
                return -1;

            tx_msg->offset = 0;
            tx_msg->u.msg_cmdrsp.buf = cmdrsp;
            tx_msg->type = HOSTIF_TX_MSG_TYPE_CMDRSP;
            tx_msg->u.msg_cmdrsp.buflen = cmdrsp_size;

            if(hif->uart_send_tx_msg_callback != NULL)
                hif->uart_send_tx_msg_callback(hostif_type, tx_msg, FALSE);
            break;
        default:
            break;
    }
    return 0;
}




int tls_hostif_cmd_handler(u8 hostif_cmd_type, char *buf, u32 length)
{
	#define CMD_RSP_BUF_SIZE    128	

    char *cmdrsp_buf;
    u32 cmdrsp_size;
    struct tls_protocmd_token_t protocmd_tok;
    int err;
    int i;	
    struct tls_hostif_hdr *hdr = (struct tls_hostif_hdr *)buf;
    u8 hostif_type;
	struct tls_hostif *hif = tls_get_hostif();

    cmdrsp_size = CMD_RSP_BUF_SIZE;

    switch (hostif_cmd_type) 
	{
        case HOSTIF_UART1_CMD:
        case HOSTIF_UART0_CMD:	
            if (hostif_cmd_type == HOSTIF_UART1_CMD)
                hostif_type = HOSTIF_MODE_UART1;	
            else
                hostif_type = HOSTIF_MODE_UART0;
			
            cmdrsp_buf = tls_mem_alloc(CMD_RSP_BUF_SIZE);
				
            if (!cmdrsp_buf)		
                return -1;	
				
            memset(&protocmd_tok, 0, sizeof(struct tls_protocmd_token_t));
		
            err = tls_protocmd_parse(&protocmd_tok, buf+1, length - 1);

            if (err) 
			{	
                cmdrsp_size = sprintf(cmdrsp_buf, "+ERR=%d\r\n", err);
            } 
			else 
			{
                cmdrsp_size = CMD_RSP_BUF_SIZE;
	
                err = tls_protocmd_exec(&protocmd_tok, cmdrsp_buf, &cmdrsp_size);
            }
            break;
        default:
            
			return -1;
            //break;
    }

	//���ͻظ���Ϣ
    err = tls_hostif_process_cmdrsp(hostif_type, cmdrsp_buf, cmdrsp_size);
    if (err)
    {	
        tls_mem_free(cmdrsp_buf);
    }

    return err;

}


