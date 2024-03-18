#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "io.h"

// ============= VAR PIN =============
class Button : public VirtButton {
   public:
    Button(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(npin, mode, btnLevel);
    }

    // указать пин и его режим работы
    void init(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pin = npin;
        EB_mode(pin, mode);
        setBtnLevel(btnLevel);
    }

    // прочитать текущее значение кнопки (без дебаунса)
    bool read() {
        return EB_read(pin) ^ bf.read(EB_INV);
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return VirtButton::tick(EB_read(pin));
    }

    // обработка кнопки без сброса событий и вызова коллбэка
    bool tickRaw() {
        return VirtButton::tickRaw(EB_read(pin));
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
        EB_mode(PIN, mode);
        setBtnLevel(btnLevel);
    }

    // прочитать текущее значение кнопки (без дебаунса)
    bool read() {
        return EB_read(PIN) ^ bf.read(EB_INV);
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return VirtButton::tick(EB_read(PIN));
    }

    // обработка кнопки без сброса событий и вызова коллбэка
    bool tickRaw() {
        return VirtButton::tickRaw(EB_read(PIN));
    }

   private:
};