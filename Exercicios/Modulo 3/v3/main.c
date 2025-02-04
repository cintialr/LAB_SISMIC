#include <msp430.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	volatile uint16_t adcResult;
	TA0CCTL0 = MC__UP | TASSEL__SMCLK;
	TA0CCR0  = 100 - 1;
	TA0CCTL0 = CCIE;


}
