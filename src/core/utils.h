#pragma once
#include <Arduino.h>

#if !defined(EB_NO_GYVER_IO) && !defined(EB_CUSTOM_READ)
#include <GyverIO.h>
#endif

bool EB_read(const uint8_t pin);
uint32_t EB_uptime();

// ====================

#ifndef EB_CUSTOM_READ
bool EB_read(const uint8_t pin) {
#ifndef EB_NO_GYVER_IO
    return gio::read(pin);
#else
    return digitalRead(pin);
#endif
}
#endif

#ifndef EB_CUSTOM_UPTIME
uint32_t EB_uptime() {
    return millis();
}
#endif