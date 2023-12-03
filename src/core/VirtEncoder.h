/**
 * @file VirtEncoder.h
 * @brief Virtual encoder class.
 * 
 */

#pragma once
#include <Arduino.h>

#include "utils.h"

// ===================== CONST ======================

#define EB_STEP4_LOW 0
#define EB_STEP4_HIGH 1
#define EB_STEP2 2
#define EB_STEP1 3

#define ENC_FLAGS \
    X(E_TYPE,    0)\
    X(E_REVERSE, 2)\
    X(E_FAST,    3)\
    X(E_DIR,     4)\
    X(E_TRN_R,   5)\
    X(E_ISR_F,   6)\
    X(E_ISR,     7)

    
#define X(a, b) a = (1 << b),
typedef enum {
	ENC_FLAGS
} ENC_FLAG_T;
#undef X

/**
 * @class VirtEncoder
 * @brief A class representing a virtual encoder.
 *        This class provides methods to set the encoder's properties,
 *        attach_call callback functions for handling encoder events,
 *        and poll the encoder for rotation and direction.
 */
class VirtEncoder {
   public:
   
    /**
     * @brief Default constructor for VirtEncoder class.
     */
    VirtEncoder() : prev(0), ecount(0) {}
    
    // ====================== SET ======================

    /**
     * @brief Set the encoder's reverse flag.
     * 
     * @param[in] rev true to reverse the encoder, false otherwise
     */
    void setEncReverse(bool rev = false) {
        if (rev) set_encf(E_REVERSE);
        else clr_encf(E_REVERSE);
    }

    /**
     * @brief Set the encoder's type.
     * 
     * @param[in] type the encoder type to set
     */
    void setEncType(uint8_t type = EB_STEP1) {
        flags = (flags & 0b11111100) | type;
    }

    /**
     * @brief Set the encoder's interrupt service routine.
     * 
     * @param[in] use true to use the ISR, false otherwise
     * @note Argument "use" by default is true because name of this function 
     *          means that it is used to set/activate the ISR.
     */
    void setEncISR(bool use=true){
        write_encf(E_ISR, use);
    }

    /**
     * @brief Attach a callback function to handle encoder events when turned right.
     * 
     * @param[in] callback_func pointer to the callback function
     * @note callback_func can be "cb_t" type 
     * @return true if the callback is attached, false otherwise
     */
    bool attach_right(cb_t callback_func){
#ifndef EB_NO_CALLBACK
        uint8_t l_r = (read_encf(E_REVERSE)) ? 1 : 0;
        if (callback_func == nullptr || cb[l_r]) return false;
        cb[l_r] = *callback_func;
        return true;
#endif
        return false;
    }

    /**
     * @brief Attach a callback function to handle encoder events when turned left.
     * 
     * @param[in] callback_func pointer to the callback function
     * @return true if the callback is attached, false otherwise
     */
    bool attach_left(cb_t callback_func){
#ifndef EB_NO_CALLBACK
        uint8_t l_r = (read_encf(E_REVERSE)) ? 0 : 1;
        if (callback_func == nullptr || cb[l_r]) return false;
        cb[l_r] = *callback_func;
        return true;
#endif
        return false;
    }

    //todo universal function
    // void attach_call(){}

    /**
     * @brief Detach the callback functions for handling encoder events.
     */
    void detach_call() {
#ifndef EB_NO_CALLBACK
        cb[0] = nullptr;
        cb[1] = nullptr;
#endif
    }

    /**
     * @brief Initialize the encoder with the specified values.
     * 
     * @param[in] e0 the value of the first encoder pin
     * @param[in] e1 the value of the second encoder pin
     */
    void initEnc(bool e0, bool e1) {
        initEnc(e0 | (e1 << 1));
    }

    /**
     * @brief Initialize the encoder with the specified value.
     * 
     * @param[in] v the initial value of the encoder
     */
    void initEnc(int8_t v) {
        prev = v;
    }

    /**
     * @brief Clear the encoder flags.
     */
    void clear() {
        if (read_encf(E_TRN_R)) clr_encf(E_TRN_R);
    }

    // ====================== POLL ======================

    /**
     * @brief Check if the encoder has been turned.
     * 
     * @return true if the encoder has been turned, false otherwise
     */
    bool turn() {
        return read_encf(E_TRN_R);
    }

    /**
     * @brief Get the direction of rotation of the encoder.
     * 
     * @return -1 if rotated left, 1 if rotated right
     */
    int8_t dir() {
        return read_encf(E_DIR) ? 1 : -1;
    }

