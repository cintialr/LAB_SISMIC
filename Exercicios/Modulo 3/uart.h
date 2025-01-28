#ifndef __UART_H
#define __UART_H

uint16_t count;
uint8_t rxBuff[32], i;


#define MAX_TIMERS  8
#define TX          6,0
#define RX          6,1
#define TB          833,us


typedef enum { us ,ms = 0, sec = 1, min = 60} timeunit_t;

void uartOpen(uint8_t interface);
void uartPrint(char * str);
void wait(uint16_t time, uint16_t uint);
void uartBang(uint8_t byte);

#endif
