#pragma once
#include <Arduino.h>

#include "VirtEncoder.h"
#include "utils.h"

// ============= VAR PIN =============
class Encoder : public VirtEncoder {
   public:
    // указать пины и их режим работы
    Encoder(uint8_t npina = 0, uint8_t npinb = 0, uint8_t mode = INPUT) {
        init(npina, npinb, mode);
    }

    // указать пины и их режим работы
    void init(uint8_t npina = 0, uint8_t npinb = 0, uint8_t mode = INPUT) {
        encA = npina;
        encB = npinb;
        pinMode(encA, mode);
        pinMode(encB, mode);
    }

    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncoder::tickISR(EBread(encA), EBread(encB));
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        return VirtEncoder::tick(EBread(encA), EBread(encB));
    }

   private:
    uint8_t encA, encB;
};

// ============= TEMPLATE PIN =============
template <uint8_t ENCA, uint8_t ENCB>
class EncoderT : public VirtEncoder {
   public:
    // указать режим работы пинов
    EncoderT(uint8_t mode = INPUT) {
        init(mode);
    }

    // указать режим работы пинов
    void init(uint8_t mode = INPUT) {
        pinMode(ENCA, mode);
        pinMode(ENCB, mode);
    }
    
    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncoder::tickISR(EBread(ENCA), EBread(ENCB));
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        return VirtEncoder::tick(EBread(ENCA), EBread(ENCB));
    }

   private:
};