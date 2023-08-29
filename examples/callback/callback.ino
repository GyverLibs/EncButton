// опрос событий через функцию-обработчик

#include <Arduino.h>
#include <EncButton.h>

EncButton eb(2, 3, 4);

void cb() {
    Serial.print("callback: ");
    switch (eb.action()) {
        case EB_PRESS:
            Serial.println("press");
            break;
        case EB_HOLD:
            Serial.println("hold");
            break;
        case EB_STEP:
            Serial.println("step");
            break;
        case EB_RELEASE:
            Serial.println("release");
            break;
        case EB_CLICK:
            Serial.println("click");
            break;
        case EB_CLICKS:
            Serial.print("clicks ");
            Serial.println(eb.getClicks());
            break;
        case EB_TURN:
            Serial.print("turn ");
            Serial.print(eb.dir());
            Serial.print(" ");
            Serial.print(eb.fast());
            Serial.print(" ");
            Serial.println(eb.encHolding());
            break;
        case EB_REL_HOLD:
            Serial.println("release hold");
            break;
        case EB_REL_HOLD_C:
            Serial.print("release hold clicks ");
            Serial.println(eb.getClicks());
            break;
        case EB_REL_STEP:
            Serial.println("release step");
            break;
        case EB_REL_STEP_C:
            Serial.print("release step clicks ");
            Serial.println(eb.getClicks());
            break;
        default: Serial.println();
    }
}

void setup() {
    Serial.begin(115200);
    eb.attach(cb);
}

void loop() {
    eb.tick();
}