/**
 * @file Button.h
 * @brief Button class for handling button presses and debouncing.
 */

#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "utils.h"

// ============= VAR PIN =============

/**
 * @brief Represents a button that extends the VirtButton class.
 * 
 * The Button class provides functionality to read the state of a button,
 * update the internal state machine, and perform initialization.
 */
class Button : public VirtButton {
   public:
    /**
     * @brief Constructs a Button object with the specified pin number.
     * 
     * @param npin The pin number to which the button is connected.
     */
    Button(uint8_t npin);

    /**
     * @brief Initializes the button with the specified pin, mode, and button level.
     * 
     * @param npin The pin number to which the button is connected.
     * @param mode The mode of the pin (INPUT, INPUT_PULLUP, etc.).
     * @param btnLevel The active level of the button (LOW or HIGH).
     */
    void init(uint8_t npin = 0, uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW);

    /**
     * @brief Reads the raw pin state for this button instance.
     * 
     * Performs an XOR against the configured active level
     * to return the logical button state.
     * 
     * @return The logical state of the button (true for pressed, false for released).
     */
    bool read();

    /**
     * @brief Reads the button state and updates the internal state machine.
     * 
     * Calls the parent VirtButton::tick() method to update
     * the debounced/held state based on the latest raw pin reading.
     * 
     * @return The updated state of the button (true for pressed, false for released).
     */
    bool tick();

    /**
     * @brief Reads the raw pin state for this button instance and updates the internal state machine.
     * 
     * Calls the parent VirtButton::tickRaw() method to update
     * the debounced/held state based on the latest raw pin reading.
     * 
     * @return The updated state of the button (true for pressed, false for released).
     */
    bool tickRaw();

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

    void init(uint8_t mode = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        pinMode(PIN, mode);
        setBtnLevel(btnLevel);
    }

    bool read() {
        return EBread(PIN) ^ read_btn_flag(B_INV);
    }

    bool tick() {
        return VirtButton::tick(EBread(PIN));
    }

    bool tickRaw() {
        return VirtButton::tickRaw(EBread(PIN));
    }

   private:
};