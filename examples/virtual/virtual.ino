#include <EncButton.h>

EncButton<EB_TICK, VIRT_BTN> enc;     // виртуальная кнопка
//EncButton<EB_TICK, VIRT_ENCBTN> enc;  // виртуальный энк с кнопкой
//EncButton<EB_TICK, VIRT_ENC> enc;     // виртуальный энк

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);   // подтянем пин
  //enc.setHoldTimeout(500);  // установка таймаута удержания кнопки
}

void loop() {
  // tick может принимать виртуальный сигнал:
  // (сигнал кнопки)
  // (сигнал энкодера А, сигнал энкодера B)
  // (сигнал энкодера А, сигнал энкодера B, сигнал кнопки)
  enc.tick(!digitalRead(4));

  if (enc.isClick()) Serial.println("click");
  if (enc.isHolded()) Serial.println("holded");
  if (enc.isStep()) Serial.println("step");

  if (enc.isPress()) Serial.println("press");
  if (enc.isClick()) Serial.println("click");
  if (enc.isRelease()) Serial.println("release");

  if (enc.hasClicks(1)) Serial.println("1 click");
  if (enc.hasClicks(2)) Serial.println("2 click");
  if (enc.hasClicks(3)) Serial.println("3 click");
  if (enc.hasClicks(5)) Serial.println("5 click");

  if (enc.hasClicks()) Serial.println(enc.clicks);
  //if (enc.isHold()) Serial.println("hold");
}
