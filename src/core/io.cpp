#include "io.h"

bool __attribute__((weak)) EB_read(uint8_t pin) {
    return gio::read(pin);
}

void __attribute__((weak)) EB_mode(uint8_t pin, uint8_t mode) {
    gio::init(pin, mode);
}

uint32_t __attribute__((weak)) EB_uptime() {
    return millis();
}