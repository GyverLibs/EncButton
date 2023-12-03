/**
 * @file Encoder.h
 * @brief Encoder class for handling rotary encoder.
 */

#pragma once
#include <Arduino.h>

#include "VirtEncoder.h"
#include "utils.h"

/**
 * @brief The Encoder class represents an encoder device.
 * 
 * This class inherits from the VirtEncoder class and provides additional functionality for initializing and reading the encoder.
 */
class Encoder : public VirtEncoder {
   public:
    /**
     * @brief Constructs an Encoder object with the specified pin numbers and mode.
     * 
     * @param encA The pin number for encoder A.
     * @param encB The pin number for encoder B.
     * @param mode The mode of the pins (INPUT, OUTPUT, INPUT_PULLUP, etc.).
     */
    Encoder(uint8_t encA = 0, uint8_t encB = 0, uint8_t mode = INPUT) {
        init(encA, encB, mode);
    }

    /**
     * @brief Initializes the encoder with the specified pin numbers and mode.
     * 
     * @param encA The pin number for encoder A.
     * @param encB The pin number for encoder B.
     * @param mode The mode of the pins (INPUT, OUTPUT, INPUT_PULLUP, etc.).
     */
    void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t mode = INPUT) {
        e0 = encA;
        e1 = encB;
        pinMode(e0, mode);
        pinMode(e1, mode);
        initEnc(readEnc());
    }

    /**
     * @brief Performs a tick operation on the encoder using an interrupt service routine (ISR).
     * 
     * @return The change in encoder position.
     */
    int8_t tickISR() {
        return VirtEncoder::tickISR(readEnc());
    }

    /**
     * @brief Performs a tick operation on the encoder.
     * 
     * If the encoder has an interrupt flag set, it uses the interrupt-based tick operation. Otherwise, it uses the regular tick operation.
     * 
     * @return The change in encoder position.
     */
    int8_t tick() {
        if (read_encf(E_ISR))
            return VirtEncoder::tick();
        else
            return VirtEncoder::tick(readEnc());
    }

    /**
     * @brief Performs a raw tick operation on the encoder.
     * 
     * If the encoder has an interrupt flag set, it uses the interrupt-based raw tick operation. Otherwise, it uses the regular raw tick operation.
     * 
     * @return The change in encoder position.
     */
    int8_t tickRaw() {
        if (read_encf(E_ISR))
            return VirtEncoder::tickRaw();
        else
            return VirtEncoder::tickRaw(readEnc());
    }

   private:
    uint8_t e0, e1;

    /**
     * @brief Reads the current state of the encoder.
     * 
     * @return The current state of the encoder as a signed 8-bit integer.
     */
    int8_t readEnc() {
        return EBread(e0) | (EBread(e1) << 1);
    }
};

// ============= TEMPLATE PIN =============
template <uint8_t ENCA, uint8_t ENCB>
class EncoderT : public VirtEncoder {
   public:
    EncoderT(uint8_t mode = INPUT) {
        init(mode);
    }

    void init(uint8_t mode = INPUT) {
        pinMode(ENCA, mode);
        pinMode(ENCB, mode);
        initEnc(readEnc());
    }

    int8_t tickISR() {
        return VirtEncoder::tickISR(readEnc());
    }

    int8_t tick() {
        if (read_encf(E_ISR)) return VirtEncoder::tick();
        else return VirtEncoder::tick(readEnc());
    }

    int8_t tickRaw() {
        if (read_encf(E_ISR)) return VirtEncoder::tickRaw();
        else return VirtEncoder::tickRaw(readEnc());
    }

    int8_t readEnc() {
        return EBread(ENCA) | (EBread(ENCB) << 1);
    }

   private:
};