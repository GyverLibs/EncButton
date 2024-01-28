#pragma once
#include <Arduino.h>

#include "io.h"

// ===================== FLAGS ======================
#define EB_PRESS (1 << 0)        // нажатие на кнопку
#define EB_HOLD (1 << 1)         // кнопка удержана
#define EB_STEP (1 << 2)         // импульсное удержание
#define EB_RELEASE (1 << 3)      // кнопка отпущена
#define EB_CLICK (1 << 4)        // одиночный клик
#define EB_CLICKS (1 << 5)       // сигнал о нескольких кликах
#define EB_TURN (1 << 6)         // поворот энкодера
#define EB_REL_HOLD (1 << 7)     // кнопка отпущена после удержания
#define EB_REL_HOLD_C (1 << 8)   // кнопка отпущена после удержания с предв. кликами
#define EB_REL_STEP (1 << 9)     // кнопка отпущена после степа
#define EB_REL_STEP_C (1 << 10)  // кнопка отпущена после степа с предв. кликами

// =================== TOUT BUILD ===================
#define EB_SHIFT 4

// таймаут антидребезга, мс
#ifdef EB_DEB_TIME
#define EB_DEB_T (EB_DEB_TIME)
#endif

// таймаут между клтками, мс
#ifdef EB_CLICK_TIME
#define EB_CLICK_T (EB_CLICK_TIME)
#endif

// таймаут удержания, мс
#ifdef EB_HOLD_TIME
#define EB_HOLD_T (EB_HOLD_TIME)
#endif

// период степа, мс
#ifdef EB_STEP_TIME
#define EB_STEP_T (EB_STEP_TIME)
#endif

#define EB_FOR_SCALE 6

// =================== PACK FLAGS ===================
#define EB_CLKS_R (1 << 0)
#define EB_PRS_R (1 << 1)
#define EB_HLD_R (1 << 2)
#define EB_STP_R (1 << 3)
#define EB_REL_R (1 << 4)

#define EB_PRS (1 << 5)
#define EB_HLD (1 << 6)
#define EB_STP (1 << 7)
#define EB_REL (1 << 8)

#define EB_BUSY (1 << 9)
#define EB_DEB (1 << 10)
#define EB_TOUT (1 << 11)
#define EB_INV (1 << 12)
#define EB_BOTH (1 << 13)
#define EB_BISR (1 << 14)

#define EB_EHLD (1 << 15)

// базовый класс кнопки
class VirtButton {
   public:
    // ====================== SET ======================
    // установить таймаут удержания, умолч. 600 (макс. 4000 мс)
    void setHoldTimeout(const uint16_t& tout) {
#ifndef EB_HOLD_TIME
        EB_HOLD_T = tout >> EB_SHIFT;
#endif
    }

    // установить таймаут импульсного удержания, умолч. 200 (макс. 4000 мс)
    void setStepTimeout(const uint16_t& tout) {
#ifndef EB_STEP_TIME
        EB_STEP_T = tout >> EB_SHIFT;
#endif
    }

    // установить таймаут ожидания кликов, умолч. 500 (макс. 4000 мс)
    void setClickTimeout(const uint16_t& tout) {
#ifndef EB_CLICK_TIME
        EB_CLICK_T = tout >> EB_SHIFT;
#endif
    }

    // установить таймаут антидребезга, умолч. 50 (макс. 255 мс)
    void setDebTimeout(const uint8_t& tout) {
#ifndef EB_DEB_TIME
        EB_DEB_T = tout;
#endif
    }

    // установить уровень кнопки (HIGH - кнопка замыкает VCC, LOW - замыкает GND)
    void setBtnLevel(const bool& level) {
        write_bf(EB_INV, !level);
    }

    // кнопка нажата в прерывании (не учитывает btnLevel!)
    void pressISR() {
        if (!read_bf(EB_DEB)) tmr = EB_uptime();
        set_bf(EB_DEB | EB_BISR);
    }

    // сбросить системные флаги (принудительно закончить обработку)
    void reset() {
        clicks = 0;
        clr_bf(~EB_INV);
    }

    // принудительно сбросить флаги событий
    void clear() {
        if (read_bf(EB_CLKS_R)) clicks = 0;
        if (read_bf(EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R)) {
            clr_bf(EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R);
        }
    }

    // подключить функцию-обработчик событий (вида void f())
    void attach(void (*handler)()) {
#ifndef EB_NO_CALLBACK
        cb = *handler;
#endif
    }

    // отключить функцию-обработчик событий
    void detach() {
#ifndef EB_NO_CALLBACK
        cb = nullptr;
#endif
    }

