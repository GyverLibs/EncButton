#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "utils.h"

// ============= VAR PIN =============
class Button : public VirtButton {
   public:
    Button(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(npin, mode, btnLevel);
    }

    // указать пин и его режим работы
    void init(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pin = npin;
        pinMode(pin, mode);
        setBtnLevel(btnLevel);
    }

    // прочитать текущее значение кнопки (без дебаунса)
    bool read() {
        return EBread(pin) ^ read_bf(EB_INV);
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return VirtButton::tick(EBread(pin));
    }

    // обработка кнопки без сброса событий и вызова коллбэка
    bool tickRaw() {
        return VirtButton::tickRaw(EBread(pin));
    }

   private:
    uint8_t pin;
};

// ============= TEMPLATE PIN =============
template <uint8_t PIN>
class ButtonT : public VirtButton {
   public:
    ButtonT(uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(mode, btnLevel);
    }

    // указать режим работы пина
    void init(uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pinMode(PIN, mode);
        setBtnLevel(btnLevel);
    }

    // прочитать текущее значение кнопки (без дебаунса)
    bool read() {
        return EBread(PIN) ^ read_bf(EB_INV);
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return VirtButton::tick(EBread(PIN));
    }

    // обработка кнопки без сброса событий и вызова коллбэка
    bool tickRaw() {
        return VirtButton::tickRaw(EBread(PIN));
    }

   private:
};