/**
 * @file VirtButton.h
 * @brief VirtButton class for handling virtual buttons.
 */

#pragma once
#include <Arduino.h>

#include "utils.h"

#define EB_SHIFT 4

#ifdef EB_DEB_TIME
#define EB_DEB_T (EB_DEB_TIME)
#endif

#ifdef EB_CLICK_TIME
#define EB_CLICK_T (EB_CLICK_TIME)
#endif

#ifdef EB_HOLD_TIME
#define EB_HOLD_T (EB_HOLD_TIME)
#endif

#ifdef EB_STEP_TIME
#define EB_STEP_T (EB_STEP_TIME)
#endif

#define EB_FOR_SCALE 6

#define BUTTON_FLAGS \
    X(B_CLKS_R, 0)\
    X(B_PRS_R,  1)\
    X(B_HLD_R,  2)\
    X(B_STP_R,  3)\
    X(B_REL_R,  4)\
    X(B_PRS,    5)\
    X(B_HLD,    6)\
    X(B_STP,    7)\
    X(B_REL,    8)\
    X(B_BUSY,   9)\
    X(B_DEB,    10)\
    X(B_TOUT,   11)\
    X(B_INV,    12)\
    X(B_BOTH,   13)\
    X(B_BISR,   14)\
    X(B_EHLD,   15)
    
#define X(a, b) a = (1 << b),
typedef enum {
	BUTTON_FLAGS
} BUTTON_FLAG_T;
#undef X


class VirtButton {
   public:
    // ====================== SET ======================

    void init(){
        setHoldTimeout();
        setStepTimeout();
        setClickTimeout();
        setDebounceTimeout();

    }

    /**
     * @brief Sets the hold timeout for the virtual button.
     * 
     * @param timeout The hold timeout value in milliseconds. Default is 600 milliseconds.
     * @note max timeout is 4000 ms
     */
    void setHoldTimeout(uint16_t timeout=600) {
#ifndef EB_HOLD_TIME
        EB_HOLD_T = (timeout < 4000) ? timeout >> EB_SHIFT : 4000 >> EB_SHIFT;
#endif
    }

    /**
     * Sets the step timeout for the virtual button.
     * @param timeout The timeout value in milliseconds. Default is 200 milliseconds.
     * @note max timeout is 4000 ms
     */
    void setStepTimeout(uint16_t timeout=200) {
#ifndef EB_STEP_TIME
        EB_STEP_T = (timeout < 4000) ? timeout >> EB_SHIFT : 4000 >> EB_SHIFT;
#endif
    }

    /**
     * @brief Sets the click timeout for the virtual button.  Clicks faster than this timeout will be detected.
     * 
     * @param timeout The click timeout value in milliseconds. Default is 500 milliseconds. Max is 4000ms.
     */
    void setClickTimeout(uint16_t timeout=500) {
#ifndef EB_CLICK_TIME
        EB_CLICK_T = (timeout < 4000) ? timeout >> EB_SHIFT : 4000 >> EB_SHIFT;
#endif
    }

    void setDebounceTimeout(uint8_t timeout=50) {
#ifndef EB_DEB_TIME
        EB_DEB_T = (timeout < 255) ? timeout : 255;
#endif
    }

    void setBtnLevel(bool level) {
        write_btn_flag(B_INV, !level);
    }

    void pressISR() {
        if (!read_btn_flag(B_DEB)) timer = EB_UPTIME();
        set_btn_flag(B_DEB | B_BISR);
    }

    void reset() {
        clicks = 0;
        clr_btn_flag(~B_INV);
    }

    void clear() {
        if (read_btn_flag(B_CLKS_R)) clicks = 0;
        if (read_btn_flag(B_CLKS_R | B_STP_R | B_PRS_R | B_HLD_R | B_REL_R)) {
            clr_btn_flag(B_CLKS_R | B_STP_R | B_PRS_R | B_HLD_R | B_REL_R);
        }
    }


