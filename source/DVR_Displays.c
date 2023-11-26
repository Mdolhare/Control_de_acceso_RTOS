/*******************************************************************************
  @file     +gpio.c+
  @brief    +Driver para LEDs+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "DVR_Displays.h"
#include "gpio.h"
#include "SysTick.h"
#include "board_leds.h"
#include <stdbool.h>
#include "tick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define HABILITADO 1
#define BIT2LEDSTATE(p,n) ((p[i] >> n) & 1)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void showData(void);
static void blink(void);
static void brillo(void);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
//pin de los leds
static pin_t const leds_board[CANT_LEDS] = LEDS_BOARD;

//pin de los puertos que selecionan display {sel0,sel1}
static pin_t const disp[CANT_SEL] = DISP_BOARD ;

//estados de las salida para selecionar display {D0,D1,D2,D3}
static uint8_t const dig_dir[CANT_DISP][CANT_SEL] = DIR_DIGS;

//configuracion de leds para cada representacion en display
static uint8_t leds_config_to_num[CANT_SIMBOLOS] = SIMBOLOS;

//arreglo con datos a cargar en display
static uint8_t data_disp[CANT_DISP]; //-1 para no contar el 'led punto'

//dots a prender o no
static uint8_t data_dots[CANT_DISP];

//variables para el blink de un display
static uint8_t disp_habilitado[CANT_DISP];
static uint8_t disp_to_blink;
static uint8_t blink_flag;

//variables para el control de intensidad
static uint32_t intensidad = MAX_INTENS;
static uint8_t newDisp_flag;



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void ledsInit(){
	uint8_t i=0;

	//GPIOS para leds
	for(i = 0; i < CANT_LEDS; i++){
		gpioMode(leds_board[i], OUTPUT);
		gpioWrite(leds_board[i], LED_ON);
	}
	//GPIOS para pines selectores de display
	for(i = 0; i < CANT_SEL; i++){
		gpioMode(disp[i], OUTPUT);
		gpioWrite(disp[i], LOW);
	}

	//inicializacion de numeros simbolos 
	#if (ACTIVE_LED==LOW)
		for(i = 0; i < CANT_SIMBOLOS; i++){
			leds_config_to_num[i] = ~leds_config_to_num[i];
		}
	#endif

	//inicializacion de variables para display
	for(i = 0; i < CANT_DISP; i++){
		data_disp[i] = leds_config_to_num[0];
		data_dots[i] = LED_OFF;
		disp_habilitado[i] = HABILITADO;
	}

	blink_flag = false;
	newDisp_flag = 0;
	disp_to_blink = D0;

	/*SysTick_Init();
	SysTick_Add(showData);
	SysTick_Add(blink);
	SysTick_Add(brillo);*/
	tickAdd(showData);
	tickAdd(blink);
	tickAdd(brillo);
}

void writeDisp(uint8_t numero, uint8_t digito){
	if(numero < CANT_SIMBOLOS && digito < CANT_DISP){
		data_disp[digito] = leds_config_to_num[numero];
	}
}

void writeWord(uint8_t * data){
	uint8_t i = 0;
	for(i=0;i<CANT_DISP;i++){
		data_disp[i] = leds_config_to_num[data[i]];
	}
}

void writeNum(uint64_t num){
	int8_t i = 0;
	for(i = CANT_DISP-1; i > 0; i--){
		data_disp[i] = leds_config_to_num[num % 10];
		num /= 10;
	}
	data_disp[0] = leds_config_to_num[num % 10];
}


void writeDot(uint8_t dot, uint8_t digito){
	if(digito < CANT_DISP){
		data_dots[digito] = dot;
	}
}

void blinkDisp(uint8_t digito){
	if(!blink_flag){
		uint8_t i = 0;
		for(i = 0; i < CANT_DISP; i++){
			disp_habilitado[i] = HABILITADO;
		}
	}
	if(digito < CANT_DISP){
		disp_to_blink = digito;
		blink_flag = 1;
	}
}

void stopBlink(void){
	blink_flag = 0;
	uint8_t i = 0;
	for(i = 0; i < CANT_DISP; i++){
		disp_habilitado[i] = HABILITADO;
	}
}

void setIntensidad(uint8_t value){
	if((value <= MAX_INTENS) && (value > 0)){
		intensidad = value;
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief Interrupcion periodica que refresca el display y carga los datos
 *			a mostrar.
 */
static void showData(void){
	static uint8_t i = 0;
	static uint32_t tempo = REFRESH_TIME;

	if(tempo){
		tempo--;
	}
	else{
		gpioWrite(disp[SEL0],dig_dir[i][SEL0]);
		gpioWrite(disp[SEL1],dig_dir[i][SEL1]);
		if(disp_habilitado[i]){
			//elijo display
			//gpioWrite(disp[SEL0],dig_dir[i][SEL0]);
			//gpioWrite(disp[SEL1],dig_dir[i][SEL1]);
			uint8_t j = 0;
			for(j = 0;j < CANT_LEDS-1; j++){
				gpioWrite(leds_board[j], BIT2LEDSTATE(data_disp,j));
			}
		}
		else{
			//elijo display
			//gpioWrite(disp[SEL0],dig_dir[i][SEL0]);
			//gpioWrite(disp[SEL1],dig_dir[i][SEL1]);
			uint8_t j = 0;
			for(j = 0;j < CANT_LEDS-1; j++){
				gpioWrite(leds_board[j], LED_OFF);
			}
		}
		gpioWrite(leds_board[DP], data_dots[i]);

		//Conociendo que CANT_DISP es potencia de 2: contador circular
		i++;
		i &= 0x03;
		tempo = REFRESH_TIME;
		newDisp_flag = 1;
	}
}

static void blink(void){
	static uint32_t tempo = BLINK_TIME;

	if(blink_flag){
		if(tempo){
			tempo--;
		}
		else{
			uint8_t i = 0;
			for(i = 0; i < CANT_DISP; i++){
				if(i==disp_to_blink){
					disp_habilitado[disp_to_blink] = !disp_habilitado[disp_to_blink];
				}
				else{
					disp_habilitado[i] = HABILITADO;
				}
			}
			tempo = BLINK_TIME;
		}
	}
	else{
		uint8_t i = 0;
		for(i = 0; i < CANT_DISP; i++){
				disp_habilitado[i] = HABILITADO;
		}
	}
}


static void brillo(void){
	static uint32_t tempo = 0;
	if(intensidad != MAX_INTENS && newDisp_flag){
		if(tempo >= intensidad){
			uint8_t j = 0;
			for(j = 0;j < CANT_LEDS; j++){
				gpioWrite(leds_board[j], LED_OFF);

			}
			newDisp_flag = 0;
			tempo = 0;
		}
		else{
			tempo++;
		}
	}
}





