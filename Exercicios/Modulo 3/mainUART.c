#include <msp430.h> 
#include <stdint.h>
#include "uart.h"

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	uartOpen(1);
	uartPrint("Hello World!\n");

	__enable_interrupt();

	volatile uint8_t c;

	while(1){
        while(!(UCA1IFG & UCTXIFG));    // Aguarda o buffer de transmissão ficar vazio
        c = UCA1RXBUF;
	}
}

