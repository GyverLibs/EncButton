#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "utils.h"

// ============= VAR PIN =============
class Button : public VirtButton {
   public:
    Button(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP) {
        init(npin, mode);
        setButtonLevel(LOW);
    }

    // указать пин и его режим работы
    void init(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP) {
        pin = npin;
        pinMode(pin, mode);
    }

    // прочитать текущее значение кнопки (без дебаунса)
    bool read() {
        return EBread(pin) ^ read_bf(EB_INV);
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return VirtButton::tick(EBread(pin));
    }

   private:
    uint8_t pin;
};

// ============= TEMPLATE PIN =============
template <uint8_t PIN>
class ButtonT : public VirtButton {
   public:
    ButtonT(uint8_t mode = INPUT_PULLUP) {
        init(mode);
        setButtonLevel(LOW);
    }

    // указать режим работы пина
    void init(uint8_t mode = INPUT_PULLUP) {
        pinMode(PIN, mode);
    }

    // прочитать текущее значение кнопки (без дебаунса)
    bool read() {
        return EBread(PIN) ^ read_bf(EB_INV);
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return VirtButton::tick(EBread(PIN));
    }

   private:
};