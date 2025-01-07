#pragma once
#include <Arduino.h>

#include "flags.h"
#include "io.h"
#include "self.h"

#ifndef __AVR__
#include <functional>
#endif

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
#define EB_TIMEOUT (1 << 11)     // прошёл таймаут после нажатия кнопки или поворота энкодера

enum class EBAction {
    None = 0,
    Press = EB_PRESS,
    Hold = EB_HOLD,
    Step = EB_STEP,
    Release = EB_RELEASE,
    Click = EB_CLICK,
    Clicks = EB_CLICKS,
    Turn = EB_TURN,
    ReleaseHold = EB_REL_HOLD,
    ReleaseHoldClicks = EB_REL_HOLD_C,
    ReleaseStep = EB_REL_STEP,
    ReleaseStepClicks = EB_REL_STEP_C,
    Timeout = EB_TIMEOUT,
};

// =================== TOUT BUILD ===================
#define EB_SHIFT 4

// таймаут антидребезга, мс
#ifdef EB_DEB_TIME
#define EB_DEB_T (EB_DEB_TIME)
#else
#endif

// таймаут между клтками, мс
#ifdef EB_CLICK_TIME
#define EB_CLICK_T (EB_CLICK_TIME)
#define EB_GET_CLICK_TIME() ((uint16_t)EB_CLICK_T)
#else
#define EB_GET_CLICK_TIME() (uint16_t)(EB_CLICK_T << EB_SHIFT)
#endif

// таймаут удержания, мс
#ifdef EB_HOLD_TIME
#define EB_HOLD_T (EB_HOLD_TIME)
#define EB_GET_HOLD_TIME() ((uint16_t)EB_HOLD_T)
#else
#define EB_GET_HOLD_TIME() (uint16_t)(EB_HOLD_T << EB_SHIFT)
#endif

// период степа, мс
#ifdef EB_STEP_TIME
#define EB_STEP_T (EB_STEP_TIME)
#define EB_GET_STEP_TIME() ((uint16_t)EB_STEP_T)
#else
#define EB_GET_STEP_TIME() (uint16_t)(EB_STEP_T << EB_SHIFT)
#endif

// время таймаута, мс
#ifdef EB_TOUT_TIME
#define EB_TOUT_T (EB_TOUT_TIME)
#define EB_GET_TOUT_TIME() ((uint16_t)EB_TOUT_T)
#else
#define EB_GET_TOUT_TIME() (uint16_t)(EB_TOUT_T << EB_SHIFT)
#endif

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
#ifdef __AVR__
    typedef void (*ActionHandler)();
#else
    typedef std::function<void()> ActionHandler;
