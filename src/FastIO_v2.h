// Быстрый IO для AVR (для остальных будет digitalxxxxx)
// v1.0

#ifndef _FastIO_v2_h
#define _FastIO_v2_h
#include <Arduino.h>

bool F_fastRead(const uint8_t pin);				// быстрое чтение пина
void F_fastWrite(const uint8_t pin, bool val);	// быстрая запись
uint8_t F_fastShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder); 				// быстрый shiftIn
void F_fastShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t data);	// быстрый shiftOut

#endif