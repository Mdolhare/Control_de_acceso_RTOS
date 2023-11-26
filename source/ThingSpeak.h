/***************************************************************************//**
  @file     ThingSpeak.h
  @brief    ThingSpeak driver
  @author   G2
 ******************************************************************************/

#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

#include  <os.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"
#include "gpio.h"
#include "board.h"
/* Task Cloud*/

#define QUEUE_SIZE 10
void Task_Cloud(void *p_arg);
#endif // _THINGSPEAK_H_