    // ====================== GET ======================
    // кнопка нажата [событие]
    bool press() {
        return read_bf(EB_PRS_R);
    }

    // кнопка нажата с предварительными кликами [событие]
    bool press(const uint8_t& num) {
        return (clicks == num) && press();
    }

    // кнопка отпущена (в любом случае) [событие]
    bool release() {
        return eq_bf(EB_REL_R | EB_REL, EB_REL_R | EB_REL);
    }

    // кнопка отпущена (в любом случае) с предварительными кликами [событие]
    bool release(const uint8_t& num) {
        return (clicks == num) && release();
    }

    // клик по кнопке (отпущена без удержания) [событие]
    bool click() {
        return eq_bf(EB_REL_R | EB_REL | EB_HLD, EB_REL_R);
    }

    // клик по кнопке (отпущена без удержания) с предварительными кликами [событие]
    bool click(const uint8_t& num) {
        return (clicks == num) && click();
    }

    // кнопка зажата (между press() и release()) [состояние]
    bool pressing() {
        return read_bf(EB_PRS);
    }

    // кнопка зажата (между press() и release()) с предварительными кликами [состояние]
    bool pressing(const uint8_t& num) {
        return (clicks == num) && pressing();
    }

    // кнопка была удержана (больше таймаута) [событие]
    bool hold() {
        return read_bf(EB_HLD_R);
    }

    // кнопка была удержана (больше таймаута) с предварительными кликами [событие]
    bool hold(const uint8_t& num) {
        return (clicks == num) && hold();
    }

    // кнопка удерживается (больше таймаута) [состояние]
    bool holding() {
        return eq_bf(EB_PRS | EB_HLD, EB_PRS | EB_HLD);
    }

    // кнопка удерживается (больше таймаута) с предварительными кликами [состояние]
    bool holding(const uint8_t& num) {
        return (clicks == num) && holding();
    }

    // импульсное удержание [событие]
    bool step() {
        return read_bf(EB_STP_R);
    }

    // импульсное удержание с предварительными кликами [событие]
    bool step(const uint8_t& num) {
        return (clicks == num) && step();
    }

    // зафиксировано несколько кликов [событие]
    bool hasClicks() {
        return eq_bf(EB_CLKS_R | EB_HLD, EB_CLKS_R);
    }

    // зафиксировано указанное количество кликов [событие]
    bool hasClicks(const uint8_t& num) {
        return (clicks == num) && hasClicks();
    }

    // получить количество кликов
    uint8_t getClicks() {
        return clicks;
    }

    // получить количество степов
    uint16_t getSteps() {
#ifndef EB_NO_FOR
#ifdef EB_STEP_TIME
        return ftmr ? ((stepFor() + EB_STEP_T - 1) / EB_STEP_T) : 0;  // (x + y - 1) / y
#else
        return ftmr ? ((stepFor() + (EB_STEP_T << EB_SHIFT) - 1) / (EB_STEP_T << EB_SHIFT)) : 0;
#endif
#endif
        return 0;
    }

    // кнопка отпущена после удержания [событие]
    bool releaseHold() {
        return eq_bf(EB_REL_R | EB_REL | EB_HLD | EB_STP, EB_REL_R | EB_HLD);
    }

    // кнопка отпущена после удержания с предварительными кликами [событие]
    bool releaseHold(const uint8_t& num) {
        return clicks == num && eq_bf(EB_CLKS_R | EB_HLD | EB_STP, EB_CLKS_R | EB_HLD);
    }

    // кнопка отпущена после импульсного удержания [событие]
    bool releaseStep() {
        return eq_bf(EB_REL_R | EB_REL | EB_STP, EB_REL_R | EB_STP);
    }

    // кнопка отпущена после импульсного удержания с предварительными кликами [событие]
    bool releaseStep(const uint8_t& num) {
        return clicks == num && eq_bf(EB_CLKS_R | EB_STP, EB_CLKS_R | EB_STP);
    }

    // кнопка ожидает повторных кликов [состояние]
    bool waiting() {
        return clicks && eq_bf(EB_PRS | EB_REL, 0);
    }

    // идёт обработка [состояние]
    bool busy() {
        return read_bf(EB_BUSY);
    }