    /**
     * @brief Attach a callback function to handle button events.
     *         The callback will be called when button events occur.
     * 
     * @param callback_func pointer to the callback function
     * @return true if the callback is attached, false otherwise
     */
    bool attach_call( cb_t callback_func) { 
#ifndef EB_NO_CALLBACK
        if (callback_func == nullptr || cb) return false;
        cb = *callback_func;
        return true;
#endif
        return false;
    }

    //todo check variant with one name on 2 virtual functions
    void detach_call() {
#ifndef EB_NO_CALLBACK
        cb = nullptr;
#endif
    }

    // ====================== GET ======================
    bool press() {
        return read_btn_flag(B_PRS_R);
    }

    bool release() {
        return eq_btn_flag(B_REL_R | B_REL, B_REL_R | B_REL);
    }

    bool click() {
        return eq_btn_flag(B_REL_R | B_REL | B_HLD, B_REL_R);
    }

    bool pressing() {
        return read_btn_flag(B_PRS);
    }

    bool hold() {
        return read_btn_flag(B_HLD_R);
    }

    bool hold(uint8_t num) {
        return clicks == num && hold();
    }

    bool holding() {
        return eq_btn_flag(B_PRS | B_HLD, B_PRS | B_HLD);
    }

    bool holding(uint8_t num) {
        return clicks == num && holding();
    }

    bool step() {
        return read_btn_flag(B_STP_R);
    }

    bool step(uint8_t num) {
        return clicks == num && step();
    }

    bool hasClicks() {
        return eq_btn_flag(B_CLKS_R | B_HLD, B_CLKS_R);
    }

    bool hasClicks(uint8_t num) {
        return clicks == num && hasClicks();
    }

    uint8_t getClicks() {
        return clicks;
    }

    uint16_t getSteps() {
#ifndef EB_NO_PEDOMETER
#ifdef EB_STEP_TIME
        return run_timer ? ((stepFor() + EB_STEP_T - 1) / EB_STEP_T) : 0;  
#else
        return run_timer ? ((stepFor() + (EB_STEP_T << EB_SHIFT) - 1) / (EB_STEP_T << EB_SHIFT)) : 0;
#endif
#endif
        return 0;
    }

    bool releaseHold() {
        return eq_btn_flag(B_REL_R | B_REL | B_HLD | B_STP, B_REL_R | B_HLD);
    }

    bool releaseHold(uint8_t num) {
        return clicks == num && eq_btn_flag(B_CLKS_R | B_HLD | B_STP, B_CLKS_R | B_HLD);
    }

    bool releaseStep() {
        return eq_btn_flag(B_REL_R | B_REL | B_STP, B_REL_R | B_STP);
    }

    bool releaseStep(uint8_t num) {
        return clicks == num && eq_btn_flag(B_CLKS_R | B_STP, B_CLKS_R | B_STP);
    }

    bool waiting() {
        return clicks && eq_btn_flag(B_PRS | B_REL, 0);
    }

    bool busy() {
        return read_btn_flag(B_BUSY);
    }

    /**
     * Get the action corresponding to the current flag combination.
     *
     * @return The action corresponding to the current flag combination.
     */
    EB_FLAGS_T action() {
        switch (flags & 0b111111111) {
            case (B_PRS | B_PRS_R):
                return EB_PRESS;
            case (B_PRS | B_HLD | B_HLD_R):
                return EB_HOLD;
            case (B_PRS | B_HLD | B_STP | B_STP_R):
                return EB_STEP;
            case (B_REL | B_REL_R):
            case (B_REL | B_REL_R | B_HLD):
            case (B_REL | B_REL_R | B_HLD | B_STP):
                return EB_RELEASE;
            case (B_REL_R):
                return EB_CLICK;
            case (B_CLKS_R):
                return EB_CLICKS;
            case (B_REL_R | B_HLD):
                return EB_REL_HOLD;
            case (B_CLKS_R | B_HLD):
                return EB_REL_HOLD_C;
            case (B_REL_R | B_HLD | B_STP):
                return EB_REL_STEP;
            case (B_CLKS_R | B_HLD | B_STP):
                return EB_REL_STEP_C;
            default:
                return EB_NONE;
        }
    }

