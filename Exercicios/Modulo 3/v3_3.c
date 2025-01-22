#include <msp430.h> 
#include <stdint.h>

uint16_t adcRead(uint8_t pin){
    ADC12CTL0 &= ~ADC12ENC;                 // zera o bit ENC

    ADC12CTL0 = ADC12SHT0_4      |          // SHT = 16 batidas de clock
                ADC12ON;                    // Liga o conversos AD

    ADC12CTL1 = ADC12SHS_0       |          // usa o bit SC como trigger para iniciar a convers�
                ADC12SHP         |          // Usa o timer interno do ADC12
                ADC12SSEL_0      |          // usa o MODSLK como clk do ADC , @5MHz
                ADC12CONSEQ_0;              // sigle channel, single conversion

    ADC12CTL2 = ADC12TCOFF       |          // desliga o sensor de temperatura
                ADC12RES_2       |          // usa a resolu��o de 12 bits
                ADC12SR;                    // Fs at� 50ksps

    P6SEL |= 1 << pin;

    ADC12MCTL0 = pin;                       // entrada no P6.{pin}*/

    ADC12CTL0 |= ADC12ENC;                  // habilita trigger

    ADC12CTL0 &= ~ADC12SC;                  // gera um flanco de subida no bit SC start conversion
    ADC12CTL0 |=  ADC12SC;

    while (!(ADC12IFG & BIT0));
        return ADC12MEM0;
}

void main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	volatile uint16_t adc0, adc1;

	while(1){
        adc0 = adcRead(0);
        adc1 = adcRead(1);
    }
}
