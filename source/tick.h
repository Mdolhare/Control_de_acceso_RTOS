
/***************************************************************************//**
  @file     Tick.h
  @brief    Tick  driver
  @author   Grupo 2
 ******************************************************************************/

#ifndef _TICK_H_
#define _TICK_H_


#include <stdbool.h>
#include "hardware.h"

#include <stdint.h>
#define MAX_FUNCTIONS 20
/*
 * @brief carga funcion a ejecutarse en cada tick
 * @param funcallback Function to be call every tick
 * @return registration succeed
 */
bool tickAdd (pinIrqFun_t funcallback);
void All_callbacks(void);
void App_OS_SetAllHooks(void);
#endif // _SYSTICK_H_
