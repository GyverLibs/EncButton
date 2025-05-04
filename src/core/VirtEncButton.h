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
    void setFastTimeout(const uint8_t tout) {
#ifndef EB_FAST_TIME
        EB_FAST_T = tout;
#endif
    }

    // сбросить флаги энкодера и кнопки
    void clear(bool resetTout = false) {
        VirtButton::clear(resetTout);
        VirtEncoder::clear();
    }

    // ====================== GET ======================
    // нажатый поворот энкодера [событие]
    bool turnH() {
        return turn() && bf.read(EB_EHLD);
    }

    // быстрый поворот энкодера [состояние]
    bool fast() {
        return ef.read(EB_FAST);
    }

    // поворот направо [событие]
    bool right() {
        return ef.read(EB_DIR) && turn() && !bf.read(EB_EHLD);
    }

    // поворот налево [событие]
    bool left() {
        return !ef.read(EB_DIR) && turn() && !bf.read(EB_EHLD);
    }

    // нажатый поворот направо [событие]
    bool rightH() {
        return ef.read(EB_DIR) && turnH();
    }

    // нажатый поворот налево [событие]
    bool leftH() {
        return !ef.read(EB_DIR) && turnH();
    }

    // нажата кнопка энкодера. Аналог pressing() [состояние]
    bool encHolding() {
        return bf.read(EB_EHLD);
    }

    // было действие с кнопки или энкодера, вернёт код события [событие]
    uint16_t action() {
        return turn() ? EB_TURN : VirtButton::action();
    }

    // было действие с кнопки или энкодера, вернёт код события [событие]
    EBAction getAction() {
        return (EBAction)action();
    }

    // ====================== POLL ======================
    // ISR
    // обработка в прерывании (только энкодер). Вернёт 0 в покое, 1 или -1 при повороте
    int8_t tickISR(const bool e0, const bool e1) {
        int8_t state = VirtEncoder::pollEnc(e0, e1);
        if (state) {
#ifdef EB_NO_BUFFER
            ef.set(EB_ISR_F);
            ef.write(EB_DIR, state > 0);
            ef.write(EB_FAST, _checkFast());
#else
            for (uint8_t i = 0; i < 15; i += 3) {
                if (!(ebuffer & (1 << i))) {
                    ebuffer |= (1 << i);                          // turn
                    if (state > 0) ebuffer |= (1 << (i + 1));     // dir
                    if (_checkFast()) ebuffer |= (1 << (i + 2));  // fast
                    break;
                }
            }
#endif
        }
        return state;
    }

    // TICK
    // обработка энкодера и кнопки
    bool tick(const bool e0, const bool e1, const bool btn) {
        clear();
        return _tick(_tickRaw(btn, pollEnc(e0, e1)));
    }

    // обработка энкодера (в прерывании) и кнопки
    bool tick(const bool btn) {
        clear();
        return _tick(_tickRaw(btn));
    }

    // RAW
    // обработка без сброса событий и вызова коллбэка
    bool tickRaw(const bool e0, const bool e1, bool btn) {
        return _tickRaw(btn, pollEnc(e0, e1));
    }

    // обработка без сброса событий и вызова коллбэка (кнопка)
    bool tickRaw(const bool btn) {
        return _tickRaw(btn);
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
    bool _checkFast() {
        uint16_t ms = EB_uptime();
        bool f = ms - tmr < EB_FAST_T;
        tmr = ms;
        return f;
    }

    inline bool _tick(bool f) {
#ifndef EB_NO_CALLBACK
        if (f || timeoutState()) call(true);
#endif
        return f;
    }

    bool _tickRaw(bool btn, int8_t estate = 0) {
        bool encf = 0;
#ifdef EB_NO_BUFFER
        if (ef.read(EB_ISR_F)) {
            ef.clear(EB_ISR_F);
            encf = 1;
        }
#else
        if (ebuffer) {
            ef.write(EB_DIR, ebuffer & 0b10);
            ef.write(EB_FAST, ebuffer & 0b100);
            ebuffer >>= 3;
            encf = 1;
        }
#endif
        else if (estate) {
            ef.write(EB_DIR, estate > 0);
            ef.write(EB_FAST, _checkFast());
            encf = 1;
        }
        if (encf) {
            if (bf.read(EB_PRS)) bf.set(EB_EHLD);  // зажать энкодер
            else clicks = 0;
            if (!bf.read(EB_TOUT)) bf.set(EB_TOUT);  // таймаут
            ef.set(EB_ETRN_R);                       // флаг поворота
        }
        return VirtButton::tickRaw(btn) | encf;
    }
};