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
- [Железо](#hardware)
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

<a id="hardware"></a>
## Железо
Для работы по сценарию "энкодер с кнопкой" рекомендую вот такие ([ссылка](https://ali.ski/CYir4), [ссылка](https://ali.ski/49q5hy)) круглые китайские модули с распаянными цепями антидребезга:  
![scheme](/doc/encAli.png)

Самостоятельно обвязать энкодер можно по следующей схеме (RC фильтры на каналы энкодера + подтяжка всех пинов к VCC):  
![scheme](/doc/enc.png)

<a id="init"></a>
## Инициализация
**Если нужен массив кнопок/энкодеров, используй EncButton2!**

### EncButton
```cpp
// ============== БАЗОВАЯ =============
EncButton<MODE, A, B, KEY> enc;     // энкодер с кнопкой
EncButton<MODE, A, B> enc;          // просто энкодер
EncButton<MODE, KEY> btn;           // просто кнопка
// A, B, KEY: номера пинов
// MODE: EB_TICK или EB_CALLBACK - режим работы ручной или с обработчиками
// для изменения направления энкодера поменяй A и B при инициализации

// ============ ПОДКЛЮЧЕНИЕ ============
// По умолчанию пины настраиваются в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
EncButton<...> enc(INPUT);

// ========= ВИРТУАЛЬНЫЙ РЕЖИМ =========
EncButton<MODE, VIRT_BTN> enc;     // виртуальная кнопка
EncButton<MODE, VIRT_ENCBTN> enc;  // виртуальный энк с кнопкой
EncButton<MODE, VIRT_ENC> enc;     // виртуальный энк
// в tick нужно будет передавать виртуальное значение, см. пример
```

### EncButton2
<details>
<summary>Инициализация EncButton2</summary>

Хранит пины НЕ в шаблоне. Почему то получилось тяжелее, ну да ладно. Всё кроме инициализации такое же как у EncButton!
```cpp
// ================ TICK ===============
EncButton2<EB_ENCBTN> enc(INPUT, A, B, KEY);    // энкодер с кнопкой
EncButton2<EB_ENC> enc(INPUT, A, B);            // просто энкодер
EncButton2<EB_BTN> enc(INPUT, KEY);             // просто кнопка
// режим пинов INPUT/INPUT_PULLUP

// ============== CALLBACK =============
EncButton2<EB_ENCBTN, EB_CALLBACK> enc(INPUT, A, B, KEY);    // энкодер с кнопкой
EncButton2<EB_ENC, EB_CALLBACK> enc(INPUT, A, B);            // просто энкодер
EncButton2<EB_BTN, EB_CALLBACK> enc(INPUT, KEY);             // просто кнопка
// режим пинов INPUT/INPUT_PULLUP

// ============== VIRT TICK ============
EncButton2<VIRT_ENCBTN> enc;        // энкодер с кнопкой
EncButton2<VIRT_ENC> enc;           // просто энкодер
EncButton2<VIRT_BTN> enc;           // просто кнопка

// ============ VIRT CALLBACK ==========
EncButton2<VIRT_ENCBTN, EB_CALLBACK> enc;   // энкодер с кнопкой
EncButton2<VIRT_ENC, EB_CALLBACK> enc;      // просто энкодер
EncButton2<VIRT_BTN, EB_CALLBACK> enc;      // просто кнопка

// =============== МАССИВ ==============
EncButton2<EB_ENCBTN> enc[количество];
EncButton2<EB_ENC> enc[количество];
EncButton2<EB_BTN> enc[количество];

EncButton2<EB_ENCBTN, EB_CALLBACK> enc[количество];
EncButton2<EB_ENC, EB_CALLBACK> enc[количество];
EncButton2<EB_BTN, EB_CALLBACK> enc[количество];
// и так далее

// ПИНЫ НАСТРАИВАЮТСЯ ПРИ ПОМОЩИ setPins(uint8_t mode, uint8_t P1, uint8_t P2, uint8_t P3);
// см. пример EucButton2_array
```

</details>

<a id="usage"></a>
## Использование
```cpp
// =============== SETTINGS ==============
void pullUp();                      // подтянуть все пины внутренней подтяжкой
void holdEncButton(bool state);     // виртуально зажать кнопку энкодера
void setHoldTimeout(int tout);      // установить время удержания кнопки, мс (до 30 000)
void setButtonLevel(bool level);    // уровень кнопки: LOW - кнопка подключает GND (по умолч.), HIGH - кнопка подключает VCC

// ================= TICK ================
// тикер, вызывать как можно чаще или в прерывании
// вернёт отличное от нуля значение, если произошло какое то событие
uint8_t tick();

// tick(uint8_t s1 = 0, uint8_t s2 = 0, uint8_t key = 0)
// может принимать виртуальный сигнал при режиме VIRT_:
// (сигнал кнопки)
// (сигнал энкодера А, сигнал энкодера B)
// (сигнал энкодера А, сигнал энкодера B, сигнал кнопки)

uint8_t tickISR();      // тикер для прерывания при режиме callback. Не вызывает подключенные функции
void checkCallback();   // проверяет и вызывает подключенные функции для режима callback

uint8_t getState();     // получить статус
void resetState();      // сбросить статус

// =============== ENCODER ===============
bool turn();        // факт поворота
bool turnH();       // факт нажатого поворота
bool right();       // вправо
bool left();        // влево
bool rightH();      // вправо нажатый
bool leftH();       // влево нажатый
bool fast();        // быстрый
int8_t getDir();    // направление последнего поворота, 1 или -1
int counter;        // доступ к счётчику энкодера

// ================ BUTTON ================
bool press();       // кнопка нажата
bool release();     // кнопка отпущена
bool click();       // клик
bool held();        // было удержание (однократное срабатывание)
bool hold();        // удерживается (постоянное срабатывание)
bool step();        // режим импульсного удержания
bool releaseStep(); // отпущена после режима step
bool state();       // текущий статус кнопки
uint8_t clicks;     // доступ к счётчику кликов
uint8_t hasClicks();            // вернёт количество кликов, если они есть
bool hasClicks(uint8_t num);    // проверка на наличие указанного количества кликов

// =============== CALLBACK ===============
void attach(eb_callback type, void (*handler)());       // подключить обработчик
void detach(eb_callback type);                          // отключить обработчик
void attachClicks(uint8_t amount, void (*handler)());   // подключить обработчик на количество кликов (может быть только один!)
void detachClicks();                                    // отключить обработчик на количество кликов

// eb_callback может быть:
TURN_HANDLER
TURN_H_HANDLER
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

// =============== DEFINES ===============
// дефайны настроек (дефайнить ПЕРЕД ПОДКЛЮЧЕНИЕМ БИБЛИОТЕКИ)
// показаны значения по умолчанию
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 50      // дебаунс кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки (устанавливает значение по умолчанию), мс
//#define EB_BETTER_ENC  // улучшенный алгоритм опроса энкодера. Добавит 16 байт SRAM при подключении библиотеки
```
### Дополнительно EncButton2
```cpp
void setPins(uint8_t mode, uint8_t P1, uint8_t P2, uint8_t P3);     // установить пины
void pullUp();          // здесь не реализована!
```

### Особенности
- Библиотека универсальная, но сделана с упором на оптимизацию памяти при работе во всех режимах внутри одного класса
    - При создании объекта с разным количеством пинов (энкодер, кнопка, энкодер с кнопкой) библиотека будет компилироваться по разному, ненужный код будет вырезан. Это позволяет экономить Flash память.
    - То же самое касается режимов работы TICK/CALLBACK, при использовании TICK весь относящийся к CALLBACK код вырезается компилятором
- Два алгоритма опроса энкодера, обычный и точный. Точный использует на 16 байт больше SRAM памяти (на всю библиотеку), но позволяет работать даже с низкокачественными и убитыми энкодерами
    - Точный алгоритм активируется добавлением `#define EB_BETTER_ENC` перед подключением библиотеки

<a id="example"></a>
## Примеры
### Полное демо tick
Остальные примеры смотри в **examples**!
```cpp
// Пример с прямой работой библиотеки

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc;  // энкодер с кнопкой <A, B, KEY>
//EncButton<EB_TICK, 2, 3> enc;     // просто энкодер <A, B>
//EncButton<EB_TICK, 4> enc;        // просто кнопка <KEY>

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
  if (enc.releaseStep()) Serial.println("release step");  // отпущена после импульсного удержания
  
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
```

### Массив кнопок EncButton2
```cpp
// объявляем массив кнопок
#define BTN_AMOUNT 5
#include <EncButton2.h>
EncButton2<EB_BTN> btn[BTN_AMOUNT];

void setup() {
  Serial.begin(9600);
  btn[0].setPins(INPUT_PULLUP, D3);
  btn[1].setPins(INPUT_PULLUP, D2);
}

void loop() {
  for (int i = 0; i < BTN_AMOUNT; i++) btn[i].tick();
  for (int i = 0; i < BTN_AMOUNT; i++) {
    if (btn[i].click()) {
      Serial.print("click btn: ");
      Serial.println(i);
    }
  }
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
- v1.7 - большая оптимизация памяти, переделан FastIO
- v1.8 - индивидуальная настройка таймаута удержания кнопки (была общая на всех)
- v1.8.1 - убран FastIO
- v1.9 - добавлена отдельная отработка нажатого поворота и запрос направления
- v1.10 - улучшил обработку released, облегчил вес в режиме callback и исправил баги
- v1.11 - ещё больше всякой оптимизации + настройка уровня кнопки
- v1.11.1 - совместимость Digispark
- v1.12 - добавил более точный алгоритм энкодера EB_BETTER_ENC
- v1.13 - добавлен экспериментальный EncButton2
- v1.14 - добавлена releaseStep(). Отпускание кнопки внесено в дебаунс
- v1.15 - добавлен setPins() для EncButton2

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
