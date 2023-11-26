/*******************************************************************************
  @file     +ThingSpeak.c+
  @brief    +Send data to cloud+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "ThingSpeak.h"
#include "uart.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MSG_LEN 12
#define RX_LEN 6

enum cases {PISO_1_1=1, PISO_2_1, PISO_3_1, PISO_1_2, PISO_2_2, PISO_3_2, PISO_1_3, PISO_2_3, PISO_3_3};

static unsigned char msg[MSG_LEN] = {
	0xAA, 0x55, 0xC3, 0x3C, 0x07, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



OS_SEM semaphore;

/*******************************************************************************
 * FUNCTION WITH GLOBAL SCOPE
 ******************************************************************************/

void Task_Cloud(void *p_arg) {

    (void)p_arg;
    OS_ERR os_err;

    uart_cfg_t config = {.baudrate=1200, .parity=true};

    uartInit(UART_0, config, &semaphore);

    char* p_msg;
    OS_MSG_SIZE p_size;

    char rx;

    while (1) {

    	p_msg = OSQPend((OS_Q*)p_arg, 0, OS_OPT_PEND_BLOCKING, &p_size, NULL, &os_err);

		switch(*p_msg) {

			case PISO_1_1:
				msg[MSG_LEN-6]=0;
			break;

			case PISO_2_1:
				msg[MSG_LEN-4]=0;
			break;

			case PISO_3_1:
				msg[MSG_LEN-2]=0;
			break;

			case PISO_1_2:
				msg[MSG_LEN-6]=1;
			break;

			case PISO_2_2:
				msg[MSG_LEN-4]=1;
			break;

			case PISO_3_2:
				msg[MSG_LEN-2]=1;
			break;

			case PISO_1_3:
				msg[MSG_LEN-6]=2;
			break;

			case PISO_2_3:
				msg[MSG_LEN-4]=2;
			break;

			case PISO_3_3:
				msg[MSG_LEN-2]=2;
			break;

			default: break;

		}

		for(int i=0; i<MSG_LEN; i++) {
			OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &os_err);
			uartWriteMsg2(msg[i]);
		}

		for(int i=0; i<RX_LEN; i++) {
			OSSemPend(&semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);
			uartReadMsg(UART_0, &rx, 1);
		}



    }
}
