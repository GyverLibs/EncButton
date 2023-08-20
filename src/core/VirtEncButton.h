#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "VirtEncoder.h"
#include "utils.h"

// =================== TOUT BUILD ===================
#ifdef EB_FAST_TIME
#define EB_FAST_T (EB_FAST_TIME)
#endif

// базовый клас энкодера с кнопкой
class VirtEncButton : public VirtButton, public VirtEncoder {
   public:
    // ====================== SET ======================
    // установить таймаут быстрого поворота, мс
    void setFastTimeout(uint8_t tout) {
#ifndef EB_FAST_TIME
        EB_FAST_T = tout;
#endif
    }

    // виртуально зажать кнопку энкодера
    void holdEncButton(bool state) {
        write_ef(EB_EHLD_M, state);
    }

    // виртуально переключить кнопку энкодера
    void toggleEncButton() {
        write_ef(EB_EHLD_M, !read_ef(EB_EHLD_M));
    }

    // ====================== GET ======================
    // поворот энкодера [событие]
    bool turn() {
        return read_bf(EB_TRN_R);
    }

    // поворот направо [событие]
    bool right() {
        return read_ef(EB_DIR) && eq_bf(EB_TRN_R | EB_EHLD, EB_TRN_R);
    }

    // поворот налево [событие]
    bool left() {
        return !read_ef(EB_DIR) && eq_bf(EB_TRN_R | EB_EHLD, EB_TRN_R);
    }

    // нажатый поворот направо [событие]
    bool rightH() {
        return read_ef(EB_DIR) && eq_bf(EB_TRN_R | EB_EHLD, EB_TRN_R | EB_EHLD);
    }

    // нажатый поворот налево [событие]
    bool leftH() {
        return !read_ef(EB_DIR) && eq_bf(EB_TRN_R | EB_EHLD, EB_TRN_R | EB_EHLD);
    }

    // нажата кнопка энкодера [состояние]
    bool encHolding() {
        return read_ef(EB_EHLD_M) || read_bf(EB_EHLD);
    }

    // быстрый поворот энкодера [состояние]
    bool fast() {
        return read_ef(EB_FAST);
    }

    // ====================== POLL ======================
    // обработка в прерывании (только энкодер). Вернёт 0 в покое, 1 или -1 при повороте
    int8_t tickISR(bool e0, bool e1) {
        return _encFlags(VirtEncoder::tickISR(e0, e1));
    }

    // обработка в прерывании (только энкодер). Вернёт 0 в покое, 1 или -1 при повороте
    int8_t tickISR(int8_t e01) {
        return _encFlags(VirtEncoder::tickISR(e01));
    }

    // обработка энкодера и кнопки
    bool tick(bool e0, bool e1, bool btn) {
        return tick(e0 | (e1 << 1), btn);
    }

    // обработка энкодера и кнопки
    bool tick(int8_t e01, bool btn) {
        e01 = VirtEncoder::tick(e01);  // direction
        btn = VirtButton::tick(btn);   // button action
        _encFlags(e01);                // change flags by direction
        if (read_ef(EB_ETRN_R)) {      // turn flag from enc
            e01 = 1;                   // force 1 if turn (maybe isr)
            set_bf(EB_TRN_R);          // set flag in button
        }
        return e01 | btn;
    }

    // ===================== PRIVATE =====================
   protected:
#ifndef EB_FAST_TIME
    uint8_t EB_FAST_T = 30;
#endif

    int8_t _encFlags(int8_t state) {
        if (state) {
            // if (!read_bf(EB_HLD)) {             // кнопка не была удержана
            if (clicks) clicks = 0;                // сбросить клики
            if (read_bf(EB_PRS)) set_bf(EB_EHLD);  // зажать энкодер
            uint16_t ms = EB_UPTIME();
            write_ef(EB_FAST, ms - tmr < EB_FAST_T);
            if (!read_bf(EB_TOUT)) set_bf(EB_TOUT);
            tmr = ms;
            // }
        }
        return state;
    }
};