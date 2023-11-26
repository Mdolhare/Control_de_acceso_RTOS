
/***************************************************************************//**
  @file     Tick.h
  @brief    Tick  driver
  @author   Grupo 2
 ******************************************************************************/

#ifndef _TICK_H_
#define _TICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include "hardware.h"

#include <stdint.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_FUNCTIONS 20
/*
 * @brief carga funcion a ejecutarse en cada tick
 * @param funcallback Function to be call every tick
 * @return registration succeed
 */
bool tickAdd (pinIrqFun_t funcallback);

/*
 * @brief llama a todas las callback de tick
 */
void All_callbacks(void);
void App_OS_SetAllHooks(void);
#endif // _SYSTICK_H_
