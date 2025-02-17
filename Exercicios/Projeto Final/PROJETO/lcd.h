#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <msp430.h>

void i2cConfig();

uint8_t i2cSend(uint8_t addr, uint8_t * data, uint8_t nBytes);

uint8_t lcdAddr();

void lcdWriteNibble(uint8_t nibble, uint8_t isChar);

void lcdWriteByte(uint8_t byte, uint8_t isChar);

void lcdInit();

void lcdPrint(uint8_t * string);

void lcdPrint(uint8_t * string);

void lcdClear();

#endif /* LCD_H_ */
