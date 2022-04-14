// Пример с прямой работой библиотеки
// просто загрузи и потыкай - всё будет понятно =)

// Опциональные дефайн-настройки (показаны по умолчанию)
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 50      // дебаунс кнопки, мс
//#define EB_CLICK 400   // таймаут накликивания, мс

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc;  // энкодер с кнопкой <A, B, KEY>
//EncButton<EB_TICK, 2, 3> enc;     // просто энкодер <A, B>
//EncButton<EB_TICK, 4> enc;        // просто кнопка <KEY>
// для изменения направления энкодера поменяй A и B при инициализации

// по умолчанию пины настроены в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
//EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  Serial.begin(9600);
  // настройки
  //enc.setButtonLevel(HIGH);     // уровень кнопки: LOW - кнопка подключает GND (по умолч.), HIGH - кнопка подключает VCC
  //enc.setHoldTimeout(1000);     // установить время удержания кнопки, мс (до 8 000)
  //enc.setStepTimeout(500);      // установить период импульсов step, мс (до 4 000)

  //enc.holdEncButton(true);      // виртуально зажать кнопку энкодера (для срабатывания нажатых поворотов)
  //enc.setEncReverse(true);      // true - инвертировать направление энкодера (умолч. false)
  //enc.setEncType(EB_HALFSTEP);  // тип энкодера: EB_FULLSTEP (0) по умолч., EB_HALFSTEP (1) если энкодер делает один поворот за два щелчка
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
    Serial.println(enc.dir());      // направление поворота
  }

  // "нажатый поворот"
  if (enc.turnH()) {
    Serial.println("hold + turn");

    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
    Serial.println(enc.dir());      // направление поворота
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
