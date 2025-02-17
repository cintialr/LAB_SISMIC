#include "uart.h"

uint16_t count;
extern volatile uint8_t rxBuff[12], i;


#define MAX_CH      8
#define TX          6,0
#define RX          6,1
#define TB          833,us

/*
 * envia STAR | BIT0 | BIT1 | ... | BIT7 | STOP
 * Baudrate: 1200 -> Tb = 883us
 */

void uartOpen(uint8_t interface){
    if(interface == 1){
        UCA1CTL1 = UCSWRST;         // reseta a interface para permitir a config.

        UCA1CTL0 = /* UCPEN    |    // habilita a paridade
                   UCPAR    |       // par = 0, impar = 1
                   UCMSB    |       // manda o LSB primeiro (MSB = 0)
                   UC7BIT   |       // manda pacotes de 8 bits
                   UCSPB    |       // manda 1 stop */
                   UCMODE_0 |       // modo UART
                   //UCSYNC;        // modo assíncrono
                   0;

        UCA1CTL1 |= UCSSEL__SMCLK;  // seleciona o SMCLK clock
        /** SMCLK
         * Clock de entrada bate a 2^20Hz
         * Baudrate = 19200 bits por seg
         * BRW = 2^20 / 19200 = 54,61 > 32          quando > que 32 usar oversampling
         *
         * sem oversampling 16
         * BRW = 54, BRS = 0,61*8 = 4,88 ~= 5
         * Problema: a janela de amostragem é pequena
         *
         * modo de oversampling  (OS16 = 1)
         * D = 2^20 / 19200 / 16 = 3,41
         * BRW = 3, BRF = 0,41*16 = 6,56 ~= 7
         **/

        /** ACLK @32768 Hz
         *  sem oversampling BRW = 1, BRS=0,71*8 = 5,68 ~= 6
         *
         */

        UCA1BRW  = 3;
        UCA1MCTL = UCBRF_7 | UCOS16;

        P4SEL |= BIT4 | BIT5;       // configura os pinos

        UCA1CTL1 &= ~UCSWRST;       // libera o reset

        UCA1IE = UCRXIE;            // habilita interrupção

    }
}

void uartPrint(char * str){
    while(*str){
        while(!(UCA1IFG & UCTXIFG));    // Aguarda o buffer de transmissão ficar vazio
        UCA1TXBUF = *str++;
    }
}

void wait(uint16_t time, uint16_t unit){
    if(unit == us){
        // USA SMCLK
        TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;
        TA2CCR0 = time;
        while (!(TA2CCTL0 & CCIFG));
        TA2CCTL0 &= ~CCIFG;

    }else{
        // USA ACLK
        TA2CTL = TASSEL__ACLK  | MC__UP | TACLR;
        if(unit == ms){
            TA2CCR0 = ( time << 5) - 1;
            while (!(TA2CCTL0 & CCIFG));
            TA2CCTL0 &= ~CCIFG;
        }
        if (unit == sec){
            count = time;
            TA2CCR0 = 0x8000 - 1;
            TA2CCTL0 = CCIE;
            while(count);
        }
        if (unit == min){
            count = time * 60;
            TA2CCR0 = 0x8000 - 1;
            TA2CCTL0 = CCIE;
            while(count);
        }
    }
    TA2CTL = MC__STOP;
}


void uartBang(uint8_t byte){
    // considero que a saida de TX está em 1 (idle)

    pinWrite(TX, 1);
//    pinMode(TX, output);


    pinWrite(TX, 0);        // ENVIA O START, BIT = 0
    wait(TB);

    uint8_t mask = BIT0;;


    while(mask){            /* envia os 8 bits, começa pelo LSB -> MSB*/
        pinWrite(TX, byte & mask);
        wait(TB);
        mask <<= 1;
    }

    pinWrite(TX, 1);        // ENVIA O STOP, BIT = 1
    wait(TB);

}
/*
#pragma vector = USCI_A1_VECTOR
__interrupt void UART_A1(){
    rxBuff[i] = UCA1RXBUF;
    i++;
    i &= 0x0F;
}*/

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TA2_CCR0_ISR(){
    count--;
}
