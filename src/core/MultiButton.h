#pragma once
#include <Arduino.h>

#include "VirtButton.h"

class MultiButton : public VirtButton {
   public:
    template <typename T0, typename T1>
    bool tick(T0& b0, T1& b1) {
        b0.clear();
        b1.clear();
        b0.tickRaw();
        b1.tickRaw();

        if (!bf.read(EB_BOTH)) {
            b0.call();
            b1.call();
        }
        return VirtButton::tick(b0, b1);
    }
};