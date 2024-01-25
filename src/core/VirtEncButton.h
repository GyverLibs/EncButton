#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "VirtEncoder.h"
#include "io.h"

#ifdef EB_FAST_TIME
#define EB_FAST_T (EB_FAST_TIME)
#endif

// базовый клас энкодера с кнопкой
class VirtEncButton : public VirtButton, public VirtEncoder {
   public:
    // ====================== SET ======================
    // установить таймаут быстрого поворота, мс
    void setFastTimeout(const uint8_t& tout) {
#ifndef EB_FAST_TIME
        EB_FAST_T = tout;
#endif
    }

    // сбросить флаги энкодера и кнопки
    void clear() {
        VirtButton::clear();
        VirtEncoder::clear();
    }

    // ====================== GET ======================
    // нажатый поворот энкодера [событие]
    bool turnH() {
        return turn() && read_bf(EB_EHLD);
    }

    // быстрый поворот энкодера [состояние]
    bool fast() {
        return read_ef(EB_FAST);
    }

    // поворот направо [событие]
    bool right() {
        return read_ef(EB_DIR) && turn() && !read_bf(EB_EHLD);
    }

    // поворот налево [событие]
    bool left() {
        return !read_ef(EB_DIR) && turn() && !read_bf(EB_EHLD);
    }

    // нажатый поворот направо [событие]
    bool rightH() {
        return read_ef(EB_DIR) && turnH();
    }

    // нажатый поворот налево [событие]
    bool leftH() {
        return !read_ef(EB_DIR) && turnH();
    }

    // нажата кнопка энкодера. Аналог pressing() [состояние]
    bool encHolding() {
        return read_bf(EB_EHLD);
    }

    // было действие с кнопки или энкодера, вернёт код события [событие]
    uint16_t action() {
        if (turn()) return EB_TURN;
        else return VirtButton::action();
    }

    // ====================== POLL ======================
    // ISR
    // обработка в прерывании (только энкодер). Вернёт 0 в покое, 1 или -1 при повороте
    int8_t tickISR(const bool& e0, const bool& e1) {
        return tickISR(e0 | (e1 << 1));
    }

    // обработка в прерывании (только энкодер). Вернёт 0 в покое, 1 или -1 при повороте
    int8_t tickISR(int8_t state) {
        state = VirtEncoder::pollEnc(state);
        if (state) {
#ifdef EB_NO_BUFFER
            set_ef(EB_ISR_F);
            write_ef(EB_DIR, state > 0);
            write_ef(EB_FAST, checkFast());
#else
            for (uint8_t i = 0; i < 15; i += 3) {
                if (!(ebuffer & (1 << i))) {
                    ebuffer |= (1 << i);                         // turn
                    if (state > 0) ebuffer |= (1 << (i + 1));    // dir
                    if (checkFast()) ebuffer |= (1 << (i + 2));  // fast
                    break;
                }
            }
#endif
        }
        return state;
    }

    // TICK
    // обработка энкодера и кнопки
    bool tick(const bool& e0, const bool& e1, const bool& btn) {
        return tick(e0 | (e1 << 1), btn);
    }

    // обработка энкодера и кнопки. state = -1 для пропуска обработки энкодера
    bool tick(const int8_t& state, const bool& btn) {
        clear();
        bool f = tickRaw(state, btn);

#ifndef EB_NO_CALLBACK
        if (cb && f) cb();
#endif
        return f;
    }

    // обработка энкодера (в прерывании) и кнопки
    bool tick(const bool& btn) {
        return tick(-1, btn);
    }

    // RAW
    // обработка без сброса событий и вызова коллбэка
    bool tickRaw(const bool& e0, const bool& e1, const bool& btn) {
        return tickRaw(e0 | (e1 << 1), btn);
    }

    // обработка без сброса событий и вызова коллбэка
    bool tickRaw(int8_t state, bool btn) {
        btn = VirtButton::tickRaw(btn);

        bool encf = 0;
#ifdef EB_NO_BUFFER
        if (read_ef(EB_ISR_F)) {
            clr_ef(EB_ISR_F);
            encf = 1;
        }
#else
        if (ebuffer) {
            write_ef(EB_DIR, ebuffer & 0b10);
            write_ef(EB_FAST, ebuffer & 0b100);
            ebuffer >>= 3;
            encf = 1;
        }
#endif
        else if ((state >= 0) && (state = VirtEncoder::pollEnc(state))) {
            write_ef(EB_DIR, state > 0);
            write_ef(EB_FAST, checkFast());
            encf = 1;
        }
        if (encf) {
            if (read_bf(EB_PRS)) set_bf(EB_EHLD);    // зажать энкодер
            else clicks = 0;
            if (!read_bf(EB_TOUT)) set_bf(EB_TOUT);  // таймаут
            set_ef(EB_ETRN_R);                       // флаг поворота
        }
        return encf | btn;
    }

    // обработка без сброса событий и вызова коллбэка (кнопка)
    bool tickRaw(const bool& btn) {
        return tickRaw(-1, btn);
    }

    // ===================== PRIVATE =====================
   protected:
#ifndef EB_FAST_TIME
    uint8_t EB_FAST_T = 30;
#endif

#ifndef EB_NO_BUFFER
    uint16_t ebuffer = 0;
#endif

   private:
    bool checkFast() {
        uint16_t ms = EB_uptime();
        bool f = 0;
        if (ms - tmr < EB_FAST_T) f = 1;
        tmr = ms;
        return f;
    }
};