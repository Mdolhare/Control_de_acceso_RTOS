/***************************************************************************//**
  @file     Leds_stat_board.h
  @brief    Config leds status
  @author   Grupo 2
 ******************************************************************************/

#ifndef _LEDS_STAT_BOARD_H_
#define _LEDS_STAT_BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>


/*******************************************************************************
 * LOCAL CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define STAT_0     PORTNUM2PIN(PC,10) // PTC5
#define STAT_1     PORTNUM2PIN(PC,11) // PTC7

#define STATUS0 	0
#define STATUS1 	1

#define CANT_ESTADOS 3
#define CANT_STAT	 2
#define CANT_LEDS	 3


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define LED1		{LOW,HIGH}		//01
#define LED2		{HIGH,LOW}		//10
#define LED3		{HIGH,HIGH}		//11

#define ESTADOS {LED1, LED2, LED3}


/*******************************************************************************
 ******************************************************************************/

#endif // _LEDS_STAT_BOARD_H_
