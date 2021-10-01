// Пример с обработчиками в прерывании

#include <EncButton.h>
EncButton<EB_CALLBACK, 2, 3, 4> enc;   // энкодер с кнопкой <A, B, KEY>

void setup() {
  Serial.begin(9600);
  enc.attach(TURN_HANDLER, myTurn);   // подключим поворот

  // прерывание обеих фаз энкодера на функцию isr
  attachInterrupt(0, isr, CHANGE);
  attachInterrupt(1, isr, CHANGE);
}

void myTurn() {
  Serial.print("TURN_HANDLER: ");
  Serial.println(enc.counter);
}

void isr() {
  enc.tickISR();  // тикер в прерывании
  // Не вызывает подключенные коллбэки в нутри прерывания!!!
}

void loop() {
  enc.tick();   // дополнительный опрос таймаутов и коллбэков в loop
  // вызов подключенных функций будет здесь,
  // чтобы не грузить прерывание
}