    // было действие с кнопки, вернёт код события [событие]
    uint16_t action() {
        switch (flags & 0b111111111) {
            case (EB_PRS | EB_PRS_R):
                return EB_PRESS;
            case (EB_PRS | EB_HLD | EB_HLD_R):
                return EB_HOLD;
            case (EB_PRS | EB_HLD | EB_STP | EB_STP_R):
                return EB_STEP;
            case (EB_REL | EB_REL_R):
            case (EB_REL | EB_REL_R | EB_HLD):
            case (EB_REL | EB_REL_R | EB_HLD | EB_STP):
                return EB_RELEASE;
            case (EB_REL_R):
                return EB_CLICK;
            case (EB_CLKS_R):
                return EB_CLICKS;
            case (EB_REL_R | EB_HLD):
                return EB_REL_HOLD;
            case (EB_CLKS_R | EB_HLD):
                return EB_REL_HOLD_C;
            case (EB_REL_R | EB_HLD | EB_STP):
                return EB_REL_STEP;
            case (EB_CLKS_R | EB_HLD | EB_STP):
                return EB_REL_STEP_C;
        }
        return 0;
    }

    // ====================== TIME ======================
    // после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [событие]
    bool timeout(const uint16_t& tout) {
        if (read_bf(EB_TOUT) && (uint16_t)((uint16_t)EB_uptime() - tmr) > tout) {
            clr_bf(EB_TOUT);
            return 1;
        }
        return 0;
    }

    // время, которое кнопка удерживается (с начала нажатия), мс
    uint16_t pressFor() {
#ifndef EB_NO_FOR
        if (ftmr) return (uint16_t)EB_uptime() - ftmr;
#endif
        return 0;
    }

    // кнопка удерживается дольше чем (с начала нажатия), мс [состояние]
    bool pressFor(const uint16_t& ms) {
        return pressFor() > ms;
    }

    // время, которое кнопка удерживается (с начала удержания), мс
    uint16_t holdFor() {
#ifndef EB_NO_FOR
        if (read_bf(EB_HLD)) {
#ifdef EB_HOLD_TIME
            return pressFor() - EB_HOLD_T;
#else
            return pressFor() - (EB_HOLD_T << EB_SHIFT);
#endif
        }
#endif
        return 0;
    }

    // кнопка удерживается дольше чем (с начала удержания), мс [состояние]
    bool holdFor(const uint16_t& ms) {
        return holdFor() > ms;
    }

    // время, которое кнопка удерживается (с начала степа), мс
    uint16_t stepFor() {
#ifndef EB_NO_FOR
        if (read_bf(EB_STP)) {
#ifdef EB_HOLD_TIME
            return pressFor() - EB_HOLD_T * 2;
#else
            return pressFor() - (EB_HOLD_T << EB_SHIFT) * 2;
#endif
        }
#endif
        return 0;
    }

    // кнопка удерживается дольше чем (с начала степа), мс [состояние]
    bool stepFor(uint16_t ms) {
        return stepFor() > ms;
    }

    // ====================== POLL ======================
    // обработка виртуальной кнопки как одновременное нажатие двух других кнопок
    bool tick(VirtButton& b0, VirtButton& b1) {
        if (read_bf(EB_BOTH)) {
            if (!b0.pressing() && !b1.pressing()) clr_bf(EB_BOTH);
            if (!b0.pressing()) b0.reset();
            if (!b1.pressing()) b1.reset();
            b0.clear();
            b1.clear();
            return tick(1);
        } else {
            if (b0.pressing() && b1.pressing()) set_bf(EB_BOTH);
            return tick(0);
        }
    }

    // обработка кнопки значением
    bool tick(bool s) {
        clear();
        s = pollBtn(s);
#ifndef EB_NO_CALLBACK
        if (cb && s) cb();
#endif
        return s;
    }

    // обработка кнопки без сброса событий и вызова коллбэка
    bool tickRaw(const bool& s) {
        return pollBtn(s);
    }

    uint8_t clicks;

    // deprecated
    void setButtonLevel(bool level) {
        write_bf(EB_INV, !level);
    }

