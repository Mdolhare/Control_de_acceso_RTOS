/*
 * display.h
 *
 *  Created on: 25 ago. 2023
 *      Author: Grupo 2
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PIN_LED_A   PORTNUM2PIN(PB,22)// ???
#define PIN_LED_B   PORTNUM2PIN(PE,26)// PTE26
#define PIN_LED_C   PORTNUM2PIN(PB,21) // PTB21
#define PIN_LED_D   PORTNUM2PIN(PB,22)// ???
#define PIN_LED_E   PORTNUM2PIN(PE,26)// PTE26
#define PIN_LED_F   PORTNUM2PIN(PB,21) // PTB21
#define PIN_LED_G   PORTNUM2PIN(PB,22)// ???
#define PIN_LED_DP  PORTNUM2PIN(PE,26)// PTE26

enum{DIG3,DIG2,DIG1,DIG0};

#define SEL_0	PORTNUM2PIN(PB,22)// ???
#define SEL_1	PORTNUM2PIN(PE,26)// PTE26

//------
#define ON 1
#define OFF 0


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


#endif /* DISPLAY_H_ */
