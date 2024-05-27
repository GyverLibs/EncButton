#pragma once
#include <Arduino.h>
#include <GyverIO.h>

bool EB_read(uint8_t pin);
void EB_mode(uint8_t pin, uint8_t mode);
uint32_t EB_uptime();