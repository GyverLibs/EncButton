// энкодер и прерывания
#include <Arduino.h>
#include <EncButton.h>
EncButton eb(2, 3, 4);

/*
// esp8266/esp32
IRAM_ATTR void isr() {
  eb.tickISR();
}
*/

void isr() {
  eb.tickISR();
}

void setup() {
    Serial.begin(115200);
    attachInterrupt(0, isr, CHANGE);
    attachInterrupt(1, isr, CHANGE);
    eb.setEncISR(true);
}

void loop() {
    eb.tick();

    if (eb.turn()) {
        Serial.print("turn: dir ");
        Serial.print(eb.dir());
        Serial.print(", fast ");
        Serial.print(eb.fast());
        Serial.print(", hold ");
        Serial.print(eb.pressing());
        Serial.print(", counter ");
        Serial.println(eb.counter);
    }

    delay(100);  // имитация загруженной программы
}