    /**
     * @brief Representing the possible flags for the encoder.
     * @return EB_FLAGS_T can have the following values:
     * - EB_TURN: Represents the flag for turning action.
     * - EB_TURN_L: Represents the flag for left turning action.
     * - EB_TURN_R: Represents the flag for right turning action.
     * - EB_NONE: Represents the flag for no action.
     */
    EB_FLAGS_T action() {
        switch (flags & 0b111111111) {
            case(E_TRN_R | E_FAST):
                return EB_TURN;
            case(E_TRN_R | E_DIR | E_REVERSE):
                return EB_TURN_L;
            case(E_TRN_R | E_DIR):
                return EB_TURN_R;   
            default:
                return EB_NONE;
        }
    }

    /**
     * @brief Process the encoder tick using the interrupt service routine.
     * 
     * @param[in] e0 the value of the first encoder pin
     * @param[in] e1 the value of the second encoder pin
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tickISR(bool e0, bool e1) {
        return tickISR(e0 | (e1 << 1));
    }

    /**
     * @brief Process the encoder tick using the interrupt service routine.
     * 
     * @param[in] state the current state of the encoder
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tickISR(int8_t state) {
        state = pollEnc(state);
        if (state) {
            set_encf(E_ISR_F);
            write_encf(E_DIR, state > 0);
        }
        return state;
    }

    /**
     * @brief Process the encoder tick.
     * 
     * @param[in] e0 the value of the first encoder pin
     * @param[in] e1 the value of the second encoder pin
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tick(bool e0, bool e1) {
        return tick(e0 | (e1 << 1));
    }

    /**
     * @brief Process the encoder tick.
     * 
     * @param[in] state the current state of the encoder
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tick(int8_t state) {
        state = tickRaw(state);
        if (state)
        {
    #ifndef EB_NO_CALLBACK
            if (cb[0] && state < 0) cb[0]();
            if (cb[1] && state > 0) cb[1]();
    #endif
            return state;
        }
        clear();
        return 0;
    }

    /**
     * @brief Process the encoder tick without clearing the turn flag.
     * 
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tick() {
        return tick(-1);
    }

    /**
     * @brief Process the encoder tick without clearing the turn flag.
     * 
     * @param[in] e0 the value of the first encoder pin
     * @param[in] e1 the value of the second encoder pin
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tickRaw(bool e0, bool e1) {
        return tickRaw(e0 | (e1 << 1));
    }

    /**
     * @name tickRaw   
     * @brief Process the encoder tick without clearing the turn flag.
     * 
     * @param[in] state the current state of the encoder
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t tickRaw(int8_t state) {
        if (read_encf(E_ISR_F)) {
            clr_encf(E_ISR_F);
            set_encf(E_TRN_R);
            return dir();
        }
        if ((state >= 0) && (state = pollEnc(state))) {
            write_encf(E_DIR, state > 0);
            set_encf(E_TRN_R);
            return state;
        }
        return 0;
    }

    
    int8_t tickRaw() {
        return tickRaw(-1);
    }

    /**
     * @brief Poll the encoder without setting the turn flag.
     * 
     * @param[in] e0 the value of the first encoder pin
     * @param[in] e1 the value of the second encoder pin
     * @return -1, 0, or 1 based on the direction of rotation
     */
    int8_t pollEnc(bool e0, bool e1) {
        return pollEnc(e0 | (e1 << 1));
    }

    /**
     * @brief Poll the encoder without setting the turn flag.
     * 
     * @param[in] state the current state of the encoder
     * @return -1, 0, or 1 based on the direction of rotation
     */
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
            state = ((ecount > 0) ^ read_encf(E_REVERSE)) ? 1 : -1;
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

    // ===================== PROTECTED =====================
   protected:
    inline void set_encf(const uint16_t x) __attribute__((always_inline)) {
        flags |= x;
    }
    inline void clr_encf(const uint16_t x) __attribute__((always_inline)) {
        flags &= ~x;
    }
    inline void write_encf(const uint16_t x, bool v) __attribute__((always_inline)) {
        if (v) set_encf(x);
        else clr_encf(x);
    }
    inline bool read_encf(const uint16_t x) __attribute__((always_inline)) {
        return flags & x;
    }

#ifndef EB_NO_CALLBACK
    cb_t cb[2] = {nullptr, nullptr}; // callbacks for right and left turns
#endif

    // ===================== PRIVATE =====================
   private:
    uint8_t flags = 0;
    int8_t prev : 4;
    int8_t ecount : 4;
};