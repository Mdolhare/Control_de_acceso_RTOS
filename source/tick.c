
/*******************************************************************************
  @file     +tick.c+
  @brief    +tick driver+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************
 ******************************************************************************/
#include  <os.h>
#include "gpio.h"
#include "tick.h"



static pinIrqFun_t callbacks[MAX_FUNCTIONS];
static bool called = false;
static uint32_t idx = 0;


 /*******************************************************************************
 * FUNCTION WITH GLOBAL SCOPE
 ******************************************************************************/


void App_OS_SetAllHooks (void)                             /* os_app_hooks.c         */
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OS_AppTimeTickHookPtr = All_callbacks;
    CPU_CRITICAL_EXIT();
}

bool tickAdd (pinIrqFun_t funcallback) {
	bool check = idx < MAX_FUNCTIONS;
	if (check)
		callbacks[idx++] = funcallback;
	return check;
}


void All_callbacks(void)
{
	for(int i = 0; i<idx; i++)
		(callbacks[i])();
}
