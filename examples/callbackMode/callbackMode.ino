// Пример с обработчиками

// Опциональные дефайн-настройки (показаны по умолчанию)
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 80      // дебаунс кнопки, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс

// подключение - только PULL-UP, внешний или внутренний!
// для изменения направления энкодера поменяй A и B при инициализации

#include <EncButton.h>
EncButton<EB_CALLBACK, 2, 3, 4> enc;   // энкодер с кнопкой <A, B, KEY>
//EncButton<EB_CALLBACK, 2, 3> enc;    // просто энкодер <A, B>
//EncButton<EB_CALLBACK, 4> enc;       // просто кнопка <KEY>

// по умолчанию пины настроены в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
//EncButton<EB_CALLBACK, 2, 3, 4> enc(INPUT);

void setup() {
  Serial.begin(9600);

  enc.attach(TURN_HANDLER, myTurn);
  enc.attach(TURN_H_HANDLER, myTurnH);
  
  enc.attach(RIGHT_HANDLER, myRight);
  enc.attach(LEFT_HANDLER, myLeft);

  enc.attach(RIGHT_H_HANDLER, myRightH);
  enc.attach(LEFT_H_HANDLER, myLeftH);
  
  enc.attach(CLICK_HANDLER, myClick);
  enc.attach(HOLDED_HANDLER, myHolded);
  enc.attach(STEP_HANDLER, myStep);

  enc.attach(PRESS_HANDLER, myPress);
  enc.attach(RELEASE_HANDLER, myRelease);
  enc.attach(HOLD_HANDLER, myHold);
  
  enc.attach(CLICKS_HANDLER, myClicks);
  enc.attachClicks(5, fiveClicks);
}

void myTurn() {
  Serial.print("TURN_HANDLER: ");
  Serial.println(enc.counter);    // вывод счётчика

  // также можно опросить здесь (isRight, isLeft)
  if (enc.isLeft()) {
    if (enc.isFast()) Serial.println("fast left");
    else Serial.println("left");
  }
}

void myTurnH() {
  Serial.print("TURN_H_HANDLER, direction: ");
  Serial.println(enc.getDir());
}

void myRight() {
  Serial.println("RIGHT_HANDLER");
}
void myLeft() {
  Serial.println("LEFT_HANDLER");
}
void myRightH() {
  Serial.println("RIGHT_H_HANDLER");
}
void myLeftH() {
  Serial.println("LEFT_H_HANDLER");
}
void myClick() {
  Serial.println("CLICK_HANDLER");
}
void myHolded() {
  Serial.println("HOLDED_HANDLER");
}
void myStep() {
  Serial.println("STEP_HANDLER");
}
void myClicks() {
  Serial.print("CLICKS_HANDLER: ");
  Serial.println(enc.clicks);
}
void fiveClicks() {
  Serial.println("action fiveClicks");
}
void myPress() {
  Serial.println("PRESS_HANDLER");
}
void myRelease() {
  Serial.println("RELEASE_HANDLER");
}
void myHold() {
  Serial.println("HOLD_HANDLER");
}

// =============== LOOP =============
void loop() {
  enc.tick();   // обработка всё равно здесь
}