#endif

   public:
    // ====================== SET ======================
    // установить таймаут удержания, умолч. 600 (макс. 4000 мс)
    void setHoldTimeout(const uint16_t tout) {
#ifndef EB_HOLD_TIME
        EB_HOLD_T = tout >> EB_SHIFT;
#endif
    }

    // установить таймаут импульсного удержания, умолч. 200 (макс. 4000 мс)
    void setStepTimeout(const uint16_t tout) {
#ifndef EB_STEP_TIME
        EB_STEP_T = tout >> EB_SHIFT;
#endif
    }

    // установить таймаут ожидания кликов, умолч. 500 (макс. 4000 мс)
    void setClickTimeout(const uint16_t tout) {
#ifndef EB_CLICK_TIME
        EB_CLICK_T = tout >> EB_SHIFT;
#endif
    }

    // установить таймаут антидребезга, умолч. 50 (макс. 255 мс)
    void setDebTimeout(const uint8_t tout) {
#ifndef EB_DEB_TIME
        EB_DEB_T = tout;
#endif
    }

    // установить время таймаута, умолч. 1000 (макс. 4000 мс)
    void setTimeout(const uint16_t tout) {
#ifndef EB_TOUT_TIME
        EB_TOUT_T = tout >> EB_SHIFT;
#endif
    }

    // установить уровень кнопки (HIGH - кнопка замыкает VCC, LOW - замыкает GND)
    void setBtnLevel(const bool level) {
        bf.write(EB_INV, !level);
    }

    // кнопка нажата в прерывании (не учитывает btnLevel!)
    void pressISR() {
        if (!bf.read(EB_DEB)) tmr = EB_uptime();
        bf.set(EB_DEB | EB_BISR);
    }

    // сбросить системные флаги (принудительно закончить обработку)
    void reset() {
        clicks = 0;
        bf.clear(~EB_INV);  // все кроме EB_INV
    }

    // принудительно сбросить флаги событий
    void clear(bool resetTout = false) {
        if (resetTout && bf.read(EB_TOUT)) bf.clear(EB_TOUT);
        if (bf.read(EB_CLKS_R)) clicks = 0;
        if (bf.read(EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R)) {
            bf.clear(EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R);
        }
    }

    // игнорировать все события до отпускания кнопки
    void skipEvents() {
        bf.set(EB_EHLD);
    }

    // подключить функцию-обработчик событий (вида void f())
    void attach(ActionHandler handler) {
#ifndef EB_NO_CALLBACK
        cb = handler;
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
        return bf.read(EB_PRS_R);
    }

    // кнопка нажата с предварительными кликами [событие]
    bool press(const uint8_t num) {
        return (clicks == num) && press();
    }

    // кнопка отпущена (в любом случае) [событие]
    bool release() {
        return bf.eq(EB_REL_R | EB_REL, EB_REL_R | EB_REL);
    }

    // кнопка отпущена (в любом случае) с предварительными кликами [событие]
    bool release(const uint8_t num) {
        return (clicks == num) && release();
    }

    // клик по кнопке (отпущена без удержания) [событие]
    bool click() {
        return bf.eq(EB_REL_R | EB_REL | EB_HLD, EB_REL_R);
    }

    // клик по кнопке (отпущена без удержания) с предварительными кликами [событие]
    bool click(const uint8_t num) {
        return (clicks == num) && click();
    }

    // кнопка зажата (между press() и release()) [состояние]
    bool pressing() {
        return bf.read(EB_PRS);
    }

    // кнопка зажата (между press() и release()) с предварительными кликами [состояние]
    bool pressing(const uint8_t num) {
        return (clicks == num) && pressing();
    }

    // кнопка была удержана (больше таймаута) [событие]
    bool hold() {
        return bf.read(EB_HLD_R);
    }

    // кнопка была удержана (больше таймаута) с предварительными кликами [событие]
    bool hold(const uint8_t num) {
        return (clicks == num) && hold();
    }

    // кнопка удерживается (больше таймаута) [состояние]
    bool holding() {
        return bf.eq(EB_PRS | EB_HLD, EB_PRS | EB_HLD);
    }

    // кнопка удерживается (больше таймаута) с предварительными кликами [состояние]
    bool holding(const uint8_t num) {
        return (clicks == num) && holding();
    }

    // импульсное удержание [событие]
    bool step() {
        return bf.read(EB_STP_R);
    }

    // импульсное удержание с предварительными кликами [событие]
    bool step(const uint8_t num) {
        return (clicks == num) && step();
    }

    // зафиксировано несколько кликов [событие]
    bool hasClicks() {
        return bf.eq(EB_CLKS_R | EB_HLD, EB_CLKS_R);
    }

    // зафиксировано указанное количество кликов [событие]
    bool hasClicks(const uint8_t num) {
        return (clicks == num) && hasClicks();
    }

    // получить количество кликов
    uint8_t getClicks() {
        return clicks;
    }

    // получить количество степов
    uint16_t getSteps() {
#ifndef EB_NO_FOR
        return ftmr ? ((stepFor() + EB_GET_STEP_TIME() - 1) / EB_GET_STEP_TIME()) : 0;  // (x + y - 1) / y
#endif
        return 0;
    }

    // кнопка отпущена после удержания [событие]
    bool releaseHold() {
        return bf.eq(EB_REL_R | EB_REL | EB_HLD | EB_STP, EB_REL_R | EB_HLD);
    }

    // кнопка отпущена после удержания с предварительными кликами [событие]
    bool releaseHold(const uint8_t num) {
        return clicks == num && bf.eq(EB_CLKS_R | EB_HLD | EB_STP, EB_CLKS_R | EB_HLD);
    }

    // кнопка отпущена после импульсного удержания [событие]
    bool releaseStep() {
        return bf.eq(EB_REL_R | EB_REL | EB_STP, EB_REL_R | EB_STP);
    }

    // кнопка отпущена после импульсного удержания с предварительными кликами [событие]
    bool releaseStep(const uint8_t num) {
        return clicks == num && bf.eq(EB_CLKS_R | EB_STP, EB_CLKS_R | EB_STP);
    }

    // кнопка отпущена после удержания или импульсного удержания [событие]
    bool releaseHoldStep() {
        return releaseHold() || releaseStep();
    }

    // кнопка отпущена после удержания или импульсного удержания с предварительными кликами [событие]
    bool releaseHoldStep(const uint8_t num) {
        return releaseHold(num) || releaseStep(num);
    }

    // кнопка ожидает повторных кликов [состояние]
    bool waiting() {
        return clicks && bf.eq(EB_PRS | EB_REL, 0);
    }

    // идёт обработка [состояние]
    bool busy() {
        return bf.read(EB_BUSY);
    }

    // было действие с кнопки, вернёт код события [событие]
    uint16_t action() {
        switch (bf.mask(0b111111111)) {
            case (EB_PRS | EB_PRS_R): return EB_PRESS;
            case (EB_PRS | EB_HLD | EB_HLD_R): return EB_HOLD;
            case (EB_PRS | EB_HLD | EB_STP | EB_STP_R): return EB_STEP;
            case (EB_REL | EB_REL_R):
            case (EB_REL | EB_REL_R | EB_HLD):
            case (EB_REL | EB_REL_R | EB_HLD | EB_STP):
                return EB_RELEASE;
            case (EB_REL_R): return EB_CLICK;
            case (EB_CLKS_R): return EB_CLICKS;
            case (EB_REL_R | EB_HLD): return EB_REL_HOLD;
            case (EB_CLKS_R | EB_HLD): return EB_REL_HOLD_C;
            case (EB_REL_R | EB_HLD | EB_STP): return EB_REL_STEP;
            case (EB_CLKS_R | EB_HLD | EB_STP): return EB_REL_STEP_C;
        }
        if (timeoutState()) return EB_TIMEOUT;
        return 0;
    }

    // было действие с кнопки, вернёт код события [событие]
    EBAction getAction() {
        return (EBAction)action();
    }

    // ====================== TIME ======================
    // после взаимодействия с кнопкой (или энкодером EncButton) время setTimeout, мс [событие]
    bool timeout() {
        if (timeoutState()) {
            bf.clear(EB_TOUT);
            return 1;
        }
        return 0;
    }

    // после взаимодействия с кнопкой (или энкодером EncButton) время setTimeout, мс [состояние]
    bool timeoutState() {
        return bf.read(EB_TOUT) && (uint16_t)((uint16_t)EB_uptime() - tmr) >= EB_GET_TOUT_TIME();
    }

    // время, которое кнопка удерживается (с начала нажатия), мс
    uint16_t pressFor() {
#ifndef EB_NO_FOR
        if (ftmr) return (uint16_t)EB_uptime() - ftmr;
#endif
        return 0;
    }

    // кнопка удерживается дольше чем (с начала нажатия), мс [состояние]
    bool pressFor(const uint16_t ms) {
        return pressFor() > ms;
    }

    // время, которое кнопка удерживается (с начала удержания), мс
    uint16_t holdFor() {
#ifndef EB_NO_FOR
        if (bf.read(EB_HLD)) return pressFor() - EB_GET_HOLD_TIME();
#endif
        return 0;
    }

    // кнопка удерживается дольше чем (с начала удержания), мс [состояние]
    bool holdFor(const uint16_t ms) {
        return holdFor() > ms;
    }

    // время, которое кнопка удерживается (с начала степа), мс
    uint16_t stepFor() {
#ifndef EB_NO_FOR
        if (bf.read(EB_STP)) return pressFor() - EB_GET_HOLD_TIME() * 2;
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
        if (bf.read(EB_BOTH)) {
            if (!b0.pressing() && !b1.pressing()) bf.clear(EB_BOTH);
            if (!b0.pressing()) b0.reset();
            if (!b1.pressing()) b1.reset();
            b0.clear();
            b1.clear();
            return tick(1);
        } else {
            if (b0.pressing() && b1.pressing()) bf.set(EB_BOTH);
            return tick(0);
        }
    }

    // обработка кнопки значением
    bool tick(bool s) {
        clear();
        s = pollBtn(s);
#ifndef EB_NO_CALLBACK
        if (s || timeoutState()) call();
#endif
        return s;
    }

    // обработка кнопки без сброса событий и вызова коллбэка
    bool tickRaw(const bool s) {
        return pollBtn(s);
    }

    // вызвать обработчик
    void call(bool force = false) {  // todo force заменить на флаг
#ifndef EB_NO_CALLBACK
        if (cb && (force || action())) {
            if (cb) {
                EB_self = this;
                cb();
                EB_self = nullptr;
                timeout();  // todo clear tout
            }
        }
#endif
    }

    uint8_t clicks;

    // deprecated
    void setButtonLevel(bool level) __attribute__((deprecated)) {
        bf.write(EB_INV, !level);
    }
    
    // после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [событие]
    bool timeout(const uint16_t tout) /*__attribute__((deprecated))*/ {
        if (timeoutState(tout)) {
            bf.clear(EB_TOUT);
            return 1;
        }
        return 0;
    }

    // после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [состояние]
    bool timeoutState(const uint16_t tout) /*__attribute__((deprecated))*/ {
        return (bf.read(EB_TOUT) && (uint16_t)((uint16_t)EB_uptime() - tmr) > tout);
    }

    // ====================== PRIVATE ======================
   protected:
    uint16_t tmr = 0;
    encb::Flags<uint16_t> bf;

