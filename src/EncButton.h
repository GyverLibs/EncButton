#pragma once
#include <Arduino.h>

#include "core/Button.h"
#include "core/Encoder.h"

#include "core/VirtEncButton.h"
#include "core/utils.h"

// ===================== CLASS =====================
class EncButton : public VirtEncButton {
   public:
    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    EncButton(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP) {
        init(encA, encB, btn, modeEnc, modeBtn);
        setButtonLevel(LOW);
    }

    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP) {
        e0 = encA;
        e1 = encB;
        b = btn;
        pinMode(e0, modeEnc);
        pinMode(e1, modeEnc);
        pinMode(b, modeBtn);
        initEnc(readEnc());
    }

    // ====================== TICK ======================
    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncButton::tickISR(readEnc());
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        return VirtEncButton::tick(readEnc(), EBread(b));
    }

    // ====================== READ ======================
    // прочитать значение кнопки
    bool readBtn() {
        return EBread(b) ^ read_bf(EB_INV);
    }

    // прочитать значение энкодера
    int8_t readEnc() {
        return EBread(e0) | (EBread(e1) << 1);
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
    EncButtonT(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP) {
        init(modeEnc, modeBtn);
        setButtonLevel(LOW);
    }

    // настроить пины (pinmode энк, pinmode кнопка)
    void init(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP) {
        pinMode(ENCA, modeEnc);
        pinMode(ENCB, modeEnc);
        pinMode(BTN, modeBtn);
        initEnc(readEnc());
    }

    // ====================== TICK ======================
    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncButton::tickISR(readEnc());
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        return VirtEncButton::tick(readEnc(), EBread(BTN));
    }

    // ====================== READ ======================
    // прочитать значение кнопки
    bool readBtn() {
        return EBread(BTN) ^ read_bf(EB_INV);
    }

    // прочитать значение энкодера
    int8_t readEnc() {
        return EBread(ENCA) | (EBread(ENCB) << 1);
    }

    // ===================== PRIVATE =====================
   private:
};