![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# EncButton
Ультра лёгкая и быстрая библиотека для энкодера, энкодера с кнопкой или просто кнопки
- Максимально быстрое чтение пинов для AVR (ATmega328/ATmega168, ATtiny85/ATtiny13)
- Оптимизированный вес
- Быстрые и лёгкие алгоритмы кнопки и энкодера
- Энкодер: поворот, нажатый поворот, быстрый поворот, счётчик
- Кнопка: антидребезг, клик, несколько кликов, счётчик кликов, удержание, режим step
- Подключение - **только с подтяжкой к питанию** (внешней или внутренней)!
- Опциональный режим callback (+22б SRAM на каждый экземпляр)
- Виртуальный режим (кнопка, энк, энк с кнопкой)

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **EncButton** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/EncButton/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
EncButton<MODE, A, B, KEY> enc;     // энкодер с кнопкой и пинами A, B, KEY
EncButton<MODE, A, B> enc;          // просто энкодер с пинами A, B
EncButton<MODE, KEY> btn;           // просто кнопка с пином KEY
// MODE: EB_TICK или EB_CALLBACK - режим работы ручной или с обработчиками

// по умолчанию пины настроены в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

// Виртуальный режим:
EncButton<MODE, VIRT_BTN> enc;     // виртуальная кнопка
EncButton<MODE, VIRT_ENCBTN> enc;  // виртуальный энк с кнопкой
EncButton<MODE, VIRT_ENC> enc;     // виртуальный энк
// в tick нужно передавать виртуальное значение, см. ниже!
```

<a id="usage"></a>
## Использование
```cpp
void pullUp();                  // подтянуть пины внутренней подтяжкой
void holdEncButton(bool state); // виртуально зажать кнопку энкодера

// тикер, вызывать как можно чаще или в прерывании
// вернёт отличное от нуля значение, если произошло какое то событие
uint8_t tick();

// tick может принимать виртуальный сигнал при режиме VIRT_:
// (сигнал кнопки)
// (сигнал энкодера А, сигнал энкодера B)
// (сигнал энкодера А, сигнал энкодера B, сигнал кнопки)

uint8_t getState();     // получить статус
void resetState();      // сбросить статус

// обработка энкодера
bool isTurn();          // факт поворота
bool isRight();         // вправо
bool isLeft();          // влево
bool isRightH();        // вправо нажатый
bool isLeftH();	        // влево нажатый
bool isFast();          // быстрый
int counter;            // доступ к счётчику энкодера

// обработка кнопки
bool isPress();         // нажатие
bool isRelease();       // отпускание
bool isClick();         // клик
bool isHolded();        // было удержание (однократное срабатывание)
bool isHold();          // удерживается (постоянное срабатывание)
bool isStep();          // режим шага
bool state();           // текущий статус кнопки
bool hasClicks(uint8_t numClicks);  // проверка на наличие указанного количества кликов
uint8_t hasClicks();    // вернёт количество кликов, если они есть
uint8_t clicks;         // доступ к счётчику кликов

// для режима с обработчиком
void attach(eb_callback type, void (*handler)());       // подключить обработчик
void detach(eb_callback type);                          // отключить обработчик
void attachClicks(uint8_t amount, void (*handler)());   // подключить обработчик на количество кликов (может быть только один!)
void detachClicks();                                    // отключить обработчик на количество кликов

// eb_callback может быть:
TURN_HANDLER
RIGHT_HANDLER
LEFT_HANDLER
RIGHT_H_HANDLER
LEFT_H_HANDLER
CLICK_HANDLER
HOLDED_HANDLER
STEP_HANDLER
HOLD_HANDLER
CLICKS_HANDLER
PRESS_HANDLER
RELEASE_HANDLER
// СМОТРИ ПРИМЕР

// дефайны настроек (дефайнить ПЕРЕД ПОДКЛЮЧЕНИЕМ БИБЛИОТЕКИ)
// показаны значения по умолчанию
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 80      // дебаунс кнопки, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
// Пример с прямой работой библиотеки

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc;    // энкодер с кнопкой <A, B, KEY>

void setup() {
  Serial.begin(9600);
  enc.counter = 100;      // изменение счётчика
}

void loop() {
  enc.tick();

  if (enc.isTurn()) {               // любой поворот
    Serial.print("turn ");
    Serial.println(enc.counter);    // вывод счётчика
  }

  if (enc.isLeft()) {
    if (enc.isFast()) Serial.println("fast left");
    else Serial.println("left");
  }

  if (enc.isRight()) {
    if (enc.isFast()) Serial.println("fast right");
    else Serial.println("right");
  }

  if (enc.isLeftH()) Serial.println("leftH");
  if (enc.isRightH()) Serial.println("rightH");
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
```

<a id="versions"></a>
## Версии
- v1.1 - пуллап отдельныи методом
- v1.2 - можно передать конструктору параметр INPUT_PULLUP / INPUT(умолч)
- v1.3 - виртуальное зажатие кнопки энкодера вынесено в отдельную функцию + мелкие улучшения
- v1.4 - обработка нажатия и отпускания кнопки
- v1.5 - добавлен виртуальный режим
- v1.6 - оптимизация работы в прерывании
- v1.6.1 - подтяжка по умолчанию INPUT_PULLUP

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!