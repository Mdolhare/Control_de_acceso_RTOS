/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include "hardware.h"

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SYSTICK_ISR_FREQUENCY_MHZ 100UL


#define TIME_LOAD  (100000-1) //1 ms

#define MAX_FUNCTIONS 20
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*pinIrqFun_t)(void);
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize SysTic driver
 * @return Initialization succeed
 */
//bool SysTick_Init (void);

/*
 * @brief carga funcion a ejecutarse en cada tick
 * @param funcallback Function to be call every SysTick
 * @return registration succeed
 */
//bool SysTick_Add(pinIrqFun_t funcallback);

/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTICK_H_
