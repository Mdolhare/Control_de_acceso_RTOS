#include "Leds_stat.h"
#include "SysTick.h"
#include "gpio.h"
#include "Leds_stat_board.h"
#include "tick.h"

//Interrupcion periodica para prender los leds de status
static void Write_leds_PISR(void);



/*******************************************************************************
 * VARIABLE WITH GLOBAL SCOPE
 ******************************************************************************/
//Arreglo que contiene los estados posibles del multiplexor para prender los leds
//(01, 10, 11)
static uint8_t const est[CANT_ESTADOS][CANT_STAT] = ESTADOS;

//Arreglo utilizado para prender y apagar los leds
static uint8_t enable[CANT_LEDS];

/*******************************************************************************
 * INICIALIZACION
 ******************************************************************************/
void Leds_Stat_Init (void)
{
	gpioMode(STAT_0, OUTPUT);
	gpioMode(STAT_1, OUTPUT);

	tickAdd(Write_leds_PISR);
	gpioWrite(STAT_0, LOW);
	gpioWrite(STAT_1, LOW);
	All_leds_stat_off();
}


/*******************************************************************************
 * FUNCIONES A UTILIZAR POR APP
 ******************************************************************************/
//Habilita al led indicado para que se ilumine
void Write_Led_stat(int i)
{
	enable[i] = HIGH;
}

//Deshabilita al led indicdo para que no se ilumine
void Turn_off_led_stat(int i)
{
	enable[i] = LOW;
}

//Deshabilita a todos los leds juntos
void All_leds_stat_off(void)
{
	for(int i = 0; i < 3; i++)
		enable[i] = LOW;
}

/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/
//Ilumina de a un led, de forma tal que en cada interrupcion se pase al siguiente led a iluminar
static void Write_leds_PISR(void)
{
	static int i = 0;
	if(enable[i])
	{
		gpioWrite(STAT_0, est[i][STATUS0]);
		gpioWrite(STAT_1, est[i][STATUS1]);
	}
	else			//Si el enable esta en 0 no se debe iluminar ningun led en esta irq
	{
		gpioWrite(STAT_0, LOW);
		gpioWrite(STAT_1, LOW);
	}
	i++;
	if(i==3)		//Vuelve a empezar la vuelta
	{
		i=0;
	}
}
