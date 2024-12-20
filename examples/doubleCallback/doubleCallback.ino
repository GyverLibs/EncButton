// опрос одновременного нажатия двух кнопок как нажатия третьей кнопки
// с корректным вызовом обработчиков

#include <Arduino.h>
#include <EncButton.h>

Button b0(4);
Button b1(5);
MultiButton b12;  // виртуальная

void decode(uint16_t action) {
    switch (action) {
        case EB_PRESS:
            Serial.println("press");
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
            Serial.println("clicks");
            break;
        case EB_REL_HOLD:
            Serial.println("release hold");
            break;
        case EB_REL_HOLD_C:
            Serial.println("release hold clicks ");
            break;
        case EB_REL_STEP:
            Serial.println("release step");
            break;
        case EB_REL_STEP_C:
            Serial.println("release step clicks ");
            break;
        case EB_TIMEOUT:
            Serial.println("timeout");
            break;
    }
}

void setup() {
    Serial.begin(115200);

    // обработчики
    b0.attach([]() {
        uint16_t action = static_cast<VirtButton*>(EB_self)->action();
        if (action != EB_HOLD) Serial.print("b0: ");
        decode(action);
    });

    b1.attach([]() {
        uint16_t action = static_cast<VirtButton*>(EB_self)->action();
        if (action != EB_HOLD) Serial.print("b1: ");
        decode(action);
    });

    b12.attach([]() {
        uint16_t action = static_cast<VirtButton*>(EB_self)->action();
        if (action != EB_HOLD) Serial.print("b0+b1: ");
        decode(action);
    });
}

void loop() {
    // обработка одновременного нажатия двух кнопок
    // обрабатываются все три кнопки
    b12.tick(b0, b1);

    // или вручную
    if (b0.click()) Serial.println("b0 click");
    if (b1.click()) Serial.println("b1 click");
    if (b12.click()) Serial.println("b0+b1 click");
}