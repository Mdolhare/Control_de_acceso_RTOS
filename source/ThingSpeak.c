
#include "ThingSpeak.h"





void Task_Cloud(void *p_arg) {

    OS_ERR os_err;
    char* p_msg;
    OS_MSG_SIZE p_size;
    gpioMode (PIN_LED_RED, OUTPUT);
    gpioMode (PIN_LED_BLUE, OUTPUT);
    gpioMode (PIN_LED_GREEN, OUTPUT);
    gpioWrite (PIN_LED_BLUE, 1);
    gpioWrite (PIN_LED_RED, 1);
    gpioWrite (PIN_LED_GREEN, 1);

    while (1) {
    	p_msg = OSQPend((OS_Q*)(p_arg), 0, OS_OPT_PEND_BLOCKING, &p_size, NULL, &os_err);
    	    	switch(*p_msg) {
    	    		case 1:
    	    			gpioWrite(PIN_LED_RED,0);
    	    			gpioWrite(PIN_LED_BLUE,1);
    	    			gpioWrite(PIN_LED_GREEN,1);
						break;
    	    		case 4:
    	    			gpioWrite(PIN_LED_RED,1);
    	    			gpioWrite(PIN_LED_GREEN,1);
    	    			gpioWrite(PIN_LED_BLUE,0);

    				break;
    	    		case 7:
    	    			gpioWrite(PIN_LED_RED,1);
        	    		gpioWrite(PIN_LED_GREEN,0);
        	    		gpioWrite(PIN_LED_BLUE,1);
    				break;
    	    		default:
    	    			break;

    	    	}
    }
}

