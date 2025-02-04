#include "adc.h"

uint16_t adcRead(uint8_t port){
    ADC12CTL0 = 0;                         // zera o bit ENC

    ADC12CTL0 = ADC12SHT0_2      |         // SHT = 16 batidas de clock
                ADC12ON;                   // Liga o conversos AD

    ADC12CTL1 = ADC12CSTARTADD_5 |         // Guarda o resultado no MEM5
                ADC12SHS_0       |         // usa o bit SC como trigger para iniciar a conversã
                ADC12SHP         |         // Usa o timer interno do ADC12
                ADC12SSEL_0      |         // usa o MODSLK como clk do ADC , @5MHz
                ADC12CONSEQ_0;             // sigle channel, single conversion

    ADC12CTL2 = ADC12TCOFF       |         // desliga o sensor de temperatura
                ADC12RES_2       |         // usa a resolução de 12 bits
                ADC12SR;                   // Fs até 50ksps

    ADC12MCTL5 = ADC12SREF_0     |         // usa referencia padrão de AVSS e AVCC
                 port;                     // entrada no P6.{port}

    ADC12CTL0 |= ADC12ENC;               // habilita trigger

    ADC12CTL0 &= ~ADC12SC;                 // gera um flanco de subida no bit SC start conversion
    ADC12CTL0 |=  ADC12SC;

    ADC12IE = ADC12IE5;                    // usa a interrupcao no MEM5


    while (!(ADC12IFG & ADC12IFG5));       //     while (!(ADC12IFG & BIT5))
        return ADC12MEM5;
}