    // ====================== TIME ======================
    /**
     * Checks if the timeout has occurred.
     * 
     * @param tout The timeout value in milliseconds.
     * @return True if the timeout has occurred, false otherwise.
     */
    bool timeout(uint16_t tout) {
        if (read_btn_flag(B_TOUT) && (uint16_t)((uint16_t)EB_UPTIME() - timer) > tout) {
            clr_btn_flag(B_TOUT);
            return 1;
        }
        return 0;
    }

    /**
     * Returns the duration in milliseconds for which the button has been pressed.
     * If the EB_NO_PEDOMETER macro is defined, the function always returns 0.
     * If the button is not currently pressed, the function also returns 0.
     *
     * @return The duration in milliseconds for which the button has been pressed.
     */
    uint16_t pressFor() {
#ifndef EB_NO_PEDOMETER
        if (run_timer) return (uint16_t)EB_UPTIME() - run_timer;
#endif
        return 0;
    }

    bool pressFor(uint16_t ms) {
        return pressFor() > ms;
    }

    /**
     * Calculates the duration for which the button has been held.
     * 
     * @return The duration in milliseconds.
     */
    uint16_t holdFor() {
#ifndef EB_NO_PEDOMETER
        if (read_btn_flag(B_HLD)) {
#ifdef EB_HOLD_TIME
            return pressFor() - EB_HOLD_T;
#else
            return pressFor() - (EB_HOLD_T << EB_SHIFT);
#endif
        }
#endif
        return 0;
    }

    /**
     * Checks if the button has been held for a specified duration.
     * 
     * @param ms The duration in milliseconds.
     * @return True if the button has been held for the specified duration, false otherwise.
     */
    bool holdFor(uint16_t ms) {
        return holdFor() > ms;
    }

    uint16_t stepFor() {
#ifndef EB_NO_PEDOMETER
        if (read_btn_flag(B_STP)) {
#ifdef EB_HOLD_TIME
            return pressFor() - EB_HOLD_T * 2;
#else
            return pressFor() - (EB_HOLD_T << EB_SHIFT) * 2;
#endif
        }
#endif
        return 0;
    }

    bool stepFor(uint16_t ms) {
        return stepFor() > ms;
    }

    // ====================== POLL ======================
    bool tick(VirtButton& b0, VirtButton& b1) {
        if (read_btn_flag(B_BOTH)) {
            if (!b0.pressing() && !b1.pressing()) clr_btn_flag(B_BOTH);
            if (!b0.pressing()) b0.reset();
            if (!b1.pressing()) b1.reset();
            b0.clear();
            b1.clear();
            return tick(1);
        } else {
            if (b0.pressing() && b1.pressing()) set_btn_flag(B_BOTH);
            return tick(0);
        }
    }

    bool tick(bool state) {
        clear();
        state = pollBtn(state);
#ifndef EB_NO_CALLBACK
        if (cb && state) cb();
#endif
        return state;
    }


    bool tickRaw(bool state) {
        return pollBtn(state);
    }

    uint8_t clicks;

    // deprecated
    void setButtonLevel(bool level) {
        write_btn_flag(B_INV, !level);
    }

