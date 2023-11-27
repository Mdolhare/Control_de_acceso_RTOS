/***************************************************************************//**
  @file     Encoder.h
  @brief    Encoder driver
  @author   Grupo 2
 ******************************************************************************/

#ifndef _ENCODER_H_
#define _ENCODER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <os.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializacion
 */
void DRV_Enc_Init (OS_Q* queue_encoder);

/**
 * @brief Devuelve la cantidad de pasos hechos por el encoder y resetea. Un paso
 * del encoder se cuenta una vez que vuelve al estado inicial.
 *
 * @return Cantidad de pasos. Si los giros fueron hacia la izquierda el resultado
 * es negativo, si fueron hacia la derecha es positivo.
 */
int get_vueltas(void);

/**
 * @brief Devuelve la cantidad de veces que fue apretado el boton y resetea
 * @return Cantidad de veces que fue apretado el boton
 */
int get_boton(void);

/**
 * @brief Cuenta los pasos del encoder una vez que este vuelve a su estado inicial. Si las vueltas
 * se hicieron a la izquierda, el valor es negativo. Si se hicieron hacia la derecha el valor es
 * positivo.
 *
 * @return Pasos del encoder si este volvio al estado inicial, 0 si todavia no dio un giro completo
 */
int get_paso(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _ENCODER_H_
