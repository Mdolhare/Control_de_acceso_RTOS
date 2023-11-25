/***************************************************************************//**
  @file     cola.c
  @brief
  @author   Grupo 2
 ******************************************************************************/

#ifndef _COLA_H_
#define _COLA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define Q_SIZE 1000

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
	unsigned char buffer[Q_SIZE];
	unsigned char* outPtr;
	unsigned char* inPtr;
	uint32_t count;
	bool overflow;
} cola_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void colaInit(cola_t* cola);

bool colaPush(cola_t* cola, unsigned const char data);

unsigned char colaPull(cola_t* cola);

/*******************************************************************************
 ******************************************************************************/

#endif // _COLA_H_
