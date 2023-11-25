/***************************************************************************//**
  @file     App.c
  @brief    Driver de lector de banda magnetica
  @author   Grupo 2
 ******************************************************************************/

#ifndef _CARDREADERDRV_H_
#define _CARDREADERDRV_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define START_SENTINEL 0xB
#define END_SENTINEL 0xF
#define FEILD_SEPARATOR 0xD

#define NUM_CHARACTERS 200
#define SIMBOL_SIZE 5

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum {NO_CARD, CARD_REJECTED, CARD_ACCEPTED};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializacion del driver
 */

void cardReaderInit(void);

/*
 * @brief Copia el contenido del buffer en un arreglo (si es que ocurrio una lectura completa y sin errores
 *  y permite leer si el estado de la lectura
 * @param buff puntero a arreglo en donde se quiere copiar el buffer
 * @returns NO_CARD, CARD_REJECTED o CARD_ACCEPTED, dependiendo de si hubo una lectura completa y si
 * 	esta fue correcta o no
 */

uint8_t getBuffer(uint32_t* buff);

/*
 * @brief Reinicia el buffer y el estado de lectura
 */

void deleteBuffer(void);

#endif
