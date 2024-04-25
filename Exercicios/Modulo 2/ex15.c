#include <msp430.h>

#define false 0
#define aberto 1
#define fech 0
#define T20ms 20972 // 20 ms (1.048.576 x 0,02)
#define T500us 524

void config_servo(void);
void config_pinos(void);
int sw1(void);
int sw2(void);
void debounce(void);

int ps1=aberto,ps2=aberto;
volatile int carga=10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    config_servo();
    config_pinos();

 while(1){
     if (sw2()) carga++;
     if (sw1()) carga--;
     if (carga > 20) carga=20;
     if (carga < 0) carga=0;

     TA2CCR2=T500us+carga*105;
     }
     return 0;
}


void config_servo(void){
    TA2CTL = TASSEL_2|ID_0|MC_1|TACLR;
    TA2EX0 = TAIDEX_0;
    TA2CCR0 = T20ms; //Período de 20 ms
    TA2CCTL2 = OUTMOD_6;
    TA2CCR2 = T500us+10*105; // Inicia em 1,5 ms

    P2DIR |= BIT5;  //P2.5 Saida

    P2SEL |= BIT5; //P2.5 PWM
}

int sw1(void){
    if ((P2IN&BIT1) == 0){
        if (ps1==aberto){
            debounce();
            ps1=fech;
            return 1;
        }
        else
            return false;
    }
    else{ //S1 aberta
        if (ps1==aberto)
            return false;
        else{
            debounce();
            ps1=aberto;
            return false;
            }
          }
     }

int sw2(void){
    if ((P1IN&BIT1) == 0){
        if (ps2==aberto){
            debounce();
            ps2=fech;
            return 1;
      }
      else
          return false;
   }
    else{
        if (ps2==aberto)
            return false;
        else{
            debounce();
            ps2=aberto;
            return false;
      }
      }
     }

void config_pinos(void){

      P2DIR &= ~BIT1; // entrada
      P2REN |= BIT1; //habilta resistor
      P2OUT |= BIT1; //Switch 1 pull-up
      P1DIR &= ~BIT1; //  entrada
      P1REN |= BIT1; //habilita resistor
      P1OUT |= BIT1; //Switch 2 pull-up
     }

void debounce(void){
      volatile unsigned int i;
      for (i=10000; i>0; i--);
     }
