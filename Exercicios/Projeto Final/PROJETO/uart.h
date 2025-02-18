#ifndef __UART_H
#define __UART_H
#include <msp430.h>
#include <stdint.h>


typedef enum { us ,ms = 0, sec = 1, min = 60} timeunit_t;

void uartOpen(uint8_t interface);
void uartPrint(char * str);
void wait(uint16_t time, uint16_t uint);
void uartBang(uint8_t byte);
uint8_t uartAvailable();
uint8_t uartRead(char * str);


#endif
