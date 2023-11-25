/***************************************************************************//**
  @file     Leds_stat.h
  @brief    Status leds functions
  @author   Grupo 2
 ******************************************************************************/

#ifndef _LEDS_STAT_H_
#define _LEDS_STAT_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>

		//   1       2		 3
enum {STAT_D2, STAT_D1, STAT_D3};



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializacion
 */
void Leds_Stat_Init(void);

/**
 * @brief Enciende el led pedido
 * @param led que se debe prender, indicando STAT_DX, X=1, 2, 3
 */
void Write_Led_stat(int i);

/**
 * @brief Apaga el led pedido
 * @param led que se debe apagar, indicando STAT_DX, X=1, 2, 3
 */
void Turn_off_led_stat(int i);

/**
 * @brief Apaga todos los leds de status
 */
void All_leds_stat_off(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _LEDS_STAT_H_
