#include "SysTick.h"
#include "tick.h"
void timerPISR(void);

static uint32_t timeSaved;
static uint32_t speed;
static bool flag_finish = false;
static bool flag_init = false;

void timerInit(uint32_t time_) {
	timeSaved = time_;
	speed = 0;//*(TIME_LOAD/SYSTICK_ISR_FREQUENCY_MHZ);

	tickAdd(timerPISR);
}

void setTimeAndInit(uint32_t time){
	timeSaved = time;
	speed = time;
	flag_finish = false;
	flag_init = true;
}


bool timePassed(void) {
	return flag_finish;
}


uint32_t time(void) {
	return speed;//*(TIME_LOAD/SYSTICK_ISR_FREQUENCY_MHZ);
}


void timerPISR(void) {
	if(flag_init){
		if(!speed) {
			flag_finish = true;
			flag_init = false;
		}
		else{
			speed--;
			flag_finish = false;

		}
	}
}


