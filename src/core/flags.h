#pragma once
#include <Arduino.h>

namespace encb {

template <typename T>
struct Flags {
    T flags = 0;

    inline T mask(const T x) __attribute__((always_inline)) {
        return flags & x;
    }
    inline void set(const T x) __attribute__((always_inline)) {
        flags |= x;
    }
    inline void clear(const T x) __attribute__((always_inline)) {
        flags &= ~x;
    }
    inline bool read(const T x) __attribute__((always_inline)) {
        return flags & x;
    }
    inline void write(const T x, const bool v) __attribute__((always_inline)) {
        v ? set(x) : clear(x);
    }
    inline bool eq(const T x, const T y) __attribute__((always_inline)) {
        return (flags & x) == y;
    }
    inline bool all(const T x) __attribute__((always_inline)) {
        return eq(x, x);
    }
};

}  // namespace encb