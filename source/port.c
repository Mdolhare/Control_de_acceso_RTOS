#include "port.h"

static pinIrqFun_t IQRs[CANT_PORTS];



static void port_IQR_handler(uint32_t port);


bool portAddIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun) {

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

void portFILT(pin_t pin, uint8_t ciclos) {
	PORT_Type* ptr_PORT = port_base[PIN2PORT(pin)];
	ptr_PORT -> DFCR = 0;
	ptr_PORT -> DFER = 1;
	ptr_PORT -> DFWR = ciclos;

}

static void port_IQR_handler(uint32_t port)
{
	IQRs[port]();
}

/*
__ISR__ PORTA_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTA);
	port_IQR_handler(PA1);
}
__ISR__ PORTB_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTB);
	port_IQR_handler(PB1);
}
__ISR__ PORTC_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTC);
	port_IQR_handler(PC1);
}
__ISR__ PORTD_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTD);
	port_IQR_handler(PD1);
}
__ISR__ PORTE_IRQHandler(void)
{
	CLEAR_ISF_PORT(PORTE);
	port_IQR_handler(PE1);
}
*/
