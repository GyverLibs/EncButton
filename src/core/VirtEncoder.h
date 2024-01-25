#pragma once
#include <Arduino.h>

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
        prev = ecount = 0;
    }
    // ====================== SET ======================
    // инвертировать направление энкодера
    void setEncReverse(const bool& rev) {
        if (rev) set_ef(EB_REV);
        else clr_ef(EB_REV);
    }

    // установить тип энкодера (EB_STEP4_LOW, EB_STEP4_HIGH, EB_STEP2, EB_STEP1)
    void setEncType(const uint8_t& type) {
        flags = (flags & 0b11111100) | type;
    }

    // использовать обработку энкодера в прерывании
    void setEncISR(const bool& use) {
        write_ef(EB_EISR, use);
    }

    // инициализация энкодера
    void initEnc(const bool& e0, const bool& e1) {
        initEnc(e0 | (e1 << 1));
    }

    // инициализация энкодера совмещённым значением
    void initEnc(const int8_t& v) {
        prev = v;
    }

    // сбросить флаги событий
    void clear() {
        if (read_ef(EB_ETRN_R)) clr_ef(EB_ETRN_R);
    }

    // ====================== ОПРОС ======================
    // был поворот [событие]
    bool turn() {
        return read_ef(EB_ETRN_R);
    }

    // направление энкодера (1 или -1) [состояние]
    int8_t dir() {
        return read_ef(EB_DIR) ? 1 : -1;
    }

    // ====================== POLL ======================
    // ISR
    // опросить энкодер в прерывании. Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t tickISR(const bool& e0, const bool& e1) {
        return tickISR(e0 | (e1 << 1));
    }

    // опросить энкодер в прерывании. Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t tickISR(int8_t state) {
        state = pollEnc(state);
        if (state) {
            set_ef(EB_ISR_F);
            write_ef(EB_DIR, state > 0);
        }
        return state;
    }

    // TICK
    // опросить энкодер. Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t tick(const bool& e0, const bool& e1) {
        return tick(e0 | (e1 << 1));
    }

    // опросить энкодер. Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t tick(int8_t state) {
        state = tickRaw(state);
        if (state) return state;
        clear();
        return 0;
    }

    // опросить энкодер (сам опрос в прерывании)
    int8_t tick() {
        return tick(-1);
    }

    // RAW
    // опросить энкодер без сброса события поворота
    int8_t tickRaw(const bool& e0, const bool& e1) {
        return tickRaw(e0 | (e1 << 1));
    }

    // опросить энкодер без сброса события поворота
    int8_t tickRaw(int8_t state) {
        if (read_ef(EB_ISR_F)) {
            clr_ef(EB_ISR_F);
            set_ef(EB_ETRN_R);
            return dir();
        }
        if ((state >= 0) && (state = pollEnc(state))) {
            write_ef(EB_DIR, state > 0);
            set_ef(EB_ETRN_R);
            return state;
        }
        return 0;
    }

    // опросить энкодер без сброса события поворота (сам опрос в прерывании)
    int8_t tickRaw() {
        return tickRaw(-1);
    }

    // POLL
    // опросить энкодер без установки события поворота (быстрее). Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t pollEnc(const bool& e0, const bool& e1) {
        return pollEnc(e0 | (e1 << 1));
    }

    // опросить энкодер без установки события поворота (быстрее). Вернёт 1 или -1 при вращении, 0 при остановке
    int8_t pollEnc(int8_t state) {
        if (prev != state) {
            ecount += ((0x49941661 >> ((state | (prev << 2)) << 1)) & 0b11) - 1;
            prev = state;
            if (!ecount) return 0;
            switch (flags & 0b11) {
                case EB_STEP4_LOW:
                    if (state != 0b11) return 0;  // skip 00, 01, 10
                    break;
                case EB_STEP4_HIGH:
                    if (state) return 0;  // skip 01, 10, 11
                    break;
                case EB_STEP2:
                    if (state == 0b10 || state == 0b01) return 0;  // skip 10 01
                    break;
            }
            state = ((ecount > 0) ^ read_ef(EB_REV)) ? 1 : -1;
            ecount = 0;
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
    inline void set_ef(const uint16_t& x) __attribute__((always_inline)) {
        flags |= x;
    }
    inline void clr_ef(const uint16_t& x) __attribute__((always_inline)) {
        flags &= ~x;
    }
    inline void write_ef(const uint16_t& x, const bool& v) __attribute__((always_inline)) {
        if (v) set_ef(x);
        else clr_ef(x);
    }
    inline bool read_ef(const uint16_t& x) __attribute__((always_inline)) {
        return flags & x;
    }

   private:
    uint8_t flags = 0;
    int8_t prev : 4;
    int8_t ecount : 4;
};