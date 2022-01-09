// Пример с прямой работой библиотеки на прерываниях

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc;   // энкодер с кнопкой <A, B, KEY>
//EncButton<EB_TICK, 2, 3> enc;    // просто энкодер <A, B>
//EncButton<EB_TICK, 4> enc;       // просто кнопка <KEY>

void setup() {
  Serial.begin(9600);

  // желательно подключить оба пина энкодера на внешние прерывания по CHANGE
  // можно использовать PCINT https://github.com/NicoHood/PinChangeInterrupt
  attachInterrupt(0, isr, CHANGE);  // D2
  attachInterrupt(1, isr, CHANGE);  // D3
  // подключил оба прерывания на одну функцию
}

void isr() {
  enc.tickISR();  // в прерывании вызываем тик ISR
}

void loop() {
  // тут тоже вызываем тик, нужен для 
  // корректной работы дебаунсов и прочих таймеров!!!
  enc.tick();

  if (enc.turn()) {                 // любой поворот
    Serial.print("turn ");
    Serial.println(enc.counter);    // вывод счётчика
  }
  
  // имитация загруженной программы, обработка происходит в прерывании
  delay(50);
}
