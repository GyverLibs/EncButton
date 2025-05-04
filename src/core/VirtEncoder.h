#pragma once
#include <Arduino.h>

#include "flags.h"
#include "io.h"

// ===================== CONST ======================
#define EB_STEP4_LOW 0
#define EB_STEP4_HIGH 1
#define EB_STEP2 2
#define EB_STEP1 3

// ===================== FLAGS ======================
#define EB_TYPE (1 << 0)
#define EB_REV (1 << 2)
#define EB_FAST (1 << 3)
#define EB_DIR (1 << 4)
#define EB_ETRN_R (1 << 5)
#define EB_ISR_F (1 << 6)
#define EB_EISR (1 << 7)

// базовый класс энкодера
class VirtEncoder {
   public:
    VirtEncoder() {
        p0 = p1 = epos = 0;
    }

    // ====================== SET ======================
    // инвертировать направление энкодера
    void setEncReverse(const bool rev) {
        rev ? ef.set(EB_REV) : ef.clear(EB_REV);
    }

    // установить тип энкодера (EB_STEP4_LOW, EB_STEP4_HIGH, EB_STEP2, EB_STEP1)
    void setEncType(const uint8_t type) {
        ef.flags = (ef.flags & 0b11111100) | type;
    }

    // использовать обработку энкодера в прерывании
    void setEncISR(const bool use) {
        ef.write(EB_EISR, use);
    }

    // инициализация энкодера
    void initEnc(const bool e0, const bool e1) {
        p0 = e0, p1 = e1;
    }

    // сбросить флаги событий
    void clear() {
        if (ef.read(EB_ETRN_R)) ef.clear(EB_ETRN_R);
    }

    // ====================== ОПРОС ======================
    // был поворот [событие]
    bool turn() {
        return ef.read(EB_ETRN_R);
    }

    // направление энкодера (1 или -1) [состояние]
    int8_t dir() {
        return ef.read(EB_DIR) ? 1 : -1;
    }

    // ====================== POLL ======================
    // ISR
    // опросить энкодер в прерывании. Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t tickISR(const bool e0, const bool e1) {
        int8_t state = pollEnc(e0, e1);
        if (state) {
            ef.set(EB_ISR_F);
            ef.write(EB_DIR, state > 0);
        }
        return state;
    }

    // TICK
    // опросить энкодер. Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t tick(const bool e0, const bool e1) {
        int8_t state = tickRaw(e0, e1);
        if (!state) clear();
        return state;
    }

    // опросить энкодер (сам опрос в прерывании)
    int8_t tick() {
        int8_t state = tickRaw();
        if (!state) clear();
        return state;
    }

    // RAW
    // опросить энкодер без сброса события поворота
    int8_t tickRaw(const bool e0, const bool e1) {
        int8_t state = tickRaw();
        if (state) return state;

        state = pollEnc(e0, e1);
        if (state) {
            ef.write(EB_DIR, state > 0);
            ef.set(EB_ETRN_R);
        }
        return state;
    }

    // опросить энкодер без сброса события поворота (сам опрос в прерывании)
    int8_t tickRaw() {
        if (ef.read(EB_ISR_F)) {
            ef.clear(EB_ISR_F);
            ef.set(EB_ETRN_R);
            return dir();
        }
        return 0;
    }

    // POLL
    // опросить энкодер без установки события поворота (быстрее). Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t pollEnc(const bool e0, const bool e1) {
        if (p0 ^ p1 ^ e0 ^ e1) {
            (p1 ^ e0) ? ++epos : --epos;
            p0 = e0, p1 = e1;
            if (!epos) return 0;

            switch (ef.mask(0b11)) {
                case EB_STEP4_LOW:
                    if (!(e0 & e1)) return 0;  // skip 01, 10, 00
                    break;
                case EB_STEP4_HIGH:
                    if (e0 | e1) return 0;  // skip 01, 10, 11
                    break;
                case EB_STEP2:
                    if (e0 ^ e1) return 0;  // skip 10 01
                    break;
            }
            int8_t state = ((epos > 0) ^ ef.read(EB_REV)) ? -1 : 1;
            epos = 0;
#ifndef EB_NO_COUNTER
            counter += state;
#endif
            return state;
        }
        return 0;
    }

#ifndef EB_NO_COUNTER
    int32_t counter = 0;
#endif

    // ===================== PRIVATE =====================
   protected:
    encb::Flags<uint8_t> ef;

   private:
    int8_t p0 : 2;
    int8_t p1 : 2;  // signed 2 bit!
    int8_t epos : 4;
};