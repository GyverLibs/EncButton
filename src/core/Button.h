#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "utils.h"

// ============= VAR PIN =============

class Button : public VirtButton {
   public:
    // Button(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        // init(npin, mode, btnLevel);
    // }

    Button(uint8_t npin) : mode(INPUT_PULLUP), btnLevel(LOW) {
        init(npin, mode, btnLevel);
    }

    void init(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pin = npin;
        pinMode(pin, mode);
        setBtnLevel(btnLevel);
    }

    /**
     * Reads the raw pin state for this button instance.
     * Performs an XOR against the configured active level
     * to return the logical button state.
     */
    bool read()
    {
        return EBread(pin) ^ read_bf(EB_INV);
    }

    /**
     * Reads the button state and updates the internal state machine.
     * Calls the parent VirtButton::tick() method to update
     * the debounced/held state based on the latest raw pin reading.
     */
    bool tick()  
    {
        return VirtButton::tick(EBread(pin));
    }

    bool tickRaw() {
        return VirtButton::tickRaw(EBread(pin));
    }

   private:
    uint8_t pin;
    uint8_t mode;
    uint8_t btnLevel;
};

// ============= TEMPLATE PIN =============

template <uint8_t PIN>
class ButtonT : public VirtButton {
   public:
    ButtonT(uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(mode, btnLevel);
    }

    void init(uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pinMode(PIN, mode);
        setBtnLevel(btnLevel);
    }

    bool read() {
        return EBread(PIN) ^ read_bf(EB_INV);
    }

    bool tick() {
        return VirtButton::tick(EBread(PIN));
    }

    bool tickRaw() {
        return VirtButton::tickRaw(EBread(PIN));
    }

   private:
};