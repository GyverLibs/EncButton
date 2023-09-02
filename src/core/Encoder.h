#pragma once
#include <Arduino.h>

#include "VirtEncoder.h"
#include "utils.h"

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
        pinMode(e0, mode);
        pinMode(e1, mode);
        initEnc(readEnc());
    }

    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncoder::tickISR(readEnc());
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        if (read_ef(EB_EISR)) return VirtEncoder::tick();
        else return VirtEncoder::tick(readEnc());
    }

    // обработка без сброса события поворота
    int8_t tickRaw() {
        if (read_ef(EB_EISR)) return VirtEncoder::tickRaw();
        else return VirtEncoder::tickRaw(readEnc());
    }

   private:
    uint8_t e0, e1;

    // прочитать значение энкодера
    int8_t readEnc() {
        return EBread(e0) | (EBread(e1) << 1);
    }
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
        initEnc(readEnc());
    }

    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncoder::tickISR(readEnc());
    }

    // функция обработки, вызывать в loop
    int8_t tick() {
        if (read_ef(EB_EISR)) return VirtEncoder::tick();
        else return VirtEncoder::tick(readEnc());
    }

    // обработка без сброса события поворота
    int8_t tickRaw() {
        if (read_ef(EB_EISR)) return VirtEncoder::tickRaw();
        else return VirtEncoder::tickRaw(readEnc());
    }

    // прочитать значение энкодера
    int8_t readEnc() {
        return EBread(ENCA) | (EBread(ENCB) << 1);
    }

   private:
};