// пример с прерываниями pinChangeInterrupt (прерывания на любом пине)
// только для ATmega328 (UNO, Nano, Pro Mini)

#define CLK 4
#define DT 5
#define SW 6

#include <EncButton.h>
EncButton<EB_TICK, CLK, DT, SW> enc;

void setup() {
  Serial.begin(9600);

  // настроить PCINT
  attachPCINT(CLK);
  attachPCINT(DT);
}

void loop() {
  // оставляем тут для работы "временных" функций и антидребезга
  enc.tick();

  if (enc.turn()) {               // любой поворот
    Serial.print("turn ");
    Serial.println(enc.counter);    // вывод счётчика
  }

  if (enc.left()) {
    if (enc.fast()) Serial.println("fast left");
    else Serial.println("left");
  }

  if (enc.right()) {
    if (enc.fast()) Serial.println("fast right");
    else Serial.println("right");
  }
}

// функция для настройки PCINT для ATmega328 (UNO, Nano, Pro Mini)
uint8_t attachPCINT(uint8_t pin) {
  if (pin < 8) {              // D0-D7 - PCINT2
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << pin);
    return 2;
  }
  else if (pin > 13) {        // A0-A5 - PCINT1
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << pin - 14);
    return 1;
  }
  else {                      // D8-D13 - PCINT0
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << pin - 8);
    return 0;
  }
}

// Векторы PCINT, нужно кинуть сюда tickISR
// пины 0-7: PCINT2
// пины 8-13: PCINT0
// пины A0-A5: PCINT1
ISR(PCINT0_vect) {

}
ISR(PCINT1_vect) {

}
ISR(PCINT2_vect) {
  enc.tickISR();
}
