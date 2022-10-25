/* exercicio 8 - temporização imprecisa */
#include <msp430.h> 
#include <stdint.h>

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
  
	// Loop 
	while(1){
	    uint16_t i;

	    P1OUT ^= BIT0;    // LED vermelho
	    i = 50000;
	    while(i--);
	}
}