#pragma once
#include <Arduino.h>

#include "VirtEncButton.h"
#include "io.h"

// ===================== CLASS =====================
class EncButton : public VirtEncButton {
   public:
    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    EncButton(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(encA, encB, btn, modeEnc, modeBtn, btnLevel);
    }

    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        e0 = encA;
        e1 = encB;
        b = btn;
        EB_mode(e0, modeEnc);
        EB_mode(e1, modeEnc);
        EB_mode(b, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(EB_read(e0), EB_read(e1));
    }

    // ====================== TICK ======================
    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncButton::tickISR(EB_read(e0), EB_read(e1));
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return ef.read(EB_EISR) ? VirtEncButton::tick(EB_read(b)) : VirtEncButton::tick(EB_read(e0), EB_read(e1), EB_read(b));
    }

    // функция обработки без сброса событий
    bool tickRaw() {
        return ef.read(EB_EISR) ? VirtEncButton::tickRaw(EB_read(b)) : VirtEncButton::tickRaw(EB_read(e0), EB_read(e1), EB_read(b));
    }

    // ====================== READ ======================
    // прочитать значение кнопки
    bool readBtn() {
        return EB_read(b) ^ bf.read(EB_INV);
    }

    // ===================== PRIVATE =====================
   private:
    uint8_t e0, e1, b;
};

// ===================== T CLASS =====================
template <uint8_t ENCA, uint8_t ENCB, uint8_t BTN>
class EncButtonT : public VirtEncButton {
   public:
    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    EncButtonT(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(modeEnc, modeBtn, btnLevel);
    }

    // настроить пины (pinmode энк, pinmode кнопка)
    void init(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        EB_mode(ENCA, modeEnc);
        EB_mode(ENCB, modeEnc);
        EB_mode(BTN, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(EB_read(ENCA), EB_read(ENCB));
    }

    // ====================== TICK ======================
    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncButton::tickISR(EB_read(ENCA), EB_read(ENCB));
    }

    // функция обработки, вызывать в loop
    bool tick() {
        return ef.read(EB_EISR) ? VirtEncButton::tick(EB_read(BTN)) : VirtEncButton::tick(EB_read(ENCA), EB_read(ENCB), EB_read(BTN));
    }

    // функция обработки без сброса событий
    bool tickRaw() {
        return ef.read(EB_EISR) ? VirtEncButton::tickRaw(EB_read(BTN)) : VirtEncButton::tickRaw(EB_read(ENCA), EB_read(ENCB), EB_read(BTN));
    }

    // ====================== READ ======================
    // прочитать значение кнопки
    bool readBtn() {
        return EB_read(BTN) ^ bf.read(EB_INV);
    }
};