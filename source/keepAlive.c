/*******************************************************************************
  @file     +keepAlive.c+
  @brief    ++
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "keepAlive.h"
#include "uart.h"
#include "gpio.h"
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MSG_LEN 6

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static const unsigned char msg[MSG_LEN] = {
		0xAA, 0x55, 0xC3, 0x3C, 0x01, 0x02
};

static OS_MUTEX* mutex_ptr;

static OS_SEM* semaphore;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Task_KeepAlive(void* p_arg) {

	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_RED, 1);
	gpioWrite(PIN_LED_BLUE, 1);
	OS_ERR os_err;

	unsigned char caracter;

	char placeholder;

	while(1) {
		OSTimeDlyHMSM(0u, 0u, 10u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);

		OSMutexPend(mutex_ptr,             /* (1) Pointer to mutex                           */
					0,                   /*     Wait up until this time for the mutex      */
					OS_OPT_PEND_BLOCKING, /*     Option(s)                                  */
					NULL,                  /*     Timestamp of when mutex was released       */
					&os_err);                /*     Pointer to Error returned                  */

		for(int i=0; i<MSG_LEN; i++) {
			OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &os_err);
			uartWriteMsg2(msg[i]);
		}

		for(int i=0; i<MSG_LEN; i++) {
			OSSemPend(semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &os_err);
			caracter = uartReadMsg(UART_0, &placeholder, 1);
		}

		OSMutexPost(mutex_ptr,             /* (3) Pointer to mutex                           */
					OS_OPT_POST_NONE,
					&os_err);                /*     Pointer to Error returned                  */

		if(caracter == 0x82) {
			gpioToggle(PIN_LED_BLUE);
		}

		else {
			gpioToggle(PIN_LED_RED);
		}
		caracter = 0;

	}

}

void Task_KeepAlive_Set_Mutex(OS_MUTEX* mutex) {
	mutex_ptr = mutex;
}

void Task_KeepAlive_Set_Semaphore(OS_SEM* semaphore_) {
	semaphore = semaphore_;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/














