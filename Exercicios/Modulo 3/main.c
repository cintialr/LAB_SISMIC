#include "LCD.h"


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    i2cConfig();
    lcdInit();

    lcdPrint("Hello World!");

    /*while(1){
        i2cSend(0x27, 0);
        __delay_cycles(1000000);
        i2cSend(0x27, BIT3);
        __delay_cycles(1000000);
    }*/
}





#pragma vector = USCI_B1_VECTOR
__interrupt void slave() {
  switch (UCB1IV) {
    case USCI_I2C_UCRXIFG:
      data = UCB0RXBUF;
      break;
    case USCI_I2C_UCTXIFG:
      UCB0TXBUF = data;
      break;
    default:
      break;
  }
}
