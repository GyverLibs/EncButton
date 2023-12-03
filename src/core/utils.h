/**
 * @file utils.h
 * @brief Utility functions for handling pins and flags.
 */

#pragma once

#include <Arduino.h>

// ===================== FLAGS ======================
//EB_NONE for error catching and Encoder with button handling
#define EB_FLAGS \
    X(EB_NONE         , 0)\
    X(EB_PRESS        , 1)\
    X(EB_STEP         , 2)\
    X(EB_HOLD         , 3)\
    X(EB_RELEASE      , 4)\
    X(EB_CLICK        , 5)\
    X(EB_CLICKS       , 6)\
    X(EB_REL_HOLD     , 7)\
    X(EB_REL_HOLD_C   , 8)\
    X(EB_REL_STEP     , 9)\
    X(EB_REL_STEP_C   , 10)\
    X(EB_TURN         , 11)\
    X(EB_TURN_R       , 12)\
    X(EB_TURN_L       , 13)\
    X(EB_TURN_HOLD_R  , 14)\
    X(EB_TURN_HOLD_L  , 15)

//TURN_H_HANDLER    
#define X(a, b) a = (1 << b),
typedef enum {
	EB_FLAGS
} EB_FLAGS_T;
#undef X

#define EB_UPTIME() millis()

typedef void (*cb_t)();

bool EBread(const uint8_t pin);
void EBwrite(const uint8_t pin, bool value);