    // ====================== PRIVATE ======================
   protected:
    bool pollBtn(bool s) {
        if (read_btn_flag(B_BISR)) {
            clr_btn_flag(B_BISR);
            s = 1;
        } else s ^= read_btn_flag(B_INV);

        if (!read_btn_flag(B_BUSY)) {
            if (s) set_btn_flag(B_BUSY);
            else return 0;
        }

        uint16_t ms = EB_UPTIME();
        uint16_t deb = ms - timer;

        if (s) {                                      
            if (!read_btn_flag(B_PRS)) {                  
                if (!read_btn_flag(B_DEB) && EB_DEB_T) {  
                    set_btn_flag(B_DEB);                   
                    timer = ms;                         
                } else {                                
                    if (deb >= EB_DEB_T || !EB_DEB_T) { 
                        set_btn_flag(B_PRS | B_PRS_R);      
#ifndef EB_NO_PEDOMETER
                        run_timer = ms;
#endif
                        timer = ms;  
                    }
                }
            } else {  
                if (!read_btn_flag(B_EHLD)) {
                    if (!read_btn_flag(B_HLD)) {  
#ifdef EB_HOLD_TIME
                        if (deb >= (uint16_t)EB_HOLD_T) {  
#else
                        if (deb >= (uint16_t)(EB_HOLD_T << EB_SHIFT)) {  
#endif
                            set_btn_flag(B_HLD_R | B_HLD); 
                            timer = ms;                 
                        }
                    } else {  
#ifdef EB_STEP_TIME
                        if (deb >= (uint16_t)(read_btn_flag(B_STP) ? EB_STEP_T : EB_HOLD_T)) {
#else
                        if (deb >= (uint16_t)(read_btn_flag(B_STP) ? (EB_STEP_T << EB_SHIFT) : (EB_HOLD_T << EB_SHIFT))) {
#endif
                            set_btn_flag(B_STP | B_STP_R);  
                            timer = ms;                   
                        }
                    }
                }
            }
        } else {                                     
            if (read_btn_flag(B_PRS)) {                 
                if (deb >= EB_DEB_T) {               
                    if (!read_btn_flag(B_HLD)) clicks++;    
                    if (read_btn_flag(B_EHLD)) clicks = 0;  
                    set_btn_flag(B_REL | B_REL_R);        
                    clr_btn_flag(B_PRS);                   
                }
            } else if (read_btn_flag(B_REL)) {
                if (!read_btn_flag(B_EHLD)) {
                    set_btn_flag(B_REL_R);
                }
                clr_btn_flag(B_REL | B_EHLD);
                timer = ms;       
            } else if (clicks) {  
#ifdef EB_CLICK_TIME
                if (read_btn_flag(B_HLD | B_STP) || deb >= (uint16_t)EB_CLICK_T) set_btn_flag(B_CLKS_R);  
#else
                if (read_btn_flag(B_HLD | B_STP) || deb >= (uint16_t)(EB_CLICK_T << EB_SHIFT)) set_btn_flag(B_CLKS_R);  
#endif
#ifndef EB_NO_PEDOMETER
                else if (run_timer) run_timer = 0;
#endif
            } else if (read_btn_flag(B_BUSY)) {
                clr_btn_flag(B_HLD | B_STP | B_BUSY);
                set_btn_flag(B_TOUT);
#ifndef EB_NO_PEDOMETER
                run_timer = 0;
#endif
                timer = ms;  // test!!
            }
            if (read_btn_flag(B_DEB)) clr_btn_flag(B_DEB);  
        }
        return read_btn_flag(B_CLKS_R | B_PRS_R | B_HLD_R | B_STP_R | B_REL_R);
    }

    uint16_t timer = 0;
#ifndef EB_NO_PEDOMETER
    uint16_t run_timer = 0;
#endif

#ifndef EB_NO_CALLBACK
    cb_t cb = nullptr;
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

    inline void set_btn_flag(const uint16_t x) __attribute__((always_inline)) {
        flags |= x;
    }
    inline void clr_btn_flag(const uint16_t x) __attribute__((always_inline)) {
        flags &= ~x;
    }
    inline bool read_btn_flag(const uint16_t x) __attribute__((always_inline)) {
        return flags & x;
    }
    inline void write_btn_flag(const uint16_t x, bool v) __attribute__((always_inline)) {
        if (v) set_btn_flag(x);
        else clr_btn_flag(x);
    }
    inline bool eq_btn_flag(const uint16_t x, const uint16_t y) __attribute__((always_inline)) {
        return (flags & x) == y;
    }

   private:
    uint16_t flags = 0;
};