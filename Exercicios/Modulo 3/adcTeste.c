#include <msp430.h>
#include <stdint.h>


volatile uint16_t ADCResult;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	TA0CTL = MC__UP | TASSEL__SMCLK;

	TA0CCR0 = 100-1;            // ativação do timer a cada 100μs
	TA0CCTL0 = CCIE;
    ADC12CTL0 = 0;                         // zera o bit ENC

    ADC12CTL0 = ADC12SHT0_2      |         // SHT = 16 batidas de clock
                ADC12ON;                   // Liga o conversos AD

    ADC12CTL1 = ADC12CSTARTADD_5 |         // Guarda o resultaod no MEM5
                ADC12SHS_0       |         // usa o bit SC como trigger para iniciar a conversã
                ADC12SHP         |         // Usa o timer interno do ADC12
                ADC12SSEL_0      |         // usa o MODSLK como clk do ADC , @5MHz
                ADC12CONSEQ_0;             // sigle channel, single conversion

    ADC12CTL2 = ADC12TCOFF       |         // desliga o sensor de temperatura
                ADC12RES_2       |         // usa a resolução de 12 bits
                ADC12SR;                   // Fs até 50ksps

    ADC12MCTL5 = ADC12SREF_0     |         // usa referencia padrão de AVSS e AVCC
                 ADC12INCH_6;                     // entrada no P6.6


    ADC12IE = ADC12IE5;                    // usa a interrupcao no MEM5

    __enable_interrupt();


    while(1){
        __low_power_mode_0();
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void ADC_TRIGGER(){
    ADC12CTL0 |=  ADC12ENC;           // habilita o trigger
    ADC12CTL0 &= ~ADC12SC;            // flanco de subida bit SC
    ADC12CTL0 |=  ADC12SC;

}
#pragma vector = ADC12_VECTOR
__interrupt void ADC_RESULT(){
    ADCResult = ADC12MEM5;
}
