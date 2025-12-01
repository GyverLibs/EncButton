#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "VirtEncoder.h"
#include "io.h"

#ifdef EB_FAST_TIME
#define EB_FAST_T (EB_FAST_TIME)
#endif

#define EB_BUF_TURN (1 << 0)
#define EB_BUF_DIR (1 << 1)
#define EB_BUF_FAST (1 << 2)
#define EB_BUF_LEN 3

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
        if (!state) return state;

#ifdef EB_NO_BUFFER
        ef.set(EB_ISR_F);
        ef.write(EB_DIR, state > 0);
        ef.write(EB_FAST, _checkFast());
#else
        for (uint8_t i = 0; i < 15; i += EB_BUF_LEN) {
            if (ebuffer & (EB_BUF_TURN << i)) continue;
            ebuffer |= (EB_BUF_TURN | ((state > 0) * EB_BUF_DIR) | (_checkFast() * EB_BUF_FAST)) << i;
            break;
        }
#endif
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

    // ===================== DEPRECATED =====================
    bool isTurnH() __attribute__((deprecated)) { return turnH(); }
    bool isTurn() __attribute__((deprecated)) { return turn(); }
    bool isFast() __attribute__((deprecated)) { return fast(); }
    bool isLeftH() __attribute__((deprecated)) { return leftH(); }
    bool isRightH() __attribute__((deprecated)) { return rightH(); }
    bool isLeft() __attribute__((deprecated)) { return left(); }
    bool isRight() __attribute__((deprecated)) { return right(); }

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
        bool f = (ms - tmr) < EB_FAST_T;
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
#ifdef EB_NO_BUFFER
        if (ef.read(EB_ISR_F)) {
            ef.clear(EB_ISR_F);
        }
#else
        if (ebuffer) {
            ef.write(EB_DIR, ebuffer & EB_BUF_DIR);
            ef.write(EB_FAST, ebuffer & EB_BUF_FAST);
            ebuffer >>= EB_BUF_LEN;
        }
#endif
        else if (estate) {
            ef.write(EB_DIR, estate > 0);
            ef.write(EB_FAST, _checkFast());
        } else {
            return VirtButton::tickRaw(btn);
        }

        if (bf.read(EB_PRS)) bf.set(EB_EHLD);  // зажать энкодер
        else clicks = 0;
        bf.set(EB_TOUT);    // таймаут
        ef.set(EB_ETRN_R);  // флаг поворота

        VirtButton::tickRaw(btn);
        return true;
    }
};