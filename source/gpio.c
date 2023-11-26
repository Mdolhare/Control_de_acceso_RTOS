/*******************************************************************************
  @file     +gpio.c+
  @brief    +Driver para GPIO+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"
#include "../SDK/CMSIS/MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


#define CANT_PORTS 5
#define BITS_PORTS 32

/*
 * brief: resetea todas las ISF correspondientes a un puerto.
 * param: p PORT_Type * indicando el puerto A,B,C,D,E correspondiente
 */
#define CLEAR_ISF_PORT(p) (p->ISFR=0xFFFFFFFF)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {

    uint32_t PS : 1;
    uint32_t PE : 1;
    uint32_t SRE : 1;
    uint32_t RESERVED_0 : 1;
    uint32_t PFE : 1;
    uint32_t ODE : 1;
    uint32_t DSE : 1;
    uint32_t RESERVED_1 : 1;
    uint32_t MUX : 3;
    uint32_t RESERVED_2 : 4;
    uint32_t LK : 1;
    uint32_t IRQC : 4;
    uint32_t RESERVED_3 : 4;
    uint32_t ISF : 1;
    uint32_t RESERVED_4 : 7;

} PCR_Type;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void port_IQR_handler(uint32_t port);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static PORT_Type * const port_base[CANT_PORTS] = PORT_BASE_PTRS;
static GPIO_Type * const gpio_base[CANT_PORTS] = GPIO_BASE_PTRS;
static uint32_t const ports_irqs[CANT_PORTS] = PORT_IRQS;


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static pinIrqFun_t IQRs[CANT_PORTS];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void  gpioMode (pin_t pin, uint8_t mode) {

    PORT_Type* ptr_port = port_base[PIN2PORT(pin)];
    GPIO_Type* ptr_gpio = gpio_base[PIN2PORT(pin)];

    //configuracion PCR
    PCR_Type* ptr_pcr;
    ptr_pcr = (PCR_Type*)(&(ptr_port->PCR[PIN2NUM(pin)]));
    ptr_pcr -> MUX = 1;

    switch(mode){
		case OUTPUT:
			//ptr_gpio -> PDDR |= BITN1(PIN2NUM(pin));
			BITBAND_REG32((ptr_gpio -> PDDR),PIN2NUM(pin)) = 1;
			break;
		case INPUT:
			//ptr_gpio -> PDDR &= BITN0(PIN2NUM(pin));
			BITBAND_REG32((ptr_gpio -> PDDR),PIN2NUM(pin)) = 0;
			break;
		case INPUT_PULLUP:
			//ptr_gpio -> PDDR &= BITN0(PIN2NUM(pin));
			BITBAND_REG32((ptr_gpio -> PDDR),PIN2NUM(pin)) = 0;
			ptr_pcr -> PE = 1;
			ptr_pcr -> PS = 1;
			break;
		case INPUT_PULLDOWN:
			//ptr_gpio -> PDDR &= BITN0(PIN2NUM(pin));
			BITBAND_REG32((ptr_gpio -> PDDR),PIN2NUM(pin)) = 0;
			ptr_pcr -> PE = 1;
			ptr_pcr -> PS = 0;
			break;
	}
}


void gpioToggle (pin_t pin) {
    GPIO_Type* ptr_gpio = gpio_base[PIN2PORT(pin)];

	//R-M-W
	//ptr_gpio -> PTOR |= BITN1(PIN2NUM(pin));

    //Con Bit-Band - linea 128 de MK64F12.h
    BITBAND_REG32((ptr_gpio -> PTOR),PIN2NUM(pin)) = 1;
}

void gpioWrite (pin_t pin, bool value) {
	//escribir en PDOR en valor correspondiente
    GPIO_Type* ptr_gpio = gpio_base[PIN2PORT(pin)];

	BITBAND_REG32((ptr_gpio -> PDOR),PIN2NUM(pin)) = value;
}


bool gpioRead (pin_t pin) {
    //GPIO_Type* ptr_gpio = searchGPIO(pin);
    GPIO_Type* ptr_gpio = gpio_base[PIN2PORT(pin)];
    return BITBAND_REG32((ptr_gpio -> PDIR),PIN2NUM(pin));
}

bool gpioIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun){

	PORT_Type* ptr_port = port_base[PIN2PORT(pin)];

	PCR_Type* ptr_pcr;

	    //configuracion PCR
	ptr_pcr = (PCR_Type*)(&(ptr_port->PCR[PIN2NUM(pin)]));
	ptr_pcr->IRQC = irqMode;
	ptr_pcr->ISF = 0;
	IQRs[PIN2PORT(pin)] = irqFun;
	NVIC_EnableIRQ(ports_irqs[PIN2PORT(pin)]);

	return true;

}

void gpioFILT(pin_t pin, uint8_t ciclos)
{
	PORT_Type* ptr_PORT = port_base[PIN2PORT(pin)];
	ptr_PORT -> DFCR = 0;
	ptr_PORT -> DFER = 1;
	ptr_PORT -> DFWR = ciclos;

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void port_IQR_handler(uint32_t port)
{
	IQRs[port]();
}



__ISR__ PORTA_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTA);
	port_IQR_handler(PA);
}
__ISR__ PORTB_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTB);
	port_IQR_handler(PB);
}
__ISR__ PORTC_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTC);
	port_IQR_handler(PC);
}
__ISR__ PORTD_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTD);
	port_IQR_handler(PD);
}
__ISR__ PORTE_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTE);
	port_IQR_handler(PE);
}

