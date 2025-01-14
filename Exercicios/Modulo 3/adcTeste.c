#include <adc.h>


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	volatile uint16_t ADCResult;

    while(1){
        ADCResult = adcRead(6);
    }
}
