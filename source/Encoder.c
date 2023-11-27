#include "Encoder.h"
//#include "SysTick.h"
#include "gpio.h"
#include "Encoder_config.h"
#include "tick.h"

//Interrupcion en caso de giro del encoder
static void PIN_AB(void);

//Interrupcion en caso de que se haya apretado el boton
static void DRV_PISR(void);


/*******************************************************************************
 * VARIABLE WITH GLOBAL SCOPE
 ******************************************************************************/
//Si las vueltas fueron hacia la derecha es positivo, sino negativo
static int vueltas;

//Cantidad de veces que fue apretado el boton
static int bot;

//Variable que se utilizara para contar las rotaciones del encoder
static int val_prev;

static OS_Q* queue_encoder_1;

/*******************************************************************************
 * INICIALIZACION
 ******************************************************************************/
void DRV_Enc_Init (OS_Q* queue_encoder)
{
	queue_encoder_1 =  queue_encoder;
	gpioMode(PIN_A, INPUT);
	gpioMode(PIN_B, INPUT);
	gpioMode(BOTON, INPUT);
	gpioIRQ(PIN_A, GPIO_IRQ_MODE_BOTH_EDGES, PIN_AB);
	gpioIRQ(PIN_B, GPIO_IRQ_MODE_BOTH_EDGES, PIN_AB);
	gpioFILT(PIN_A,0b11111);
	gpioFILT(PIN_B,0b11111);

	/*SysTick_Init();
	SysTick_Add(DRV_PISR);*/
	tickAdd(DRV_PISR);
	vueltas = 0;
	bot = 0;
	val_prev = (gpioRead(PIN_A)<<1)+ gpioRead(PIN_B);

	gpioMode(PORTNUM2PIN(PE,26),OUTPUT);
	gpioMode(PORTNUM2PIN(PB,9),OUTPUT);
	gpioWrite(PORTNUM2PIN(PB,9),HIGH);

	gpioWrite(PORTNUM2PIN(PE,26),HIGH);

}


/*******************************************************************************
 * FUNCIONES A UTILIZAR POR APP
 ******************************************************************************/
//Devuelve la cantidad de rotaciones del encoder
int get_vueltas()
{
	int32_t giros = vueltas/ESTADOS_TICK;
	vueltas = 0;
	return giros;
}

//Si paso un tick devuelve la cantidad de vueltas, sino 0
int get_paso()
{
	if(vueltas >= 4 || vueltas <= -4)
		return get_vueltas();
	else
		return 0;
}

//Devuelve la cantidad de veces que fue apretado el boton
int get_boton()
{
	int push = bot;
	bot = 0;
	return push;
}



/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/
//Interrupcion dedicada para contar los giros del encoder
void PIN_AB(void)
{
	gpioWrite(PORTNUM2PIN(PE,26),LOW);
	int val_new = (gpioRead(PIN_A)<<1)+ gpioRead(PIN_B);
	switch(val_new)
	{
		case ESTADO1:
			if(val_prev == ESTADO3)
				vueltas++;			//giro DER
			if(val_prev == ESTADO2)
				vueltas--;			//giro IZQ
			break;
		case ESTADO2:
			if(val_prev == ESTADO1)
				vueltas++;
			if(val_prev == ESTADO4)
				vueltas--;
			break;
		case ESTADO3:
			if(val_prev == ESTADO4)
				vueltas++;
			if(val_prev == ESTADO1)
				vueltas--;
			break;
		case ESTADO4:
			if(val_prev == ESTADO2)
				vueltas++;
			if(val_prev == ESTADO3)
				vueltas--;
			break;
	}
	val_prev = val_new;
	gpioWrite(PORTNUM2PIN(PE,26),HIGH);
}


//Interrupcion periodica que cuenta la cantidad de veces que se apreto el boton
void DRV_PISR(void)
{
	static int val_prev;
	int val_new = gpioRead(BOTON);

	OS_ERR os_err;
	if((val_new != val_prev) && val_new == BOT_ACTIVE)
	{
		bot++;
	}
	val_prev= val_new;
	char msg = get_boton();
	if(msg)
	{
		OSQPost(queue_encoder_1, (void*)(&msg), sizeof(void), OS_OPT_POST_FIFO, &os_err);
	}


}


