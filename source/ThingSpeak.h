/***************************************************************************//**
  @file     ThingSpeak.h
  @brief    ThingSpeak driver
  @author   G2
 ******************************************************************************/

#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include  <os.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"
#include "gpio.h"
#include "board.h"
/* Task Cloud*/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define QUEUE_SIZE 10

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Envía los datos de los pisos y personas a la nube
 */
void Task_Cloud(void *p_arg);



#endif // _THINGSPEAK_H_


