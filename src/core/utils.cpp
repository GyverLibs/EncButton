#include "utils.h"

bool EBread(const uint8_t pin) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    if (pin < 8) return bitRead(PIND, pin);
    else if (pin < 14) return bitRead(PINB, pin - 8);
    else if (pin < 20) return bitRead(PINC, pin - 14);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
    return bitRead(PINB, pin);
#else
    return digitalRead(pin);
#endif
    return 0;
}