#pragma once
#include <Arduino.h>

#include "VirtEncoder.h"
#include "io.h"

// ============= VAR PIN =============
class Encoder : public VirtEncoder {
   public:
    // указать пины и их режим работы
    Encoder(uint8_t encA = 0, uint8_t encB = 0, uint8_t mode = INPUT) {
        init(encA, encB, mode);
    }

    // указать пины и их режим работы
    void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t mode = INPUT) {
        e0 = encA;
        e1 = encB;
        EB_mode(e0, mode);
        EB_mode(e1, mode);
        initEnc(EB_read(e0), EB_read(e1));
    }

    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncoder::tickISR(EB_read(e0), EB_read(e1));
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        return ef.read(EB_EISR) ? VirtEncoder::tick() : VirtEncoder::tick(EB_read(e0), EB_read(e1));
    }

    // обработка без сброса события поворота
    int8_t tickRaw() {
        return ef.read(EB_EISR) ? VirtEncoder::tickRaw() : VirtEncoder::tickRaw(EB_read(e0), EB_read(e1));
    }

   private:
    uint8_t e0, e1;
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
        EB_mode(ENCA, mode);
        EB_mode(ENCB, mode);
        initEnc(EB_read(ENCA), EB_read(ENCB));
    }

    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncoder::tickISR(EB_read(ENCA), EB_read(ENCB));
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        return ef.read(EB_EISR) ? VirtEncoder::tick() : VirtEncoder::tick(EB_read(ENCA), EB_read(ENCB));
    }

    // обработка без сброса события поворота
    int8_t tickRaw() {
        return ef.read(EB_EISR) ? VirtEncoder::tickRaw() : VirtEncoder::tickRaw(EB_read(ENCA), EB_read(ENCB));
    }
};