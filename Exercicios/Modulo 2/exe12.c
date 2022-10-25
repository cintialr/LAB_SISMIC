/* exercicio 12 - PWM com duty cycle fixo */
#include <msp430.h> 
#include <stdint.h>

#define PWM_PERIOD  1000

void main(){
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

	// CCR0 (teto de contagem) --> Período
	TA0CCR0 = PWM_PERIOD - 1;               // Período

	// CCR1 --> Duty cycle (PWM): 50%
	TA0CCR1 = PWM_PERIOD/2 - 1;             // Duty Cycle

	// Loop infinito
	while(1){
        // Trava de execução de CCR1
        while(!(TA0CCTL1 & CCIFG));
        TA0CCTL1 &= ~CCIFG;

        P1OUT &= ~BIT0;                     // Ação do CCR1 -> RESET

	    // Trava de execução de CCR0
        while(!(TA0CCTL0 & CCIFG));
        TA0CCTL0 &= ~CCIFG;

        P1OUT |=  BIT0;                     // Ação do CCR0 -> SET
  }
}

