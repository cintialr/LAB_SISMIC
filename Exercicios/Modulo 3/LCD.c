#include "LCD.h"

void i2cConfig(){
    UCB0CTL1 |= UCSWRST;                    // reset = 1
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;   // MESTRE
    UCB0CTL1 |= UCSSEL__SMCLK;              // clock SMCLK
    UCB0BRW = 100;                          // 100kHz


    // configura��o dos pinos
    // LEDs
    P1OUT &= ~BIT0;                         // Configura o LED vermelho
    P1DIR |=  BIT0;                         // Sa�da por padr�o em 0
    P4OUT &= ~BIT7;                         // Configura o LED verde
    P4DIR |=  BIT7;                         // Sa�da por padr�o em 0

    // B0 = MESTRE
    P3SEL |= BIT1;
    P3DIR &= ~BIT1;                         // Configura
    P3REN |=  BIT1;                         // Entrada com resistor
    P3OUT |=  BIT1;                         // de pull-up

    P3SEL |= BIT0;
    P3DIR &= ~BIT0;                         // Configura
    P3REN |=  BIT0;                         // Entrada com resistor
    P3OUT |=  BIT0;                         // de pull-up

    // B1 = ESCRAVO
    P4SEL |= BIT2;
    P4DIR &= ~BIT2;
    P4REN |=  BIT2;                         // Entrada com resistor
    P4OUT |=  BIT2;                         // de pull-up

    P4SEL |= BIT1;
    P4DIR &= ~BIT1;
    P4REN |=  BIT1;                         // Entrada com resistor
    P4OUT |=  BIT1;                         // de pull-up

    UCB0CTL1 &= ~UCSWRST;                   // reset = 0

}

uint8_t i2cSend(uint8_t addr, uint8_t * data, uint8_t nBytes)
{
     UCB1IFG = 0;                           // zera o registro de flags antes
     UCB0I2CSA = addr;                      // recebe o endere�o a ser testado, no caso SLAVE
     UCB0CTL1 |= UCTR;                      // transmissor = 1
     UCB0CTL1 |= UCTXSTT;                   //start = 1

     while( !(UCB0IFG & UCTXIFG));

     UCB0TXBUF = *data++;
    nBytes--;

      while (UCB0CTL1 & UCTXSTT);

   //   UCB0CTL1 |= UCTXSTP;

      if (UCB1IFG & UCNACKIFG)
      {
          UCB0CTL1 |= UCTXSTP;
          while(UCB0CTL1 & UCTXSTP);
          return 1;
      }
    while(nBytes--){
      while(!(UCB1IFG & UCTXIFG));
        UCB1TXBUF = *data++;
    }
    while(!(UCB1IFG & UCTXIFG));
      UCB0CTL1 |= UCTXSTP;

      while(UCB0CTL1 & UCTXSTP);

      return 0;
}

uint8_t i2cWriteByte(uint8_t addr, uint8_t byte){
    return i2cSend(addr, &byte, 1);
}

// nibble =  agrupamento de 4 bits
// 0x2
void lcdWriteNibble(uint8_t nibble, uint8_t isChar){

    nibble <<= 4;   //  | BT | EN | RW |  RS
    i2cSend(LCD, nibble | BT | 0  |  0 | isChar);
    i2cSend(LCD, nibble | BT | EN |  0 | isChar);
    i2cSend(LCD, nibble | BT | 0  |  0 | isChar);

}

void lcdWriteByte(uint8_t byte, uint8_t isChar){
    lcdWriteNibble(byte >> 4  , isChar);        // manda primeiro MSNibble
    lcdWriteNibble(byte & 0x0F, isChar);        // LSNibble
}

void lcdPrint(uint8_t * string){
    while(*string){
        if (*string == '\n')
        {
            line ^= BIT6;
            lcdWriteByte( BIT7 | line, INSTR );
        }else   
            lcdWriteByte(*string++, CHAR);

        string++;
    }

}

void lcdInit(){

    lcdWriteNibble(0x3, INSTR);
    lcdWriteNibble(0x3, INSTR);         // garante que o lcd em 8 bis
    lcdWriteNibble(0x3, INSTR);         // antes de entrar no modo 4 bits

    lcdWriteNibble(0x2, INSTR);         // modo de 4 bits

    lcdWriteByte(0x06, INSTR);         // configura o LCD
    lcdWriteByte(0x0F, INSTR);         // liga o display, cursor
    lcdWriteByte(0x14, INSTR);         // cursor anda para a direita
    lcdWriteByte(0x28, INSTR);         // modo 4 bits, 2 linhas

    lcdWriteByte(0x01, INSTR);         // limpa o LCD

}

