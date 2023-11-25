/***************************************************************************//**
  @file     Encoder_config.h
  @brief    Encoder driver
  @author   Grupo 2
 ******************************************************************************/

#ifndef _ENCODER_CONFIG_H_
#define _ENCODER_CONFIG_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>


/*******************************************************************************
 * LOCAL CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define PIN_A     PORTNUM2PIN(PB,2) // PTB2
#define PIN_B     PORTNUM2PIN(PB,3) // PTB3
#define BOTON     PORTNUM2PIN(PB,10) // PTB10
 // PTB3


#define ESTADO1	0b00
#define ESTADO2 0b01
#define ESTADO3 0b10
#define ESTADO4 0b11

#define BOT_ACTIVE 0

#define ESTADOS_TICK  4		//cantidad de estados por los que pasa el encoder en cada tick

/*******************************************************************************
 ******************************************************************************/

#endif // _ENCODER_CONFIG_H_
