/**
 * @file VirtEncButton.h
 * @brief Virtual encoder button class
 */

#pragma once
#include <Arduino.h>

#include "VirtButton.h"
#include "VirtEncoder.h"
#include "utils.h"

#ifdef EB_FAST_TIME
#define EB_FAST_T (EB_FAST_TIME)
#endif

/**
 * @brief A class representing a virtual encoder button.
 * 
 * This class inherits from VirtButton and VirtEncoder classes.
 * It provides methods for setting and getting the fast timeout,
 * as well as various methods for checking the state of the encoder button.
 * The class also includes methods for handling interrupts and polling the encoder button.
 */
class VirtEncButton : public VirtButton, public VirtEncoder {
   public:
    // ====================== SET ======================

    /**
     * @brief Set the Fast Timeout object
     * 
     * @param tout The fast timeout value to set
     * @return true if the fast timeout was successfully set, false otherwise
     */
    bool setFastTimeout(uint8_t tout) {
#ifndef EB_FAST_TIME
        EB_FAST_T = tout;
        return true;
#endif
        return false;
    }

    /**
     * @brief Clear the state of the virtual encoder button.
     * 
     * This function clears the state of the virtual encoder button,
     * including the button state and encoder state.
     */
    void clear() {
        VirtButton::clear();
        VirtEncoder::clear();
    }

    /**
     * @brief Attaches a callback function to handle a specific event of the virtual encoder button.
     * 
     * @param handler The event type to attach the callback function to.
     * @param callback_func The callback function to be attached.
     * @return Returns true if the callback function was successfully attached, false otherwise.
     */
    bool attach_call(EB_FLAGS_T handler, cb_t callback_func){
#ifndef EB_NO_CALLBACK
        if(callback_func){
            switch (handler)
            {
                case EB_PRESS:
                    if(cb[0]) return false;
                    return VirtButton::attach_call(callback_func);
                
                case EB_TURN_R:
                    if(cb[1]) return false;
                    return VirtEncoder::attach_right(callback_func); //todo attach_call

                case EB_TURN_L:
                    if(cb[2]) return false;
                    return VirtEncoder::attach_left(callback_func); //todo attach_call

                case EB_TURN_HOLD_R:
                    if(cb[3]) return false;
                    cb[3] = *callback_func;
                    return true;

                case EB_TURN_HOLD_L:
                    if(cb[4]) return false;
                    cb[4] = *callback_func;
                    return true;

                default:
                    return false;
            }
        }       
#endif
        return false;
    }

    /**
     * @brief Detach the callback functions for the virtual encoder button.
     */
    void detach_call(){
#ifndef EB_NO_CALLBACK
        VirtButton::detach_call();
        VirtEncoder::detach_call();
        cb[3] = nullptr;
        cb[4] = nullptr;
#endif
    }

    // ====================== GET ======================

    /**
     * @brief Check if the encoder button is turned to the right.
     * 
     * @return true if the encoder button is turned to the right, false otherwise
     */
    bool turnH() {
        return turn() && read_btn_flag(B_EHLD);
    }

    /**
     * @brief Check if the encoder button is in the fast mode.
     * 
     * @return true if the encoder button is in the fast mode, false otherwise
     */
    bool fast() {
        return read_encf(E_FAST);
    }

    /**
     * @brief Check if the encoder button is turned to the right without holding the button.
     * 
     * @return true if the encoder button is turned to the right without holding the button, false otherwise
     */
    bool right() {
        return read_encf(E_DIR) && turn() && !read_btn_flag(B_EHLD);
    }

    /**
     * @brief Check if the encoder button is turned to the left without holding the button.
     * 
     * @return true if the encoder button is turned to the left without holding the button, false otherwise
     */
    bool left() {
        return !read_encf(E_DIR) && turn() && !read_btn_flag(B_EHLD);
    }

    /**
     * @brief Check if the encoder button is turned to the right while holding the button.
     * 
     * @return true if the encoder button is turned to the right while holding the button, false otherwise
     */
    bool rightH() {
        return read_encf(E_DIR) && turnH();
    }

    /**
     * @brief Check if the encoder button is turned to the left while holding the button.
     * 
     * @return true if the encoder button is turned to the left while holding the button, false otherwise
     */
    bool leftH() {
        return !read_encf(E_DIR) && turnH();
    }

    /**
     * @brief Check if the encoder button is being held.
     * 
     * @return true if the encoder button is being held, false otherwise
     */
    bool encHolding() {
        return read_btn_flag(B_EHLD);
    }

    /**
     * @brief Get the action of the encoder button.
     * 
     * @return The action of the encoder button
     * @todo todo in binary switch case block e_state + b_state and EB_HOLD + EB_TURN verification
     *       if ((state & EB_HOLD) && (state & EB_TURN_R)) return EB_TURN_HOLD_R;
     */
    EB_FLAGS_T action() {
        EB_FLAGS_T e_state =  VirtEncoder::action(); 
        EB_FLAGS_T b_state =  VirtButton::action();

        if (e_state == EB_NONE) {
            return b_state; 
        }
        
        if(b_state == EB_HOLD){
            switch (e_state)
            {
                case EB_TURN_R:
                    return EB_TURN_HOLD_R;
                case EB_TURN_L:
                    return EB_TURN_HOLD_L;
                case EB_TURN:
                    return EB_TURN;
                default:
                    return EB_HOLD;
            }
        }
        return e_state;


        /* needs to be tested

         switch (b_state | e_state)
         {
            case 1000000001000:
                return EB_TURN_HOLD_R; //EB_HOLD + EB_TURN_R
            case 10000000011000:
                return EB_TURN_HOLD_L; //EB_HOLD + EB_TURN_L
            case 10000001000 :
                return EB_TURN;//EB_HOLD + EB_TURN
            case (e_state | EB_NONE):
                return e_state; //if b_state == EB_NONE
            default:
                return b_state; 
         }
        
        */
    }

    
    // ====================== POLL ======================
    // ISR

