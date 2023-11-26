
#include "ThingSpeak.h"





void Task_Cloud(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;
    char* p_msg;
    OS_MSG_SIZE p_size;
    while (1) {
    	p_msg = OSQPend(&queue, 0, OS_OPT_PEND_BLOCKING, &p_size, NULL, &os_err);

    	    	switch(*p_msg) {
    	    		case 0:
    	    			gpioToggle(PIN_LED_RED);
						break;
    	    		case 1:
    	    			gpioToggle(PIN_LED_BLUE);
    				break;
    	    		case 2:
    	    			gpioToggle(PIN_LED_GREEN);
    				break;

    	    	}
    }
}