#ifndef EB_NO_CALLBACK
    ActionHandler cb = nullptr;
#endif

   private:
#ifndef EB_NO_FOR
    uint16_t ftmr = 0;
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
#ifndef EB_TOUT_TIME
    uint8_t EB_TOUT_T = (1000 >> EB_SHIFT);
#endif

    bool pollBtn(bool s) {
        if (bf.read(EB_BISR)) {
            bf.clear(EB_BISR);
            s = 1;
        } else s ^= bf.read(EB_INV);

        if (!bf.read(EB_BUSY)) {
            if (s) bf.set(EB_BUSY);
            else return 0;
        }

        uint16_t ms = EB_uptime();
        uint16_t deb = ms - tmr;

        if (s) {                                         // кнопка нажата
            if (!bf.read(EB_PRS)) {                      // кнопка не была нажата ранее
                if (!bf.read(EB_DEB) && EB_DEB_T) {      // дебаунс ещё не сработал
                    bf.set(EB_DEB);                      // будем ждать дебаунс
                    tmr = ms;                            // сброс таймаута
                } else {                                 // первое нажатие
                    if (deb >= EB_DEB_T || !EB_DEB_T) {  // ждём EB_DEB_TIME
                        bf.set(EB_PRS | EB_PRS_R);       // флаг на нажатие
#ifndef EB_NO_FOR
                        ftmr = ms;
#endif
                        tmr = ms;  // сброс таймаута
                    }
                }
            } else {  // кнопка уже была нажата
                if (!bf.read(EB_EHLD)) {
                    if (!bf.read(EB_HLD)) {               // удержание ещё не зафиксировано
                        if (deb >= EB_GET_HOLD_TIME()) {  // ждём EB_HOLD_TIME - это удержание
                            bf.set(EB_HLD_R | EB_HLD);    // флаг что было удержание
                            tmr = ms;                     // сброс таймаута
                        }
                    } else {  // удержание зафиксировано
                        if (deb >= (uint16_t)(bf.read(EB_STP) ? EB_GET_STEP_TIME() : EB_GET_HOLD_TIME())) {
                            bf.set(EB_STP | EB_STP_R);  // флаг степ
                            tmr = ms;                   // сброс таймаута
                        }
                    }
                }
            }
        } else {                                       // кнопка не нажата
            if (bf.read(EB_PRS)) {                     // но была нажата
                if (deb >= EB_DEB_T) {                 // ждём EB_DEB_TIME
                    if (!bf.read(EB_HLD)) clicks++;    // не удерживали - это клик
                    if (bf.read(EB_EHLD)) clicks = 0;  //
                    bf.set(EB_REL | EB_REL_R);         // флаг release
                    bf.clear(EB_PRS);                  // кнопка отпущена
                }
            } else if (bf.read(EB_REL)) {
                if (!bf.read(EB_EHLD)) {
                    bf.set(EB_REL_R);  // флаг releaseHold / releaseStep
                }
                bf.clear(EB_REL | EB_EHLD);
                tmr = ms;                                                                       // сброс таймаута
            } else if (clicks) {                                                                // есть клики, ждём EB_CLICK_TIME
                if (bf.read(EB_HLD | EB_STP) || deb >= EB_GET_CLICK_TIME()) bf.set(EB_CLKS_R);  // флаг clicks
#ifndef EB_NO_FOR
                else if (ftmr) ftmr = 0;
#endif
            } else if (bf.read(EB_BUSY)) {
                bf.clear(EB_HLD | EB_STP | EB_BUSY);
                bf.set(EB_TOUT);
#ifndef EB_NO_FOR
                ftmr = 0;
#endif
                tmr = ms;  // test!!
            }
            if (bf.read(EB_DEB)) bf.clear(EB_DEB);  // сброс ожидания нажатия (дебаунс)
        }
        return bf.read(EB_CLKS_R | EB_PRS_R | EB_HLD_R | EB_STP_R | EB_REL_R);
    }
};