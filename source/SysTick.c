/*#include "SysTick.h"
#include "gpio.h"
static pinIrqFun_t callbacks[MAX_FUNCTIONS];
static bool called = false;
static uint32_t idx = 0;

/*__ISR__ SysTick_Handler(void) {
	gpioWrite(PORTNUM2PIN(PB,9),LOW);

	for(int i = 0; i<idx; i++)
		(callbacks[i])();

	gpioWrite(PORTNUM2PIN(PB,9),HIGH);

}*/

/*
bool SysTick_Init (void) {
	if (!called) {
		SysTick->CTRL = 0x0;
		SysTick->LOAD = TIME_LOAD;
		SysTick->VAL = 0x0 ;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
			SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		called = true;
	}
	return 0;
}

bool SysTick_Add(pinIrqFun_t funcallback) {
	bool check = idx < MAX_FUNCTIONS;
	if (check)
		callbacks[idx++] = funcallback;
	return check;
}

*/
