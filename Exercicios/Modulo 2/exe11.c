/* exercicio 11 - interrupção agrupadas */

#include <msp430.h> 
#include <stdint.h>

void main()
{
    // Configuração
    WDTCTL = WDTPW | WDTHOLD;


  // configuração dos pinos

    P1OUT &= ~BIT0;                         // Configura o LED vermelho
    P1DIR |=  BIT0;                         // Saída por padrão em 0
    P4OUT &= ~BIT7;                         // Configura o LED verde
    P4DIR |=  BIT7;                         // Saída por padrão em 0

    P1DIR &= ~BIT1;                         // Configura S2
    P1REN |=  BIT1;                         // Entrada com resistor
    P1OUT |=  BIT1;                         // de pull-up
    P2DIR &= ~BIT1;                         // Configura S1
    P2REN |=  BIT1;                         // Entrada com resistor
    P2OUT |=  BIT1;                         // de pull-up
  
    // Configuração do timer
    TA0CTL  = TASSEL__ACLK |                // Escolhe o ACLK como ref de tempo
              MC__UP       |                // Modo UP: Conta de 0 até CCR0
              TACLR;                        // Começa em 0

    TA0CCR0  = 32768/2 - 1;                 // CCR0 Tem uma ISR só para ele

    TA0CCR1  = 32768/2 - 1;                 // CCR1 e CCR2 dividem uma ISR
    TA0CCR2  = 32768/4 - 1;                 // (interrupção agrupada)

    TA0CCTL1 = CCIE;                        // CCIE - Habilitador local da INT
    TA0CCTL2 = CCIE;                        // CCIE - Habilitador local da INT

    __enable_interrupt();                   // GIE = 1 (Habilitador Global)

    // Loop 
    while(1)
    {
        // Rotina de plano de fundo...

    }

}

#pragma vector = TIMER0_A1_VECTOR           // Coloca o endereço dessa função na tabela de ISRs
__interrupt void INT_TA0_CCRn()             // Retorno especial (RETI)
{
    switch (TA0IV) {

        case TA0IV_TA0CCR1:    //CCR1
            P1OUT ^= BIT0;
            break;

        case TA0IV_TA0CCR2:    //CCR2
            P4OUT ^= BIT7;
            break;

        default:
            break;
    }
}


