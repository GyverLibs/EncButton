#include "utils.h"

bool EBread(const uint8_t pin){
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    if (pin < 8)
        return bitRead(PIND, pin);
    else if (pin < 14)
        return bitRead(PINB, pin - 8);
    else if (pin < 20)
        return bitRead(PINC, pin - 14);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
    return bitRead(PINB, pin);
#elif defined(__AVR_ATmega2560__)
    if (pin < 8)
      return bitRead(PIND, pin);
    else if (pin < 14)
      return bitRead(PINE, pin - 8);
    else if (pin < 22)
      return bitRead(PING, pin - 14);
    else if (pin < 30)
      return bitRead(PINH, pin - 22);
    else if (pin < 38)
      return bitRead(PINJ, pin - 30);
    else if (pin < 46)
      return bitRead(PINK, pin - 38);
    else if (pin < 54)
      return bitRead(PINL, pin - 46);
#else
    return digitalRead(pin);
#endif
    return 0;
}

void EBwrite(const uint8_t pin, bool value){
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    if (pin < 8)
        bitWrite(PORTD, pin, value);
    else if (pin < 14)
        bitWrite(PORTB, pin - 8, value);
    else if (pin < 20)
        bitWrite(PORTC, pin - 14, value);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
    bitWrite(PORTB, pin, value);
#elif defined(__AVR_ATmega2560__)
    if (pin < 8)
        bitWrite(PORTD, pin, value);
    else if (pin < 14)
        bitWrite(PORTE, pin - 8, value);
    else if (pin < 22)
        bitWrite(PORTG, pin - 14, value);
    else if (pin < 30)
        bitWrite(PORTH, pin - 22, value);
    else if (pin < 38)
        bitWrite(PORTJ, pin - 30, value);
    else if (pin < 46)
        bitWrite(PORTK, pin - 38, value);
    else if (pin < 54)
        bitWrite(PORTL, pin - 46, value);
#else
    digitalWrite(pin, value);
#endif
}