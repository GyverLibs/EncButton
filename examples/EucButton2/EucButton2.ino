// пример с библиотекой EncButton2

// Опциональные дефайн-настройки (показаны по умолчанию)
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 50      // дебаунс кнопки, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс

#include <EncButton2.h>
EncButton2<EB_ENCBTN> enc(INPUT, 2, 3, 4);  // энкодер с кнопкой
//EncButton2<EB_ENC> enc(INPUT, 2, 3);        // просто энкодер
//EncButton2<EB_BTN> enc(INPUT, 4);           // просто кнопка
// для изменения направления энкодера поменяй A и B при инициализации

// по умолчанию пины настроены в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
//EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  Serial.begin(9600);
  // ещё настройки
  //enc.counter = 100;        // изменение счётчика энкодера
  //enc.setHoldTimeout(500);  // установка таймаута удержания кнопки
  //enc.setButtonLevel(HIGH); // LOW - кнопка подключает GND (умолч.), HIGH - кнопка подключает VCC
}

void loop() {
  enc.tick();                       // опрос происходит здесь

  // =============== ЭНКОДЕР ===============
  // обычный поворот
  if (enc.turn()) {
    Serial.println("turn");

    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
    Serial.println(enc.getDir()); // направление поворота
  }

  // "нажатый поворот"
  if (enc.turnH()) {
    Serial.println("hold + turn");

    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
    Serial.println(enc.getDir()); // направление поворота
  }

  if (enc.left()) Serial.println("left");     // поворот налево
  if (enc.right()) Serial.println("right");   // поворот направо
  if (enc.leftH()) Serial.println("leftH");   // нажатый поворот налево
  if (enc.rightH()) Serial.println("rightH"); // нажатый поворот направо

  // =============== КНОПКА ===============
  if (enc.press()) Serial.println("press");
  if (enc.click()) Serial.println("click");
  if (enc.release()) Serial.println("release");

  if (enc.held()) Serial.println("held");     // однократно вернёт true при удержании
  //if (enc.hold()) Serial.println("hold");   // будет постоянно возвращать true после удержания
  if (enc.step()) Serial.println("step");     // импульсное удержание

  // проверка на количество кликов
  if (enc.hasClicks(1)) Serial.println("action 1 clicks");
  if (enc.hasClicks(2)) Serial.println("action 2 clicks");
  if (enc.hasClicks(3)) Serial.println("action 3 clicks");
  if (enc.hasClicks(5)) Serial.println("action 5 clicks");

  // вывести количество кликов
  if (enc.hasClicks()) {
    Serial.print("has clicks ");
    Serial.println(enc.clicks);
  }
}
