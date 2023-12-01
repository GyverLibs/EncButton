#include "utils.h"

bool EBread(const uint8_t pin)
{
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    if (pin < 8)
        return bitRead(PIND, pin);
    else if (pin < 14)
        return bitRead(PINB, pin - 8);
    else if (pin < 20)
        return bitRead(PINC, pin - 14);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
    return bitRead(PINB, pin);
#if defined(__AVR_ATmega2560__)
    if (pin < 8)
      bitValue = bitRead(PIND, pin);
    else if (pin < 14)
      bitValue = bitRead(PINE, pin - 8);
    else if (pin < 22)
      bitValue = bitRead(PING, pin - 14);
    else if (pin < 30)
      bitValue = bitRead(PINH, pin - 22);
    else if (pin < 38)
      bitValue = bitRead(PINJ, pin - 30);
    else if (pin < 46)
      bitValue = bitRead(PINK, pin - 38);
    else if (pin < 54)
      bitValue = bitRead(PINL, pin - 46);
#else
    return digitalRead(pin);
#endif
    return 0;
}