    // ====================== PRIVATE ======================
   protected:
    bool pollBtn(bool s) {
        if (read_bf(EB_BISR)) {
            clr_bf(EB_BISR);
            s = 1;
        } else s ^= read_bf(EB_INV);

        if (!read_bf(EB_BUSY)) {
            if (s) set_bf(EB_BUSY);
            else return 0;
        }

        uint16_t ms = EB_uptime();
        uint16_t deb = ms - tmr;

        if (s) {                                         // кнопка нажата
            if (!read_bf(EB_PRS)) {                      // кнопка не была нажата ранее
                if (!read_bf(EB_DEB) && EB_DEB_T) {      // дебаунс ещё не сработал
                    set_bf(EB_DEB);                      // будем ждать дебаунс
                    tmr = ms;                            // сброс таймаута
                } else {                                 // первое нажатие
                    if (deb >= EB_DEB_T || !EB_DEB_T) {  // ждём EB_DEB_TIME
                        set_bf(EB_PRS | EB_PRS_R);       // флаг на нажатие
#ifndef EB_NO_FOR
                        ftmr = ms;
#endif
                        tmr = ms;  // сброс таймаута
                    }
                }
            } else {  // кнопка уже была нажата
                if (!read_bf(EB_EHLD)) {
                    if (!read_bf(EB_HLD)) {  // удержание ещё не зафиксировано
#ifdef EB_HOLD_TIME
                        if (deb >= (uint16_t)EB_HOLD_T) {  // ждём EB_HOLD_TIME - это удержание
#else
                        if (deb >= (uint16_t)(EB_HOLD_T << EB_SHIFT)) {  // ждём EB_HOLD_TIME - это удержание
#endif
                            set_bf(EB_HLD_R | EB_HLD);  // флаг что было удержание
                            tmr = ms;                   // сброс таймаута
                        }
                    } else {  // удержание зафиксировано
#ifdef EB_STEP_TIME
                        if (deb >= (uint16_t)(read_bf(EB_STP) ? EB_STEP_T : EB_HOLD_T)) {
#else
                        if (deb >= (uint16_t)(read_bf(EB_STP) ? (EB_STEP_T << EB_SHIFT) : (EB_HOLD_T << EB_SHIFT))) {
#endif
                            set_bf(EB_STP | EB_STP_R);  // флаг степ
                            tmr = ms;                   // сброс таймаута
                        }
                    }
                }
            }
        } else {                                       // кнопка не нажата
            if (read_bf(EB_PRS)) {                     // но была нажата
                if (deb >= EB_DEB_T) {                 // ждём EB_DEB_TIME
                    if (!read_bf(EB_HLD)) clicks++;    // не удерживали - это клик
                    if (read_bf(EB_EHLD)) clicks = 0;  //
                    set_bf(EB_REL | EB_REL_R);         // флаг release
                    clr_bf(EB_PRS);                    // кнопка отпущена
                }
            } else if (read_bf(EB_REL)) {
                if (!read_bf(EB_EHLD)) {
                    set_bf(EB_REL_R);  // флаг releaseHold / releaseStep
                }
                clr_bf(EB_REL | EB_EHLD);
                tmr = ms;         // сброс таймаута
            } else if (clicks) {  // есть клики, ждём EB_CLICK_TIME
#ifdef EB_CLICK_TIME
                if (read_bf(EB_HLD | EB_STP) || deb >= (uint16_t)EB_CLICK_T) set_bf(EB_CLKS_R);  // флаг clicks
#else
                if (read_bf(EB_HLD | EB_STP) || deb >= (uint16_t)(EB_CLICK_T << EB_SHIFT)) set_bf(EB_CLKS_R);  // флаг clicks
#endif
#ifndef EB_NO_FOR
                else if (ftmr) ftmr = 0;
#endif
            } else if (read_bf(EB_BUSY)) {
                clr_bf(EB_HLD | EB_STP | EB_BUSY);
                set_bf(EB_TOUT);
#ifndef EB_NO_FOR
                ftmr = 0;
#endif
                tmr = ms;  // test!!
            }
            if (read_bf(EB_DEB)) clr_bf(EB_DEB);  // сброс ожидания нажатия (дебаунс)
        }
        return read_bf(EB_CLKS_R | EB_PRS_R | EB_HLD_R | EB_STP_R | EB_REL_R);
    }

    uint16_t tmr = 0;
#ifndef EB_NO_FOR
    uint16_t ftmr = 0;
#endif

#ifndef EB_NO_CALLBACK
    void (*cb)() = nullptr;
#endif

#ifndef EB_DEB_TIME
    uint8_t EB_DEB_T = 50;
#endif
#ifndef EB_CLICK_TIME
    uint8_t EB_CLICK_T = (500 >> EB_SHIFT);
#endif
#ifndef EB_HOLD_TIME
    uint8_t EB_HOLD_T = (600 >> EB_SHIFT);
#endif
#ifndef EB_STEP_TIME
    uint8_t EB_STEP_T = (200 >> EB_SHIFT);
#endif

    inline void set_bf(const uint16_t& x) __attribute__((always_inline)) {
        flags |= x;
    }
    inline void clr_bf(const uint16_t& x) __attribute__((always_inline)) {
        flags &= ~x;
    }
    inline bool read_bf(const uint16_t& x) __attribute__((always_inline)) {
        return flags & x;
    }
    inline void write_bf(const uint16_t& x, const bool& v) __attribute__((always_inline)) {
        if (v) set_bf(x);
        else clr_bf(x);
    }
    inline bool eq_bf(const uint16_t& x, const uint16_t& y) __attribute__((always_inline)) {
        return (flags & x) == y;
    }

   private:
    uint16_t flags = 0;
};