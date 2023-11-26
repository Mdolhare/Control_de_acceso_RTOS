/***************************************************************************//**
  @file     App.c
  @brief    Driver de lector de banda magnetica
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "CardReaderDrv.h"
#include "SysTick.h"
#include "gpio.h"
#include "board.h"
#include "tick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SPEED 1000
#define MSK(n) (n&(0xFF>>3))
#define MSK2(n) (n&(0xFF>>7))

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum {NOT_READING, READING, CHECK_LRC};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint8_t internalState = NOT_READING;
static uint8_t state = NO_CARD;
static uint32_t buffer[NUM_CHARACTERS];
static uint8_t sentinel = 0;
static uint32_t count = SIMBOL_SIZE;
static uint32_t speed = SPEED;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void cardReaderPISR(void);

static void cardReaderISR(void);

static bool checkEndSentinel(void);

static bool parityCheck(void);

static bool checkLRC(void);

static void sentinel2Buffer(void);

/*******************************************************************************
 *******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/

/**
 * @brief Inicializacion del driver
 */

void cardReaderInit(void) {
	gpioMode(DATA, INPUT);
	gpioMode(ENABLE, INPUT);
	gpioMode(CLK_IN, INPUT);
	gpioIRQ(
		CLK_IN,
		GPIO_IRQ_MODE_FALLING_EDGE,
		cardReaderISR
	);
	/*SysTick_Init();
	SysTick_Add(cardReaderPISR);*/
	tickAdd(cardReaderPISR);
	//gpioMode(PORTNUM2PIN(PE,26),OUTPUT);
	//gpioWrite(PORTNUM2PIN(PE,26),HIGH);
}

/*
 * @brief Copia el contenido del buffer en un arreglo (si es que ocurrio una lectura completa y sin errores
 *  y permite leer si el estado de la lectura
 * @param buff puntero a arreglo en donde se quiere copiar el buffer
 * @returns NO_CARD, CARD_REJECTED o CARD_ACCEPTED, dependiendo de si hubo una lectura completa y si
 * 	esta fue correcta o no
 */

uint8_t getBuffer(uint32_t* buff) {
	int oldState = state;
	state = NO_CARD;
	if (oldState == CARD_ACCEPTED) {
		for(int i = 0; i < NUM_CHARACTERS; i++)
			if (i < count)
				buff[i] = buffer[i];
			else
				buff[i] = 1;
	}
	return oldState;
}

/*
 * @brief Reinicia el buffer y el estado de lectura
 */

void deleteBuffer(void){
	uint32_t i = 0;
	for(i=0;i<NUM_CHARACTERS;i++){
		buffer[i]=1;
	}
	state = NO_CARD;
}

/*******************************************************************************
 *******************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/

/*
 * @brief Interrupcion dedicada, lee el la entrada de datos cada interrupcion (flanco negativo de clock)
 */

void cardReaderISR(void) {
	//gpioWrite(PORTNUM2PIN(PE,26),LOW);
	switch(internalState) {
	case NOT_READING:
		sentinel >>= 1;
		sentinel += gpioRead(DATA) << (SIMBOL_SIZE-1);
		break;
	case CHECK_LRC:
	case READING:
		buffer[count++] = gpioRead(DATA);
		speed = SPEED;
		break;
	}
	//gpioWrite(PORTNUM2PIN(PE,26),HIGH);
}

/*
 * @brief Interrupcion periodica, maquina de estados que se encarga de determinar cuando se esta leyendo datos
 * hacia el buffer y si hubo errores de lectura
 */

void cardReaderPISR(void) {
	if (count > NUM_CHARACTERS) {
		state = CARD_REJECTED;
		internalState = NOT_READING;
		count = SIMBOL_SIZE;
	}
	else{
		switch (internalState) {
			case NOT_READING:
				if((MSK(sentinel) == MSK((uint8_t)(~START_SENTINEL)))) {
					count = SIMBOL_SIZE;
					sentinel2Buffer();
					internalState = READING;
					state = NO_CARD;
					sentinel = 0;
					speed = SPEED;
				}
				break;
			case READING:
				if((count%SIMBOL_SIZE == 0) && (count >=(SIMBOL_SIZE*2))) {
					if (checkEndSentinel()) {
						internalState = CHECK_LRC;
					}
					if (parityCheck()) {
						internalState = NOT_READING;
						state = CARD_REJECTED;
					}

				}
				speed--;
				if(!speed) {
					internalState = NOT_READING;
					state = CARD_REJECTED;
				}
				break;
			case CHECK_LRC:
				if(count%SIMBOL_SIZE == 0) {
					if (checkLRC() || parityCheck()) {
						state = CARD_REJECTED;
						internalState = NOT_READING;
					}
					else {
						state = CARD_ACCEPTED;
						internalState = NOT_READING;
					}
				}
				speed--;
				if(!speed) {
					internalState = NOT_READING;
					state = CARD_REJECTED;
				}
			break;
		}
	}
}

/*
 * @brief revisa si los ultimos 5 bits recibidos forman el end sentinel
 * @returns true si son el end sentinel
 */

bool checkEndSentinel(void){
	int counter = count;
	return (
		!buffer[counter-5] &&
		!buffer[counter-4] &&
		!buffer[counter-3] &&
		!buffer[counter-2] &&
		!buffer[counter-1]
	);
}

/*
 * @brief revisa si se cumple la paridad de los simbolos
 * @returns false si no se detecta error
 */

bool parityCheck(void){
	int num = 0;
	int counter = count;
	for (int i=1; i<=SIMBOL_SIZE; i++) {
		if (buffer[counter-i])
			num++;
	}
	return num%2!=0;
}

/*
 * @brief revisa si se cumple la paridad total
 * @returns false si no se detecta error
 */

bool checkLRC(void) {
	int counter = 0;
	for (int i=0; i<SIMBOL_SIZE-1; i++) {
		for (int j=0; j<count; j+=SIMBOL_SIZE) {
			counter += !buffer[i+j];
		}
		if  (counter%2!=0)
			return true;
		counter = 0;
	}
	return false;
}

/*
 * @brief escribe el start sentinel en el buffer
 */

void sentinel2Buffer(void) {
	for (int i=SIMBOL_SIZE-1; i>=0; i--) {
		buffer[i] = MSK2(sentinel>>i);
	}
}

