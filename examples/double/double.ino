// опрос одновременного нажатия двух кнопок как нажатия третьей кнопки (виртуальной)
// библиотека сама сбросит события с первых двух кнопок, если они нажаты вместе

#include <Arduino.h>
#include <EncButton.h>

Button b0(4);
Button b1(5);
VirtButton b2;  // виртуальная

void setup() {
    Serial.begin(115200);
}

void loop() {
    b0.tick();
    b1.tick();

    // обработка одновременного нажатия двух кнопок
    b2.tick(b0, b1);

    if (b0.click()) Serial.println("b0 click");
    if (b1.click()) Serial.println("b1 click");

    if (b2.click()) Serial.println("b0+b1 click");
    if (b2.step()) Serial.println("b0+b1 step");
}