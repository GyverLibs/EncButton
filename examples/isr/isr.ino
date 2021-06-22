// Пример с прямой работой библиотеки на прерываниях

// Опциональные дефайн-настройки (показаны по умолчанию)
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 80      // дебаунс кнопки, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс

// подключение - только PULL-UP, внешний или внутренний!
// для изменения направления энкодера поменяй A и B при инициализации

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc;   // энкодер с кнопкой <A, B, KEY>
//EncButton<EB_TICK, 2, 3> enc;    // просто энкодер <A, B>
//EncButton<EB_TICK, 4> enc;       // просто кнопка <KEY>

// по умолчанию пины настроены в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
//EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  Serial.begin(9600);

  // желательно подключить оба пина энкодера на внешние прерывания по CHANGE
  // можно использовать PCINT https://github.com/NicoHood/PinChangeInterrupt
  attachInterrupt(0, isr, CHANGE);  // D2
  attachInterrupt(1, isr, CHANGE);  // D3
  // подключил оба прерывания на одну функцию
}

// в прерывании вызываем тик
void isr() {
  enc.tick();
}

void loop() {
  // тут тоже вызываем тик, нужен для 
  // корректной работы дебаунсов и прочих таймеров!!!
  enc.tick();

  if (enc.isTurn()) {               // любой поворот
    Serial.print("turn ");
    Serial.println(enc.counter);    // вывод счётчика
  }
  
  // имитация загруженной программы, обработка происходит в прерывании
  delay(50);
}
