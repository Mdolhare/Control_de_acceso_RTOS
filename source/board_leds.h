/*
 * board_leds.h
 *
 *      Author: Grupo 2
 */

#ifndef BOARD_LEDS_H_
#define BOARD_LEDS_H_

#include "gpio.h"

enum {A,B,C,D,E,F,G,DP};
enum {SEL0,SEL1};

//tiempo de prendido de cada display
#define REFRESH_TIME 4 //4 + 1 = 5ms
#define BLINK_TIME 500 //500 + 1 = 501ms

//Polaridad para activar led y display
#define ACTIVE_LED HIGH

//contadores
#define CANT_SEL 2
#define CANT_SIMBOLOS 23


//Numero de pin correspondiente a los leds y selectores de display
#define LED_A PORTNUM2PIN(PA,1)
#define LED_B PORTNUM2PIN(PC,4)
#define LED_C PORTNUM2PIN(PD,0)
#define LED_D PORTNUM2PIN(PD,2)
#define LED_E PORTNUM2PIN(PD,3)
#define LED_F PORTNUM2PIN(PD,1)
#define LED_G PORTNUM2PIN(PC,3)
#define LED_DP PORTNUM2PIN(PC,2)
#define SEL_DISP_0 PORTNUM2PIN(PC,16)
#define SEL_DISP_1 PORTNUM2PIN(PC,17)

#define LEDS_BOARD {LED_A,LED_B,LED_C,LED_D,LED_E,LED_F,LED_G,LED_DP}
#define DISP_BOARD {SEL_DISP_0,SEL_DISP_1}

//configuracion posible en {sel0,sel1} para elegir display
#define DIR_DIG_0 {LOW,LOW}
#define DIR_DIG_1 {HIGH,LOW}
#define DIR_DIG_2 {LOW,HIGH}
#define DIR_DIG_3 {HIGH,HIGH}

#define DIR_DIGS {DIR_DIG_0, DIR_DIG_1, DIR_DIG_2, DIR_DIG_3}


//los siguientes numeros representan con 0 led apagado, 1 led prendido para formar la letra
#define L_0 0X3f
#define L_1 0X06
#define L_2 0X5B
#define L_3 0X4F
#define L_4 0X66
#define L_5 0X6D
#define L_6 0X7D
#define L_7 0X07
#define L_8 0X7F
#define L_9 0X6F
#define L_APAGADO 0X00
#define L_B 0b01111100
#define L_N 0b01010100
#define L_TICK 0b00001110
#define L_VOLVER 0b00110001
#define L_GUION 0b01000000
#define L_D 0b01011110
#define L_I 0b00110000
#define L_C 0b00111001
#define L_E 0b01111001
#define L_R 0b01010000
#define L_P 0b01110011
#define L_A 0b01110111



#define SIMBOLOS {L_0,L_1,L_2,L_3,L_4,L_5,L_6,L_7,L_8,L_9,L_APAGADO,L_TICK,L_VOLVER,L_GUION, L_A,L_B,L_C,L_D,L_E,L_I,L_N,L_P,L_R}


#endif /* BOARD_LEDS_H_ */
