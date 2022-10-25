/* exercicio 13 - PWM por hardware */
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

	// Configura P1.5 como saída do timer --> TA0.4
	P1DIR |= BIT5;
	P1SEL |= BIT5;

	// Configura o canal 4 do timer como saída do waveform generator
	TA0CCTL4 = OUTMOD_7;

    // CCR4 --> Duty cycle (PWM): 50%
    TA0CCR4 = PWM_PERIOD/2 - 1;             // Duty Cycle

    // ----------------
    TA1CTL = TASSEL__ACLK | MC__UP | ID__4;
    TA1CCR0 = 0x8000/8 - 1;

    enum {up, down} modo = up;

	// Loop infinito
	while(1)
	{

	    while(!(TA1CCTL0 & CCIFG));
	    TA1CCTL0 & CCIFG;

	    if(modo == up)
	    {
	        TA0CCR4 += PWM_PERIOD/8;

	        if(TA0CCR4 == PWM_PERIOD)
	            modo = down;
	    }
	    if(modo == down)
	    {
            TA0CCR4 -= PWM_PERIOD/8;

            if(TA0CCR4 <= PWM_PERIOD/8)
                modo = up;
	    }

	    // A PWM é gerada automaticamente pelo timer

	}

}