    /**
     * @brief Handle the interrupt tick for the encoder button.
     * 
     * @param e0 The state of the first encoder pin
     * @param e1 The state of the second encoder pin
     * @return The updated state of the encoder button
     */
    int8_t tickISR(bool e0, bool e1) {
        return tickISR(e0 | (e1 << 1));
    }

    /**
     * @brief Handle the interrupt tick for the encoder button.
     * 
     * @param state The current state of the encoder button
     * @return The updated state of the encoder button
     */
    int8_t tickISR(int8_t state) {
        state = VirtEncoder::pollEnc(state);
        if (state) {
#ifdef EB_NO_BUFFER
            set_encf(E_ISR_F);
            write_encf(E_DIR, state > 0);
            write_encf(E_FAST, checkFast());
#else
            for (uint8_t i = 0; i < 15; i += 3) {
                if (!(ebuffer & (1 << i))) {
                    ebuffer |= (1 << i);                 // turn
                    if (state > 0) ebuffer |= E_DIR;    // dir
                    if (checkFast()) ebuffer |= E_FAST;  // fast

                    // if (state > 0) ebuffer |= (1 << (i + 1));    // dir
                    // if (checkFast()) ebuffer |= (1 << (i + 2));  // fast
                    break;
                }
            }
#endif
        }
        return state;
    }

    /**
     * @brief Handle the tick for the encoder button.
     * 
     * @param e0 The state of the first encoder pin
     * @param e1 The state of the second encoder pin
     * @param btn The state of the button
     * @return true if the encoder button state has changed, false otherwise
     */
    bool tick(bool e0, bool e1, bool btn) {
        return tick(e0 | (e1 << 1), btn);
    }

    /**
     * @brief Handle the tick for the encoder button.
     * 
     * @param[out] state The current state of the encoder button
     * @param[out] btn The state of the button
     * @return true if the encoder button state has changed, false otherwise
     */
    bool tick(int8_t state, bool btn) {
        clear();
        bool f = tickRaw(state, btn);

#ifndef EB_NO_CALLBACK
        if(f){
            switch (action())
            {
                case EB_PRESS:
                    if(cb[0]) (this->cb[0])();
                    break;
                case EB_TURN_R:
                    if(cb[1]) (this->cb[1])();
                    break;
                case EB_TURN_L:
                    if(cb[2]) (this->cb[2])();
                    break;
                case EB_TURN_HOLD_R:
                    if(cb[3]) (this->cb[3])();
                    break;
                case EB_TURN_HOLD_L:
                    if(cb[4]) (this->cb[4])();
                    break;
                default:
                    break;
            }
        }
#endif
        return f;
    }

    /**
     * @brief Handle the tick for the encoder button.
     * 
     * @param btn The state of the button
     * @return true if the encoder button state has changed, false otherwise
     */
    bool tick(bool btn) {
        return tick(-1, btn);
    }

    // RAW

    /**
     * @brief Handle the raw tick for the encoder button.
     * 
     * @param e0 The state of the first encoder pin
     * @param e1 The state of the second encoder pin
     * @param btn The state of the button
     * @return true if the encoder button state has changed, false otherwise
     */
    bool tickRaw(bool e0, bool e1, bool btn) {
        return tickRaw(e0 | (e1 << 1), btn);
    }

    /**
     * @brief Handle the raw tick for the encoder button.
     * 
     * @param[out] state The current state of the encoder button
     * @param[out] btn The state of the button
     * @return true if the encoder button state has changed, false otherwise
     */
    bool tickRaw(int8_t state, bool btn) {
        btn = VirtButton::tickRaw(btn);

        bool encf = 0;
#ifdef EB_NO_BUFFER
        if (read_encf(E_ISR_F)) {
            clr_encf(E_ISR_F);
            encf = 1;
        }
#else
        if (ebuffer) {
            write_encf(E_DIR, ebuffer & 0b10);
            write_encf(E_FAST, ebuffer & 0b100);
            ebuffer >>= 3;
            encf = 1;
        }
#endif
        else if ((state >= 0) && (state = VirtEncoder::pollEnc(state))) {
            write_encf(E_DIR, state > 0);
            write_encf(E_FAST, checkFast());
            encf = 1;
        }
        if (encf) {
            if (read_btn_flag(B_PRS)) set_btn_flag(B_EHLD);    
            else clicks = 0;
            if (!read_btn_flag(B_TOUT)) set_btn_flag(B_TOUT);  
            set_encf(E_TRN_R);                      
        }
        return encf | btn;
    }

    /**
     * @brief Handle the raw tick for the encoder button.
     * 
     * @param btn The state of the button
     * @return true if the encoder button state has changed, false otherwise
     */
    bool tickRaw(bool btn) {
        return tickRaw(-1, btn);
    }

    // ===================== PRIVATE =====================

   protected:

#ifndef EB_NO_CALLBACK
    // void (*callback[14])() = {}; //from version 2
    cb_t cb[5] = {VirtButton::cb, VirtEncoder::cb[0], VirtEncoder::cb[1], nullptr, nullptr};
#endif

#ifndef EB_FAST_TIME
    uint8_t EB_FAST_T = 30;
#endif

#ifndef EB_NO_BUFFER
    uint16_t ebuffer = 0;
#endif

   private:
    bool checkFast() {
        uint16_t ms = EB_UPTIME();
        bool f = 0;
        if (ms - timer < EB_FAST_T) f = 1;
        timer = ms;
        return f;
    }
};