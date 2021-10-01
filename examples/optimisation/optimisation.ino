// пример с небольшой оптимизацией опроса

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc;  // энкодер с кнопкой <A, B, KEY>
//EncButton<EB_TICK, 2, 3> enc;     // просто энкодер <A, B>
//EncButton<EB_TICK, 4> enc;        // просто кнопка <KEY>

void setup() {
  Serial.begin(9600);
}

void loop() {
  // тик вернёт отличное от нуля значение, если произошло событие:
  // 1 - left + turn
  // 2 - right + turn
  // 3 - leftH + turnH
  // 4 - rightH + turnH
  // 5 - click
  // 6 - held
  // 7 - step
  // 8 - press

  // опрос этих событий можно проводить в условии, 
  // чтобы "не тратить время" на постоянный опрос в loop
  if (enc.tick()) {
    if (enc.turn()) Serial.println("turn");
    if (enc.turnH()) Serial.println("hold + turn");
    if (enc.left()) Serial.println("left");
    if (enc.right()) Serial.println("right");
    if (enc.leftH()) Serial.println("leftH");
    if (enc.rightH()) Serial.println("rightH");

    if (enc.press()) Serial.println("press");
    if (enc.click()) Serial.println("click");
    if (enc.held()) Serial.println("held");
    if (enc.step()) Serial.println("step");

    // в конце лучше вызвать resetState(), чтобы сбросить необработанные флаги!
    enc.resetState();
  }
}
