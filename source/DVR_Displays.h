/*******************************************************************************
  @file     +DVR_Displays.h+
  @brief    +Driver para 4 Displays+
  @author   +GRUPO 2+
 ******************************************************************************/
#ifndef DVR_DISPLAYS_H_
#define DVR_DISPLAYS_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANT_LEDS 8
#define CANT_DISP 4

//vista placa
//			|1| |1| |1| |1|   |leds status|
enum digitos		{D0, D1, D2, D3};

enum simbolos_display {APAGADO=10,SIM_TICK,SIM_VOLVER,SIM_GUION};
enum letras_display {LETRA_A=14, LETRA_B, LETRA_C, LETRA_D, LETRA_E, LETRA_I, LETRA_N, LETRA_P, LETRA_R};

#define LED_ON ACTIVE_LED
#define LED_OFF !ACTIVE_LED
#define MAX_INTENS 5
#define MIN_INTENS 1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint32_t delay_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Configura e inicializa al display
 */
void ledsInit();

/**
 * @brief Permiete que uno de los 4 displays titile
 * @param digito: selecciona cual de los display va a titilar
 */
void blinkDisp(uint8_t digito);

/**
 * @brief Hace que pare de titilar el display sin importar cual titilaba
 */
void stopBlink();

/**
 * @brief Permiete que uno de los 4 displays titile
 * @param numero: numero (0 a 9) o constante definida
 * 					(simbolos y letras) a mostrar en el display
 * @param digito: indica en cual display mostrar.
 */
void writeDisp(uint8_t numero, uint8_t digito);

/**
 * @brief Carga al display 4 digitos apuntados en data en forma ordenada.
 * 				elemento0 -> D0 ... elemento3 -> D3
 * @param data: puntero a 4 elementos para mostrar
 */
void writeWord(uint8_t * data);

/**
 * @brief Muestra en el display los 4 digitos menos significativos de un numero
 * @param num: numero para mostrar
 */
void writeNum(uint64_t num);

/**
 * @brief Activa o no los puntos de un display especifico
 * @param dot: estado del punto, LED_ON o LED_OFF
 * @param digito: digito en cual modificar el punto
 */
void writeDot(uint8_t dot, uint8_t digito);

/**
 * @brief Modifica la intensidad del display con niveles de MAX_INTENS a MIN_INTENS
 * @param value: valor de intensedad entre MAX_INTENS y MIN_INTENS
 */
void setIntensidad(uint8_t value);

/*******************************************************************************
 ******************************************************************************/

#endif /* DVR_DISPLAYS_H_ */
