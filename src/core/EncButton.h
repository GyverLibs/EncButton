/**
 * @file EncButton.h
 * @brief EncButton class for handling rotary encoder with button 
 */

#pragma once
#include <Arduino.h>

#include "VirtEncButton.h"
#include "utils.h"

// ===================== CLASS =====================
/**
 * @class EncButton
 * @brief A class that represents an encoder button.
 * 
 * This class extends the VirtEncButton class and provides additional functionality for handling an encoder button.
 * It allows initializing the encoder pins, button pin, and button mode. It also provides methods for reading the button state and encoder value.
 */
class EncButton : public VirtEncButton {
   public:
    /**
     * @brief Constructs an EncButton object with default pin and mode values.
     * 
     * @param encA The pin number for encoder A.
     * @param encB The pin number for encoder B.
     * @param btn The pin number for the button.
     * @param modeEnc The mode for the encoder pins.
     * @param modeBtn The mode for the button pin.
     * @param btnLevel The logic level for the button pin when pressed.
     */
    EncButton(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(encA, encB, btn, modeEnc, modeBtn, btnLevel);
    }

    /**
     * @brief Initializes the EncButton object with the specified pin and mode values.
     * 
     * @param encA The pin number for encoder A.
     * @param encB The pin number for encoder B.
     * @param btn The pin number for the button.
     * @param modeEnc The mode for the encoder pins.
     * @param modeBtn The mode for the button pin.
     * @param btnLevel The logic level for the button pin when pressed.
     */
    void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        e0 = encA;
        e1 = encB;
        b = btn;
        pinMode(e0, modeEnc);
        pinMode(e1, modeEnc);
        pinMode(b, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(readEnc());
    }

    // ====================== TICK ======================

    /**
     * @brief Performs a tick operation on the EncButton object.
     * 
     * @return The change in encoder value.
     */
    int8_t tickISR() {
        return VirtEncButton::tickISR(readEnc());
    }

    /**
     * @brief Performs a tick operation on the EncButton object.
     * 
     * @return True if the button state changed, false otherwise.
     */
    bool tick() {
        if (read_encf(E_ISR))
            return VirtEncButton::tick(EBread(b));
        else
            return VirtEncButton::tick(readEnc(), EBread(b));
    }

    /**
     * @brief Performs a raw tick operation on the EncButton object.
     * 
     * @return True if the button state changed, false otherwise.
     */
    bool tickRaw() {
        if (read_encf(E_ISR))
            return VirtEncButton::tickRaw(EBread(b));
        else
            return VirtEncButton::tickRaw(readEnc(), EBread(b));
    }

    // ====================== READ ======================

    /**
     * @brief Reads the state of the button.
     * 
     * @return True if the button is pressed, false otherwise.
     */
    bool readBtn() {
        return EBread(b) ^ read_btn_flag(B_INV);
    }

    /**
     * @brief Reads the value of the encoder.
     * 
     * @return The current encoder value.
     */
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
    EncButtonT(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(modeEnc, modeBtn, btnLevel);
    }

    void init(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pinMode(ENCA, modeEnc);
        pinMode(ENCB, modeEnc);
        pinMode(BTN, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(readEnc());
    }

    // ====================== TICK ======================
    int8_t tickISR() {
        return VirtEncButton::tickISR(readEnc());
    }

    bool tick() {
        if (read_encf(E_ISR)) return VirtEncButton::tick(EBread(BTN));
        else return VirtEncButton::tick(readEnc(), EBread(BTN));
    }

    bool tickRaw() {
        if (read_encf(E_ISR)) return VirtEncButton::tickRaw(EBread(BTN));
        else return VirtEncButton::tickRaw(readEnc(), EBread(BTN));
    }

    // ====================== READ ======================
    bool readBtn() {
        return EBread(BTN) ^ read_btn_flag(B_INV);
    }

    int8_t readEnc() {
        return EBread(ENCA) | (EBread(ENCB) << 1);
    }

    // ===================== PRIVATE =====================
   private:
};