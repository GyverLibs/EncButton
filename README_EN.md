This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/EncButton.svg?color=brightgreen)](https://github.com/GyverLibs/EncButton/releases/latest/download/EncButton.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/EncButton.svg)](https://registry.platformio.org/libraries/gyverlibs/EncButton)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/EncButton?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# EncButton

| и️и️и️<br>**The new version of v3 is incompatible with previous versions.[documentation](#docs), [examples](#example)brief[migration guide](#migrate)from v2 to v3!**<br> ️ ️ ️|
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Lightweight and very functional library for encoder with button, encoder or button with Arduino
- Button
    - Event handling: press, release, click, click counter, retention, impulse retention, retention time + pre-clicks for all modes
    - Programming suppression of rattles
    - Supports processing of two simultaneously pressed buttons as a third button
- encoder
    - Event handling: ordinary turn, pressed turn, fast turn
    - Support for four types of incremental encoders
    - High-precision positioning algorithm
    - Buffering in interruption
- Simple and understandable use
- A huge number of possibilities and their combinations for different scenarios using even one button
- Virtual mode (for example, to work with a pin expander)
- Optimized for work in interruption
- Fastest pin reading for AVR, esp8266, esp32 (used by GyverIO)
- Fast asynchronous algorithms for questioning actions from a button and encoder
- Hard optimization and low weight in Flash and SRAM memory: 5 bytes of SRAM (per instance) and ~350 bytes of Flash button processing

Examples of use cases:
- A few clicks - enable the mode (by number of clicks)
- A few clicks + short retention - another option to enable the mode (by the number of clicks)
- Multiple clicks + retention - a gradual change in the value of the selected variable (by the number of clicks)
- A few clicks select a variable, the encoder changes it.
- Changing the step of changing the variable as the encoder rotates - for example, reducing when the button is pressed and increasing when the fast rotation
- Navigation on the menu when rotating the encoder, changing the variable when rotating the clamped encoder
- Full navigation on the menu using two buttons (simultaneous retention to move to the next level, simultaneous pressing to return to the previous one)
- And so on.

Weight in Bytes compared to other libraries (survey of five button events):

|              | [uButton](https://github.com/GyverLibs/uButton) | [EncButton](https://github.com/GyverLibs/EncButton) | [EncButton opti](https://github.com/GyverLibs/EncButton) | [GyverButton](https://github.com/GyverLibs/GyverButton) | [OneButton](https://github.com/mathertel/OneButton) | [OneButtonTiny](https://github.com/mathertel/OneButton) | [JC_Button](https://github.com/JChristensen/JC_Button) | [AceButton](https://github.com/bxparks/AceButton)|
----------------------------------------------------------------------------------------
| Flash | 776 | 1006 | 610 | 1054 | 1838 | 1152 | 650 | 1482 |
| SRAM | 6 | 15 | 6 | 23 | 83 | 36 | 24 | 45 |
| Feech | Very Many (20+) | Very Many (20+) | Very Many (20+) | Very Many (13) | Average (8) | Very Little (4) | Extremely Little (6) | Average (7|)

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Installation](#install)
- [Information](#info)
- [Documentation.](#docs)
  - [Compilation settings](#config)
  - [Full description of classes](#class)
  - [Processing and interviewing](#tick)
  - [Preliminary clicks](#preclicks)
  - [Direct reading of the button](#btnread)
  - [Immersion in a cycle](#loop)
  - [Timeout](#timeout)
  - [Busy](#busy)
  - [Receiving the event](#actions)
  - [Optimization](#optimise)
  - [Collbackie.](#callback)
  - [Simultaneous press](#double)
  - [Interruptions](#isr)
  - [Array of buttons/encoders](#array)
  - [Custom functions](#custom)
  - [Timer survey](#timer)
  - [Mini examples, scripts.](#examples-mini)
- [Migration from v2](#migrate)
- [Examples](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>

## Installation
- A library is required to work[GyverIO](https://github.com/GyverLibs/GyverIO)
- The library can be found under the name **EncButton** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/EncButton/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="info"></a>

## Information
### encoder
#### Type of encoder
The library supports all 4 types of *incremental* encoders, the type can be configured using`setEncType(тип)`:
- `EB_STEP4_LOW`Active low signal (VCC lift) Full period (4 phases) in one click. *Installed by default *
- `EB_STEP4_HIGH`Active high signal (lift to GND). Full period (4 phases) in one click
- `EB_STEP2`Half the period (2 phases) in one click
- `EB_STEP1`a quarter of the period (1 phase) in one click, as well as encoders without fixation

![diagram](/doc/enc_type.png)  

#### Recommendations
To work on the script "encoder with a button" I recommend these ([reference](https://ali.ski/cmPI2), [reference](https://ali.ski/sZbTK)) round Chinese modules with soldered anti-drop chains (have the type`EB_STEP4_LOW`under classification above:
![scheme](/doc/encAli.png)  

You can tie the encoder yourself according to the following scheme (RC filters on the encoder channels + lifting all pins to the VCC):
![scheme](/doc/enc_scheme.png)  

> Note: by default, encoder pins in the library are configured to`INPUT`With an external lift. If you have an encoder without a lift, you can use an internal one.`INPUT_PULLUP`, specifying this when initializing the encoder (see documentation below).

### Button
#### The button level
The button can be connected to the microcontroller in two ways and give a high or low signal when pressed. There is a setup in the library`setBtnLevel(уровень)`where the level is the active button signal:
- `HIGH`- The button connects the VCC. Installed by default`Virt`- libraries
- `LOW`- The button connects the GND. Installed by default in major libraries

![scheme](/doc/btn_scheme.png)  

#### Pin lift
In microcontroller circuits, the most common use is to connect a button to the GND with a pin lift to the VCC. The lift can be external (the pin mode must be set).`INPUT`) or internal (pin mode)`INPUT_PULLUP`). In "real" projects, an external lift is recommended, since it is less prone to interference - the internal resistance is too high.

<a id="docs"></a>

## Documentation.

<a id="config"></a>

### Define settings
Announce before connecting the library

```cpp

// disable support for pressFor/holdFor/stepFor and step counter (saves 2 bytes of RAM)
#define EB_NO_FOR

// disable event handler attach (saves 2 bytes of RAM)
#define EB_NO_CALLBACK

// disable the encoder counter [VirtEncoder, Encoder, EncButton] (saves 4 bytes of RAM)
#define EB_NO_COUNTER

// disable encoder buffering (saves 2 bytes of RAM)
#define EB_NO_BUFFER

/*
  Настройка таймаутов для всех классов
  - Заменяет таймауты константами, изменить их из программы (SetXxxTimeout()) будет нельзя
  - Настройка влияет на все объявленные в программе кнопки/энкодеры
  - Экономит 1 байт оперативки на объект за каждый таймаут
  - Показаны значения по умолчанию в мс
  - Значения не ограничены 4000мс, как при установке из программы (SetXxxTimeout())
*/
#define EB_DEB_TIME 50      // timeout of the rattle button (button)
#define EB_CLICK_TIME 500   // Timeout waiting for clicks (button)
#define EB_HOLD_TIME 600    // retention timeout (button)
#define EB_STEP_TIME 200    // impulse retention timeout (button)
#define EB_FAST_TIME 30     // Quick turn timeout (encoder)
#define EB_TOUT_TIME 1000   // Action timeout (button and encoder)
```

<a id="class"></a>

### Classes
Starting with version 3.0, EncButton has several libraries (classes) for different use cases, they inherit each other for enhanced functionality. Thus, the library is an onion, each layer of which has access to the functions of the lower layers:
- Basic classes:
  - `VirtButton`The basic class of the virtual button provides all the features of the button.
  - `VirtEncoder`The basic class of the virtual encoder, determines the fact and direction of the encoder rotation
  - `VirtEncButton`- the base class of the virtual encoder with a button, provides a poll of the encoder taking into account the button, *inherits VirtButton and VirtEncoder*
- Main classes:
  - `Button`, `ButtonT`- button class, *inherit VirtButton *
  - `Encoder`, `EncoderT`- encoder class, *inherits VirtEncoder*
  - `EncButton`, `EncButtonT`Encoder class with button, *inherits VirtEncButton, VirtButton, VirtEncoder*

Thus, to study all the available functions of a particular library, you need to look not only at it, but also what it inherits. For example, to process a button using`Button`You should open the description below.`Button`and`VirtButton`.

> *Virtual* - without specifying the pin of the microcontroller, works directly with the transmitted value, for example, to poll button encoders through pin extenders and shift registers.

> `T`Libraries require pins to be specified with constants (digits). The pin numbers will be stored in the program memory, which speeds up work and makes the code 1 byte lighter for each pin.

> Note:`#include <EncButton.h>`It connects all the library tools!

<details >>
<summary>Table of button functions</summary >>

|                   | VirtButton | VirtEncButton | Button | EncButton |
| ----------------- | :--------: | :-----------: | :----: | :-------: |
| read              |            |               |   ✔    |           |
| readBtn           |            |               |        |     ✔     |
| tickRaw           |     ✔      |       ✔       |   ✔    |     ✔     |
| setHoldTimeout    |     ✔      |       ✔       |   ✔    |     ✔     |
| setStepTimeout    |     ✔      |       ✔       |   ✔    |     ✔     |
| setClickTimeout   |     ✔      |       ✔       |   ✔    |     ✔     |
| setDebTimeout     |     ✔      |       ✔       |   ✔    |     ✔     |
| setTimeout        |     ✔      |       ✔       |   ✔    |     ✔     |
| setBtnLevel       |     ✔      |       ✔       |   ✔    |     ✔     |
| pressISR          |     ✔      |       ✔       |   ✔    |     ✔     |
| reset             |     ✔      |       ✔       |   ✔    |     ✔     |
| clear             |     ✔      |       ✔       |   ✔    |     ✔     |
| skipEvents        |     ✔      |       ✔       |   ✔    |     ✔     |
| attach            |     ✔      |       ✔       |   ✔    |     ✔     |
| detach            |     ✔      |       ✔       |   ✔    |     ✔     |
| press             |     ✔      |       ✔       |   ✔    |     ✔     |
| release           |     ✔      |       ✔       |   ✔    |     ✔     |
| click             |     ✔      |       ✔       |   ✔    |     ✔     |
| pressing          |     ✔      |       ✔       |   ✔    |     ✔     |
| hold              |     ✔      |       ✔       |   ✔    |     ✔     |
| holding           |     ✔      |       ✔       |   ✔    |     ✔     |
| step              |     ✔      |       ✔       |   ✔    |     ✔     |
| hasClicks         |     ✔      |       ✔       |   ✔    |     ✔     |
| getClicks         |     ✔      |       ✔       |   ✔    |     ✔     |
| getSteps          |     ✔      |       ✔       |   ✔    |     ✔     |
| releaseHold       |     ✔      |       ✔       |   ✔    |     ✔     |
| releaseStep       |     ✔      |       ✔       |   ✔    |     ✔     |
| releaseHoldStep   |     ✔      |       ✔       |   ✔    |     ✔     |
| waiting           |     ✔      |       ✔       |   ✔    |     ✔     |
| busy              |     ✔      |       ✔       |   ✔    |     ✔     |
| action            |     ✔      |       ✔       |   ✔    |     ✔     |
| getAction         |     ✔      |       ✔       |   ✔    |     ✔     |
| timeout           |     ✔      |       ✔       |   ✔    |     ✔     |
| pressFor          |     ✔      |       ✔       |   ✔    |     ✔     |
| holdFor           |     ✔      |       ✔       |   ✔    |     ✔     |
| stepFor           |     ✔      |       ✔       |   ✔    |     ✔     |
</details>

<details >>
<summary>Table of encoder functions</summary >>

|                | VirtEncoder | Encoder | VirtEncButton | EncButton |
| -------------- | :---------: | :-----: | :-----------: | :-------: |
| initEnc        |      ✔      |    ✔    |       ✔       |     ✔     |
| setEncReverse  |      ✔      |    ✔    |       ✔       |     ✔     |
| setEncType     |      ✔      |    ✔    |       ✔       |     ✔     |
| setEncISR      |      ✔      |    ✔    |       ✔       |     ✔     |
| clear          |      ✔      |    ✔    |       ✔       |     ✔     |
| turn           |      ✔      |    ✔    |       ✔       |     ✔     |
| dir            |      ✔      |    ✔    |       ✔       |     ✔     |
| tickRaw        |      ✔      |    ✔    |       ✔       |     ✔     |
| pollEnc        |      ✔      |    ✔    |       ✔       |     ✔     |
| counter        |      ✔      |    ✔    |       ✔       |     ✔     |
| setFastTimeout |             |         |       ✔       |     ✔     |
| turnH          |             |         |       ✔       |     ✔     |
| fast           |             |         |       ✔       |     ✔     |
| right          |             |         |       ✔       |     ✔     |
| left           |             |         |       ✔       |     ✔     |
| rightH         |             |         |       ✔       |     ✔     |
| leftH          |             |         |       ✔       |     ✔     |
| action         |             |         |       ✔       |     ✔     |
| getAction      |             |         |       ✔       |     ✔     |
| timeout        |             |         |       ✔       |     ✔     |
| attach         |             |         |       ✔       |     ✔     |
| detach         |             |         |       ✔       |     ✔     |
</details>

<details>
<summary>VirtButton</summary>

```cpp
// ===================================================
// Set a retention timeout, shut up. 600 (max. 4,000 ms)
void setHoldTimeout(uint16_t tout);

// Set impulse retention timeout, shut up. 200 (max. 4,000 ms)
void setStepTimeout(uint16_t tout);

// Set a timeout for clicks, shut up. 500 (max. 4,000 ms)
void setClickTimeout(uint16_t tout);

// Set an anti-fuck timeout, shut up. 50 (max. 255 ms)
void setDebTimeout(uint8_t tout);

// Set a timeout for timeout(), keep quiet. 1000 (max. 4,000 ms)
void setTimeout(const uint16_t tout);

// set the button level (HIGH - button closes VCC, LOW - closes GND)
// Shut up. HIGH, i.e. true - button pressed
void setBtnLevel(bool level);

// plug-in
void attach(void (*handler)());

// disable the event-processor function
void detach();

// =======================================================
// Reset system flags (forced processing)
void reset();

// forcefully drop flags of events
void clear(bool resetTout = false);

// ignore all events before releasing the button
void skipEvents();

// ======================================================
// button-handling n
bool tick(bool s);

// processing a virtual button as simultaneously pressing two other buttons
bool tick(VirtButton& b0, VirtButton& b1);

// push-button
void pressISR();

// processing the button without resetting events and calling callback
bool tickRaw(bool s);

// ============================================================
// button pressed [event]
bool press();
bool press(uint8_t clicks);

// button released (in any case) [event]
bool release();
bool release(uint8_t clicks);

// Click on the button (released without retention) [event]
bool click();
bool click(uint8_t clicks);

// button pressed (between press() and release()) [state]
bool pressing();
bool pressing(uint8_t clicks);

// The button was held (more timeout) [event]
bool hold();
bool hold(uint8_t clicks);

// The button is held (more timeout) [state]
bool holding();
bool holding(uint8_t clicks);

// impulse retention [event]
bool step();
bool step(uint8_t clicks);

// A few clicks were recorded [event]
bool hasClicks();
bool hasClicks(uint8_t clicks);

// Button released after holding [event]
bool releaseHold();
bool releaseHold(uint8_t clicks);

// button released after impulse retention [event]
bool releaseStep();
bool releaseStep(uint8_t clicks);

// button released after holding or impulse holding [event]
bool releaseHoldStep();
bool releaseHoldStep(uint8_t clicks);

// click
uint8_t getClicks();

// steppe
uint16_t getSteps();

// The button awaits repeated clicks (between click() and hasClick())
bool waiting();

// processing (between the first click and after waiting for clicks)
bool busy();

// was an action from a button, will return the event code [event]
uint16_t action();
EBAction getAction();

// ==========================================================
// after interacting with the button (or EncButton encoder) setTimeout time, MS [event]
bool timeout();

// after interacting with the button (or EncButton encoder) setTimeout time, MS [state]
bool timeoutState();

// time that the button is held (from the beginning of pressing), MS
uint16_t pressFor();

// The button is held longer than (from the beginning of pressing), ms [state]
bool pressFor(uint16_t ms);

// time that the button is held (from the beginning of retention), MS
uint16_t holdFor();

// The button is held longer than (from the beginning of retention), ms [state]
bool holdFor(uint16_t ms);

// time that the button is held (from the beginning of the step), MS
uint16_t stepFor();

// The button is held longer than (from the beginning of the steppe), MS [state]
bool stepFor(uint16_t ms);
```
</details>
<details>
<summary>VirtEncoder</summary>

```cpp
// =============================================================
// invert the direction of the encoder (no. 0)
void setEncReverse(bool rev);

// set the encoder type (EB STEP4 LOW, EB STEP4 HIGH, EB STEP2, EB STEP1)
void setEncType(uint8_t type);

// use encoder processing in interruption
void setEncISR(bool use);

// initialization
void initEnc(bool e0, bool e1);

// initialization of the encoder with a combined value
void initEnc(int8_t v);

// drop off event flags
void clear();

// =======================================================
// It was a turnaround.
bool turn();

// direction of encoder (1 or -1) [state]
int8_t dir();

// counter
int32_t counter;

// ================================================================
// Interrogate the encoder in interruption. Return 1 or -1 when rotating, 0 when stopping
int8_t tickISR(bool e0, bool e1);

// interview the encoder. Return 1 or -1 when rotating, 0 when stopping
int8_t tick(bool e0, bool e1);
int8_t tick();  // interruption

// Interview the encoder without resetting the turn event. Return 1 or -1 when rotating, 0 when stopping
int8_t tickRaw(bool e0, bool e1);
int8_t tickRaw();  // interruption

// Interrogate the encoder without setting flags to turn (faster). Return 1 or -1 when rotating, 0 when stopping
int8_t pollEnc(bool e0, bool e1);
```
</details>
<details>
<summary>VirtEncButton</summary>

- Functions available from`VirtButton`
- Functions available from`VirtEncoder`

```cpp
// =======================================================
// set a timeout for a quick turn, ms.
void setFastTimeout(uint8_t tout);

// drop encoder flags and buttons
void clear(bool resetTout = false);

// =======================================================
// Any turn of the encoder [event]
bool turn();

// The Encoder's Turn [Event]
bool turnH();

// fast turn of the encoder [state]
bool fast();

// unpressed turn to the right [event]
bool right();

// unpressed turn to the left [event]
bool left();

// turn to the right [event]
bool rightH();

// turn to the left [event]
bool leftH();

// an action from a button or encoder will return the event code
uint16_t action();
EBAction getAction();

// ================================================================
// Interruption processing (encoder only). Return 0 at rest, 1 or -1 when turning
int8_t tickISR(bool e0, bool e1);

// encoder processing
bool tick(bool e0, bool e1, bool btn);
bool tick(bool btn);  // interrupter

// handling the encoder and button without dropping flags and calling a callback
bool tickRaw(bool e0, bool e1, bool btn);
bool tickRaw(bool btn);  // interrupter
```
</details>
<details>
<summary>Button</summary>

- Functions available from`VirtButton`
- Default button mode -`LOW`

```cpp
Button;
Button(uint8_t pin);                // pinpoint
Button(uint8_t npin, uint8_t mode); // + mode of operation (silent. INPUT PULLUP)
Button(uint8_t npin, uint8_t mode, uint8_t btnLevel); // + button level (silent. LOW)
```
```cpp
// specify the PIN and its mode of operation
void init(uint8_t npin, uint8_t mode);

// read the current value of the button (without debunking) taking into account setBtnLevel
bool read();

// processing function, call to loop
bool tick();

// processing the button without resetting events and calling callback
bool tickRaw();

// pin
uint8_t getPin();
```
</details>
<details>
<summary>ButtonT</summary>

- Functions available from`VirtButton`
- Default button mode -`LOW`

```cpp
ButtonT<uint8_t pin>;                 // pinpoint
ButtonT<uint8_t pin> (uint8_t mode);  // + mode of operation (silent. INPUT PULLUP)
ButtonT<uint8_t pin> (uint8_t mode, uint8_t btnLevel); // + button level (silent. LOW)
```
```cpp
// schedule
void init(uint8_t mode);

// read the current value of the button (without debunking) taking into account setBtnLevel
bool read();

// processing function, call to loop
bool tick();
```
</details>
<details>
<summary>Encoder</summary>

- Functions available from`VirtEncoder`

```cpp
Encoder;
Encoder(uint8_t encA, uint8_t encB);                // pinpoint
Encoder(uint8_t encA, uint8_t encB, uint8_t mode);  // + mode of operation (silent. INPUT)
```
```cpp
// specify pins and their mode of operation
void init(uint8_t encA, uint8_t encB, uint8_t mode);

// processing function for calling in an encoder interrupt
int8_t tickISR();

// processing function for call to loop
int8_t tick();

// pin
uint8_t getPinA();

// pin
uint8_t getPinB();
```
</details>
<details>
<summary>EncoderT</summary>

- Functions available from`VirtEncoder`

```cpp
EncoderT<uint8_t encA, uint8_t encB>;                 // pinpoint
EncoderT<uint8_t encA, uint8_t encB> (uint8_t mode);  // + mode of operation (silent. INPUT)
```
```cpp
// pinpoint
void init(uint8_t mode);

// processing function for calling in an encoder interrupt
int8_t tickISR();

// processing function for call to loop
int8_t tick();
```
</details>
<details>
<summary>EncButton</summary>

- Functions available from`VirtButton`
- Functions available from`VirtEncoder`
- Functions available from`VirtEncButton`

```cpp
EncButton;

// set up pins (enck, enck, button)
EncButton(uint8_t encA, uint8_t encB, uint8_t btn);

// set up pins (enc, enc, button, pinmode enc, pinmode button, button level)
EncButton(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);
```
```cpp
// set up pins (enc, enc, button, pinmode enc, pinmode button, button level)
void init(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);

// processing function for calling in an encoder interrupt
int8_t tickISR();

// processing function, call to loop
bool tick();

// Read the button value with setBtnLevel in mind
bool readBtn();

// pin
uint8_t getPin();

// pin
uint8_t getPinA();

// pin
uint8_t getPinB();
```
</details>
<details>
<summary>EncButtonT</summary>

- Functions available from`VirtButton`
- Functions available from`VirtEncoder`
- Functions available from`VirtEncButton`

```cpp
// pinpoint
EncButtonT<uint8_t encA, uint8_t encB, uint8_t btn>;

// + mode of operation of pins, button level
EncButtonT<uint8_t encA, uint8_t encB, uint8_t btn> (uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);
```
```cpp
// adjust the operation of pins, button level
void init(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);

// processing function for calling in an encoder interrupt
int8_t tickISR();

// processing function, call to loop
bool tick();

// read
bool readBtn();
```
</details>

<a id="tick"></a>

### Processing and interviewing
All libraries have a common processing function (ticker).`tick`), which receives the current signal from the button and encoder
- This function must be called once in the main cycle of the program (for virtual - with the transfer of value).
- Function returns`true`when the event occurs (for the encoder -`1`or`-1`When you turn,`0`in his absence. Turning to either side is considered as`true`)
- There are separate functions for calling in interruption, they have a suffix.`ISR`see the documentation below

The library processes the signal within this function, the result can be obtained from **poll functions** events. There are two types:
- `[событие]`- return function`true`Once upon a time when an event occurs. Reset after the next call of the processing function (for example, click, turn the encoder). Except for the event`timeout`
- `[состояние]`- return function`true`while this state is active (for example, the button is held)

For ease of perception, the processing function should be placed at the beginning of the cycle, and polls should be done below:
```cpp
void loop() {
  btn.tick();   // survey

  if (btn.click()) Serial.println("click"); // single-click
  if (btn.click()) Serial.println("click"); // same click!
}
```
> Unlike previous versions of the library, processing functions are reset not inside itself, but inside the ticker. Thus, in the example above, when clicking on the button to the port, the message will be displayed twice.`click()`. This allows you to use the survey functions several times during the current iteration of the cycle to create a complex logic of the program.

#### Several processing functions
For obvious reasons, you can not call the processing function more than once per cycle - each next call will reset events from the previous one and the code will not work correctly. You can't do that.
```cpp
// You can't.
void loop() {
  btn.tick();
  if (btn.click()) ...

  // ....

  btn.tick();
  if (btn.hold()) ...
}
```

If you really need to get into a dead cycle and poll the button there, then this is how you can:
```cpp
// so-so
void loop() {
  btn.tick();
  if (btn.click()) ...

  while (true) {
    btn.tick();
    if (btn.hold()) ...
    if (btn.click()) break;
  }
}
```

If the library is used with a connected event handler`attach()`(see below), it can be called`tick()`Wherever and as many times as you like, events will be processed in the processor:
```cpp
// so-so
void cb() {
  switch (btn.action()) {
    // ...
  }
}

void setup() {
  btn.attach(cb);
}

void loop() {
  btn.tick();
  // ...
  btn.tick();
  // ...
  btn.tick();
}
```

#### “Launched” programme
The EncButton library is **asynchronous**: it does not wait until the button is processed, but allows the program to run further. This means that for the library to function properly, the main cycle of the program must run as quickly as possible and not contain delays and other “deaf” cycles within itself. To ensure proper handling of the button, it is not recommended to have more than 50-100 ms in the main delay cycle. A few tips:
- Beginners: Learn the lesson cycle[how to sketch](https://alexgyver.ru/lessons/how-to-sketch/)
  - Write asynchronous code in`loop()`
  - Any synchronous construction on`delay()`You can make asynchronous using`millis()`
  - If the program *each * iteration of the main cycle is performed longer than 50-100 ms - in most cases the program is written incorrectly, except for some special cases.
- Connect the button to the hardware interrupt (see below)
- Avoid executing "heavy" sections of code while the button is being processed, for example by placing them in a condition`if (!button.busy()) { тяжёлый код }`
- If the main cycle cannot be optimized, call the ticker in another “stream” and use the processing function:
  - In a timer interrupt with a period of ~50ms or more often
  - On another core (e.g. ESP32)
  - In another FreeRTOS drag
  - Inside.`yield()`(inside)`delay()`)

#### Separate processing
> It only makes sense in a manual interview! When the processing function is connected, it is enough to call the usual`tick()`between the heavy parts of the program

Also in the downloaded program, you can divide the processing and resetting of events: instead of`tick()`use`tickRaw()`between heavy sections of code and manual reset`clear()`. The order is as follows:
- Interview actions (click, press, turn...)
- Call out.`clear()`
- Call in`tickRaw()`between heavy sections of code

```cpp
void loop() {
  if (btn.click()) ...
  if (btn.press()) ...
  if (btn.step()) ...

  btn.clear();

  // ...
  btn.tickRaw();
  // ...
  btn.tickRaw();
  // ...
  btn.tickRaw();
  // ...
}
```This will allow you to poll the button/encoder in a not very well written program, where the main cycle is littered with heavy code. Inside.`tickRaw()`events accumulate, which are disassembled once in a cycle, and then manually reset.

> In this scenario, the buffering of the encoder in interruption does not work and all events are not processed.`releaseXxx`

#### Processing inside the delay
If it's hard to get rid of`delay()`Within the main cycle of the program, you can place your code inside it on some platforms. Thus, it is possible to obtain even the processing of the encoder in the dile cycle without using interruptions:
```cpp
// delaying
void yield() {
  eb.tickRaw();
}

void loop() {
  if (eb.click()) ...
  if (btn.turn()) ...

  eb.clear();

  // ...
  delay(10);
  // ...
  delay(50);
  // ...
}
```

> In this scenario, the buffering of the encoder in interruption does not work and all events are not processed.`releaseXxx`

#### Processing the button
The library processes the button as follows:
- Clicking with software rattle suppression (holding longer than deb timeout), result - event`press`condition`pressing`and`busy`
- Holding longer than hold timeout - event`hold`condition`holding`
- Holding longer than holding timeout + step timeout - impulse event`step`Activates with a step period while the button is held
- Release of the button, result - event`release`,state relief`pressing`and`holding`
  - Release before retention timeout - event`click`
  - Release after retention - event`releaseHold`
  - Release after impulse retention - event`releaseStep`
  - Events`releaseHold`and`releaseStep`mutually exclusive if the button has been held to`step` - `releaseHold`It won't work.
- Waiting for a new click during the click timeout, state`waiting`
- If there is no new clique - withdrawal of the condition`busy`processing is complete
  - If the button is pressed again - processing a new click
  - Click-counter`getClicks()`drop off`releaseHold`/`releaseStep`That checks for pre-clicks. In the general handler`action()`event`EB_REL_HOLD_C`or`EB_REL_STEP_C`
  - The number of clicks made should be checked by event`hasClicks`It is also possible to poll inside almost all the button events that go up to`releaseXxx`
- If expected`timeout`a timeout event period from`setTimeout`
- The processing of the button in interrupt informs the library of the fact of pressing, all other processing is performed normally in`tick()`

> Difference`click(n)`from`hasClicks(n)`: `click(n)`return`true`In any case, if the number of clicks coincides, even if more clicks are made.`hasClicks(n)`return`true`Only if the exact number of clicks was made and there were no more clicks!

> Better to see once than read a hundred times. Run a demo example and press a button, or try.[online simulation in Wokwi](https://wokwi.com/projects/373591584298469377)

##### Click
![click](/doc/click.gif)  

##### Hold
![hold](/doc/hold.gif)  

##### Step
![step](/doc/step.gif)  

Online simulation available[here](https://wokwi.com/projects/373591584298469377)

#### Encoder processing
- A “fast” turn is a turn made in less than a tuned timeout from the previous turn.
- Interrupted turns become active (cause events) after a call`tick()`
- Access to the encoder counter`counter`It's a public class variable, you can do anything with it.
```cpp
Serial.println(eb.counter); // read
eb.counter += 1234;         // change
eb.counter = 0;             // zero down
```

#### Processing the encoder with a button
- Rotating the encoder with a pressed button removes and blocks all subsequent events and clicks, except for the event.`release`. The state of the pressed button does not change.
- Rotating the encoder also affects the system timeout (function)`timeout()`) - will work within the specified time after turning the encoder
- The Click Counter is available when you press a turn: a few clicks, pressing a button, turning

<a id="preclicks"></a>

### Preliminary clicks
The library counts the number of clicks on the button and some survey functions can be separately processed with *pre-clicks*. For example, 3 clicks, then retention. This greatly expands the power of a single button. There are two options for dealing with such events:
```cpp
  // 1
  if (btn.hold()) {
    if (btn.getClicks() == 2) Serial.println("hold 2 clicks");
  }

  // 2
  if (btn.hold(2)) Serial.println("hold 2 clicks");
```

In the first option, you can get the number of clicks for further manual processing, and in the second - the library will do it itself, if the number of clicks for the action is known in advance.

<a id="btnread"></a>

### Direct reading of the button
In some scenarios, it is necessary to obtain the state of the “here and now” button, for example, to determine whether the button is held immediately after starting the microcontroller (start the program). Function`tick()`You need to call constantly in the cycle, so that the button is processed with the quenching of rattle contacts and other calculations, so the design of the following type ** will not work **:
```cpp
void setup() {
  btn.tick();
  if (btn.press()) Serial.println("Кнопка нажата при старте");
}
```

For such scenarios, the following functions will help, return`true`if the button is pressed:
- `read()`The Button and ButtonT libraries
- `readBtn()`EncButton and EncButton libraries

> The button survey is performed taking into account the previously configured button level (setBtnLevel)! Manually additionally invert logic is not necessary:

```cpp
void setup() {
  // btn.setBtnLevel(LOW) // level can be adjusted

  if (btn.read()) Serial.println("Кнопка нажата при старте");
}
```

<a id="loop"></a>

### Immersion in a cycle
Let's say you need to process the button synchronously and with quenching rattles. For example, if the button is clamped at the start of the microcontroller - get its retention or even impulse retention inside the block.`setup`before the start of the main program. You can take advantage of the condition.`busy`and poll the button from the loop:
```cpp
void setup() {
  Serial.begin(115200);

  btn.tick();
  while (btn.busy()) {
    btn.tick();
    if (btn.hold()) Serial.println("hold");
    if (btn.step()) Serial.println("step");
  }

  Serial.println("program start");
}
```How it works: the first tick polls the button if the button is pressed - the busy state is immediately activated and the system enters the loop`while`. Inside it, we continue to tick and receive events from the button. When the button is released and all events are triggered, the busy flag will drop and the program will automatically leave the cycle. You can rewrite this design into a cycle with a postcondition, more beautifully:
```cpp
do {
  btn.tick();
  if (btn.hold()) Serial.println("hold");
  if (btn.step()) Serial.println("step");
} while (btn.busy());
```

<a id="timeout"></a>

### Timeout
In button-related classes (Button, EncButton) there is a function`timeout()`- she'll give it back once.`true`If, after the end of the action with the button / encoder, the specified`setTimeout`time. This can be used to save parameters after input, for example:
```cpp
void setup() {
  //eb.setTimeout(1500); // silent. 1,000
}
void loop() {
  eb.tick();

  // ...

  if (eb.timeout()) {
    // After interaction with the encoder passed 1000 ms
    // EEPROM.put(0, settings);
  }
}
```

In the current version, timeout processing is not very beautiful for the sake of saving space: the timeout system flag is active all the time.`action`will return the timeout event), reset in the following cases:

- A challenge`timeout()`This method will return once.`true`Subsequent calls will be returned.`false`beforehand
- Reset automatically after calling the handler if it is connected
- On call.`clear(true)`- with a timeout cleaning flag
- On call.`reset()`

If you need to run a timeout survey through several calls - you can use`timeoutState()`But the final challenge must be`timeout()`.

<a id="busy"></a>

### Busy
Function`busy()`return`true`While the button is being processed, i.e. while the system is waiting for actions and timeouts. This can be used to optimize the code, such as avoiding long and heavy parts of the program while processing the button.
```cpp
void loop() {
  eb.tick();

  // ...

  if (!eb.busy()) {
    // potentially long and heavy code
  }
}
```

<a id="actions"></a>

### Receiving the event
Available in all classes **with button **:
- `VirtButton`
- `Button`
- `VirtEncButton`
- `EncButton`

Function`action()`When an event occurs, it returns the event code (other than zero, which in itself is an indication of the event):
- `EB_PRESS`- push the button
- `EB_HOLD`- button held.
- `EB_STEP`- impulse retention
- `EB_RELEASE`- button released.
- `EB_CLICK`- single click
- `EB_CLICKS`- multiple click signal
- `EB_TURN`- turning the encoder
- `EB_REL_HOLD`- button released after retention
- `EB_REL_HOLD_C`- the button is released after holding from the prev. click
- `EB_REL_STEP`- button released after step
- `EB_REL_STEP_C`- button released after step with prev. click
- `EB_TIMEOUT`Passed a timeout after pressing a button or turning the encoder

The resulting event code can be processed through`switch`:
```cpp
switch (eb.action()) {
  case EB_PRESS:
    // ...
    break;
  case EB_HOLD:
    // ...
    break;
  // ...
}
```

There is a similar function`getAction()`, will return the same, but with more readable constants (convenient with autocomplete):

- `EBAction::Press`
- `EBAction::Hold`
- `EBAction::Step`
- `EBAction::Release`
- `EBAction::Click`
- `EBAction::Clicks`
- `EBAction::Turn`
- `EBAction::ReleaseHold`
- `EBAction::ReleaseHoldClicks`
- `EBAction::ReleaseStep`
- `EBAction::ReleaseStepClicks`
- `EBAction::Timeout`

The resulting event code can be processed through`switch`:
```cpp
switch (eb.getAction()) {
  case EBAction::Press:
    // ...
    break;
  case EBAction::Hold:
    // ...
    break;
  // ...
}
```

> Outcome of functions`action()`/`getAction()`reset after the next call`tick()`Available throughout the current iteration of the main cycle

<a id="optimise"></a>

### Optimization
#### Library weight
To minimize the weight of the library (in particular, in RAM), you need to set the timeout constants through define (saving 1 byte per timeout), disable the event handler, counters-buffers and use the T-class (saving 1 byte per pin):
```cpp
#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_NO_COUNTER
#define EB_NO_BUFFER
#define EB_DEB_TIME 50     // timeout of the rattle button (button)
#define EB_CLICK_TIME 500  // Timeout waiting for clicks (button)
#define EB_HOLD_TIME 600   // retention timeout (button)
#define EB_STEP_TIME 200   // impulse retention timeout (button)
#define EB_FAST_TIME 30    // Quick turn timeout (encoder)
#define EB_TOUT_TIME 1000  // Action timeout (button and encoder)
#include <EncButton.h>
EncButtonT<2, 3, 4> eb;
```In this case, the encoder with the button will take only 8 bytes in the SRAM, and just the button - 5.

#### Speed of implementation
To reduce the time to check the system flags of events (insignificantly, but pleasantly), you can put all surveys in the condition`tick()`because`tick()`return`true`Only in the event of an event:
```cpp
void loop() {
  if (eb.tick()) {
    if (eb.turn()) ...;
    if (eb.click()) ...;
  }
}
```

Events are also questioned by the function`action()`It is faster than a manual survey of individual event functions, so the library will work in this format as efficiently as possible:
```cpp
void loop() {
  if (eb.tick()) {
    switch (eb.action()) {
      case EB_PRESS:
        // ...
        break;
      case EB_HOLD:
        // ...
        break;
      // ...
    }
  }
}
```

For questioning **states** buttons`pressing()`, `holding()`, `waiting()`You can put them inside the terms of`busy()`In order not to question the conditions while they are not guaranteed:
```cpp
if (btn.busy()) {
  if (btn.pressing())...
  if (btn.holding())...
  if (btn.waiting())...
}
```

<a id="callback"></a>

### Collbackie.
You can connect an external function-executor of the event, it will be called when any event occurs. This feature works in all classes ** with the button **:
- `VirtButton`
- `Button`
- `VirtEncButton`
- `EncButton`

> Inside the callback, you can get a pointer to the current object (which caused the callback) from the variable.`void* EB_self`

```cpp
EncButton eb(2, 3, 4);

void callback() {
  switch (eb.action()) {
    case EB_PRESS:
      // ...
      break;
    case EB_HOLD:
      // ...
      break;
    // ...
  }

  // Here is EB self pointer to eb
}

void setup() {
  eb.attach(callback);
}

void loop() {
  eb.tick();
}
```

<a id="double"></a>

### Simultaneous press
The library natively supports working with two simultaneously pressed buttons as a third button. This requires:

#### New method (v3)
1. Create a special button`MultiButton`
2. Transfer your buttons to the virtual button (this can be class objects)`Button`and`EncButton`+`T`-versions. The multi-button will interrogate both buttons! **
3. Interview events or listen to the handler

```cpp
Button b0(4);
Button b1(5);
MultiButton b2;  // 1

void loop() {
  b2.tick(b0, b1);  // 2

  // 3
  if (b0.click()) Serial.println("b0 click");
  if (b1.click()) Serial.println("b1 click");
  if (b2.click()) Serial.println("b0+b1 click");
}
```

The library itself will “drop” superfluous events from real buttons if they were clicked together, except for the event.`press`. Thus, a full-fledged third button of the other two with a convenient survey is obtained.

#### The old way
1. Create an additional virtual button
2. Call tickers at individual buttons
3. Call the ticker at the virtual button, transferring individual buttons to it

This method does not work with the handler - it will be called at individual buttons anyway.

```cpp
Button b0(4);
Button b1(5);
VirtButton b2;  // virtual - 1

void loop() {
    // tickers - 2
    b0.tick();
    b1.tick();

    // processing of simultaneous pressing of two buttons - 3
    b2.tick(b0, b1);

    if (b0.click()) Serial.println("b0 click");
    if (b1.click()) Serial.println("b1 click");

    if (b2.click()) Serial.println("b0+b1 click");
    if (b2.step()) Serial.println("b0+b1 step");
}
```

<a id="isr"></a>

### Interruptions
#### encoder
To process the encoder in the downloaded program you need:
- Connect both of its pins to hardware interrupts`CHANGE`
- Install`setEncISR(true)`
- Call a special ticker in the handler for interruption
- The main ticker should also be called in`loop`for correticular work - events are generated in the main ticker:
```cpp
// ATmega328 and EncButton
EncButton eb(2, 3, 4);

/*
// esp8266/esp32
IRAM_ATTR void isr() {
  eb.tickISR();
}
*/

void isr() {
  eb.tickISR();
}
void setup() {
  attachInterrupt(0, isr, CHANGE);
  attachInterrupt(1, isr, CHANGE);
  eb.setEncISR(true);
}
void loop() {
  eb.tick();
}
```

Note: using the work in interrupt allows you to correctly process the position of the encoder and not miss a new turn. The rotating event obtained from the interrupt will become available *after* the call`tick`in the main cycle of the program, which allows not to disturb the sequence of the main cycle:
- Buffering disabled: event`turn`activated only once, regardless of the number of encoder clicks made between two calls`tick`(clicks processed in interruption)
- Buffering included: event`turn`will be called as many times as the actual clicks of the encoder, this allows you to not miss turns at all and not load the system in interruption. ** Buffer size - 5 unprocessed encoder clicks**

Notes:
- Function`setEncISR`It only works in non-virtual classrooms. If it's on, the main ticker.`tick`It simply does not poll the encoder pins, which saves processor time. Processing takes place only in interruption.
- The encoder counter is always relevant and can outpace buffered turns in the program with large delays in the main cycle!
- On different platforms, interrupts can work in different ways (for example, on ESPxx - you need to add attribute functions).`IRAM_ATTR`See the documentation on your platform!
- Processor connected to`attach()`will be called from`tick()`I mean, not out of interruption!

#### Virtual classrooms
In virtual there is a ticker, in which you do not need to transfer the state of the encoder, if it is processed in interruption, this allows you not to interrogate pins in idle. For example:

```cpp
VirtEncoder e;

void isr() {
    e.tickISR(digitalRead(2), digitalRead(3));
}
void setup() {
    attachInterrupt(0, isr, CHANGE);
    attachInterrupt(1, isr, CHANGE);

    e.setEncISR(1);
}
void loop() {
    e.tick();   // We don't transmit pins.
}
```

#### Button
To process the button in interruption you need:
- Connect the interrupt to the **press** button, taking into account its physical connection and level:
  - If the button closes`LOW`- interruption`FALLING`
  - If the button closes`HIGH`- interruption`RISING`
- Call in`pressISR()`interrupter

```cpp
Button b(2);

/*
// esp8266/esp32
IRAM_ATTR void isr() {
  b.pressISR();
}
*/

void isr() {
  b.pressISR();
}
void setup() {
  attachInterrupt(0, isr, FALLING);
}
void loop() {
  b.tick();
}
```

Note: the button is mainly processed`tick()`function`pressISR()`It tells the library that the button has been pressed outside.`tick()`. This allows you not to miss pressing the button while the program was busy with something else.

#### Encoder with a button
You need to connect all three pins to interrupts and act as above - both for the button and for the encoder:

```cpp
void eisr() {
    eb.tickISR();
}
void bisr() {
    eb.pressISR();
}

void setup() {
    // interrupt numbers "for example"
    attachInterrupt(0, eisr, CHANGE);
    attachInterrupt(1, eisr, CHANGE);
    eb.setEncISR(true);

    attachInterrupt(2, bisr, FALLING);
}

void loop() {
    eb.tick();
}
```

<a id="array"></a>

### Array of buttons/encoders
You can create an array only from unconventional classes (without a letter).`T`), because the pin numbers will have to be specified in the runtime later in the program. For example:
```cpp
Button btns[5];
EncButton ebs[3];

void setup() {
  btns[0].init(2);  // pin
  btns[1].init(5);
  btns[2].init(10);
  // ...

  ebs[0].init(11, 12, 13, INPUT);
  ebs[1].init(14, 15, 16);
  // ...
}
void loop() {
  for (int i = 0; i < 5; i++) btns[i].tick();
  for (int i = 0; i < 3; i++) ebs[i].tick();

  if (btns[2].click()) Serial.println("btn2 click");
  // ...
}
```

<a id="custom"></a>

### Custom functions
The library supports setting its functions to read pins and get time without editing library files. To do this, you need to implement the corresponding function in your .cpp or .ino file:
- `bool EB_read(uint8_t pin)`- for its pin reading function
- `void EB_mode(uint8_t pin, uint8_t mode)`- for its analogue pinMode
- `uint32_t EB_uptime()`- for its counterpart millis()

Example:

```cpp
#include <EncButton.h>

bool EB_read(uint8_t pin) {
    return digitalRead(pin);
}

void EB_mode(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
}

uint32_t EB_uptime() {
    return millis();
}
```

<a id="timer"></a>

### Timer survey
Sometimes you need to call.`tick()`Not on every iteration, but on a timer. For example, for a virtual button with a pin expander, when reading a pin expander is a long operation, and calling it often does not make sense. This cannot be done, events will be active during the timer period!
```cpp
void loop() {
  // timer
  static uint32_t tmr;
  if (millis() - tmr >= 50) {
    tmr = millis();
    btn.tick(readSomePin());
  }

  // It will be active for 50 ms!!
  if (btn.click()) foo();
}
```

In this situation, you need to do this: tick the timer, process events there and drop flags at the end:
```cpp
void loop() {
  // timer
  static uint32_t tmr;
  if (millis() - tmr >= 50) {
    tmr = millis();
    // tick
    btn.tick(readSomePin());

    // storytelling
    if (btn.click()) foo();

    // flag-dropping
    btn.clear();
  }
}
```

Or you can connect the handler and call`clear()`at the end of function:
```cpp
void callback() {
  switch (btn.action()) {
    // ...
  }

  // flag-dropping
  btn.clear();
}

void loop() {
  // timer
  static uint32_t tmr;
  if (millis() - tmr >= 50) {
    tmr = millis();
    btn.tick(readSomePin());
  }
}
```

In the case of a timer call, the antidrum will be partially provided by the timer itself and it can be turned off in the library (set period 0).

For the correct operation of timeouts, states and the click counter, a different approach is needed: buffer the states read by the timer and transfer them to the tick in the main cycle. Like this:
```cpp
bool readbuf = 0;  // pin-buffer

void loop() {
  // timer
  static uint32_t tmr;
  if (millis() - tmr >= 50) {
    tmr = millis();
    readbuf = readSomePin();  // buffering
  }

  // buffer-tick
  btn.tick(readbuf);

  if (btn.click()) foo();
}
```

### Skipping events
EncButton allows the button to work in tandem with the encoder to correctly track *pushed turns* - when you press the turn, the events from the button will be skipped, i.e. retention and click will not be processed. Let’s assume several buttons: they can perform actions both by themselves and in pair with an encoder (the button is clamped and the encoder rotates, the value selected by the button changes in the program). So that when holding the button does not generate events (retention, step, clicks...), you can turn on the event skip. It will continue until the button is released:

```cpp
if (btn.pressing() && enc.turn()) {
  btn.skipEvents();  // We got a turn. Missing events
  // turn-on
}

if (btn.click()) {
  // just click
}
```

<a id="examples-mini"></a>

### Mini examples, scripts.
```cpp
// change the values of variables

// turning
if (enc.turn()) {
  // Change in step 5.
  var += 5 * enc.dir();

  // Change in steps 1 in normal turn, 10 in rapid turn
  var += enc.fast() ? 10 : 1;

  // Change in steps 1 in normal rotation, 10 when pressed
  var += enc.pressing() ? 10 : 1;

  // We change one variable when we turn, the other when we turn.
  if (enc.pressing()) var0++;
  else var1++;

  // If the button is pressed, preliminary clicks are available.
  // Select a variable to change by prior clicks
  if (enc.pressing()) {
    switch (enc.getClicks()) {
      case 1: var0 += enc.dir();
        break;
      case 2: var1 += enc.dir();
        break;
      case 3: var2 += enc.dir();
        break;
    }
  }
}

// impulse retention at each step incremental variable
if (btn.step()) var++;

// Change of direction of variable after release from step
if (btn.step()) var += dir;
if (btn.releaseStep()) dir = -dir;

// Change the selected variable by step
if (btn.step(1)) var1++;  // click-hold
if (btn.step(2)) var2++;  // click-click-hold
if (btn.step(3)) var3++;  // click-click-click-hold

// If you hold a step for more than 2 seconds, increment +5 until less - +1
if (btn.step()) {
  if (btn.stepFor(2000)) var += 5;
  else var += 1;
}

// switching on
if (btn.hasClicks()) mode = btn.getClicks();

// Enabling a mode of several clicks and retention
if (btn.hold(1)) mode = 1;  // click-hold
if (btn.hold(2)) mode = 2;  // click-click-hold
if (btn.hold(3)) mode = 3;  // click-click-click-hold

// so
if (btn.hold()) mode = btn.getClicks();

// The button is released, see how long it was held.
if (btn.release()) {
  // 1-second
  if (btn.pressFor() > 1000 && btn.pressFor() <= 2000) mode = 1;
  // 2 to 3 seconds
  else if (btn.pressFor() > 2000 && btn.pressFor() <= 3000) mode = 2;
}
```

<a id="migrate"></a>

## Guide to migration from v2 to v3
### Initialization
```cpp
// Virtual
VirtEncButton eb; // button-encoder
VirtButton b;     // button
VirtEncoder e;    // encoder

// REAL
// button-encoder
EncButton eb(enc0, enc1, btn);                    // encoder pins and buttons
EncButton eb(enc0, enc1, btn, modeEnc);           // + Encoder pin mode (silent. INPUT)
EncButton eb(enc0, enc1, btn, modeEnc, modeBtn);  // + button pin mode (silent. INPUT PULLUP)
EncButton eb(enc0, enc1, btn, modeEnc, modeBtn, btnLevel);  // + button level (silent. LOW)
// formulaic
EncButton<enc0, enc1, btn> eb;                    // encoder pins and buttons
EncButton<enc0, enc1, btn> eb(modeEnc);           // + Encoder pin mode (silent. INPUT)
EncButton<enc0, enc1, btn> eb(modeEnc, modeBtn);  // + button pin mode (silent. INPUT PULLUP)
EncButton<enc0, enc1, btn> eb(modeEnc, modeBtn, btnLevel);  // + button level (silent. LOW)

// button
Button b(pin);                  // pin
Button b(pin, mode);            // + button pin mode (silent. INPUT PULLUP)
Button b(pin, mode, btnLevel);  // + button level (silent. LOW)
// formulaic
ButtonT<pin> b;                 // pin
ButtonT<pin> b(mode);           // + button pin mode (silent. INPUT PULLUP)
ButtonT<pin> b(mode, btnLevel); // + button level (silent. LOW)

// encoder
Encoder e(enc0, enc1);          // encoder pins
Encoder e(enc0, enc1, mode);    // + Encoder pin mode (silent. INPUT)
// formulaic
EncoderT<enc0, enc1> e;         // encoder pins
EncoderT<enc0, enc1> e(mode);   // + Encoder pin mode (silent. INPUT)
```

### Functions
| v2          | v3           |
| ----------- | ------------ |
| `held()`    | `hold()`     |
| `hold()`    | `holding()`  |
| `state()`   | `pressing()` |
| `setPins()` | `init()`     |

- Changed order of pins (see doc above)
- `clearFlags()`substitute`clear()`(drop event flags) and`reset()`(Reset system processing flags, complete processing)

### Logic of work
In v3, event polling functions (click, turn...) are not reset immediately after their call - they are reset on the next call.`tick()`, thus retain their importance in all subsequent calls on the current iteration of the main cycle of the program. **Therefore`tick()`You only need to call 1 time per cycle, otherwise there will be missed actions!** Read more about this above.

<a id="example"></a>
## Examples
For more examples see **examples**!
<details>
<summary>Full demo of EncButton</summary >>

```cpp
// #define EB NO FOR // disable support for pressFor/holdFor/stepFor and step counter (saves 2 bytes of RAM)
// #define EB NO CALLBACK // disable event handler attach (saves 2 bytes of RAM)
// #define EB NO COUNTER // disable the encoder counter (saves 4 bytes of RAM)
// #define EB NO BUFFER // disable encoder buffering (saves 1 byte of RAM)

// #define EB DEB TIME 50 // timeout quenching the rattle button (button)
// #define EB CLICK TIME 500 // timeout waiting for clicks
// #define EB HOLD TIME 600 // retention timeout (button)
// #define EB STEP TIME 200 // pulse retention timeout (button)
// #define EB FAST TIME 30 // Quick Turn timeout (encoder)
// #define EB TOUT TIME 1000 // action timeout (button and encoder)

#include <EncButton.h>
EncButton eb(2, 3, 4);
//EncButton eb(2, 3, 4, INPUT); // + encoder pin mode
//EncButton eb(2, 3, 4, INPUT, INPUT PULLUP); // + button pin mode
//EncButton eb(2, 3, 4, INPUT, INPUT PULLUP, LOW); // + button level

void setup() {
    Serial.begin(115200);

    // default
    eb.setBtnLevel(LOW);
    eb.setClickTimeout(500);
    eb.setDebTimeout(50);
    eb.setHoldTimeout(600);
    eb.setStepTimeout(200);

    eb.setEncReverse(0);
    eb.setEncType(EB_STEP4_LOW);
    eb.setFastTimeout(30);

    // drop off the encoder
    eb.counter = 0;
}

void loop() {
    eb.tick();

    // cornering
    if (eb.turn()) {
        Serial.print("turn: dir ");
        Serial.print(eb.dir());
        Serial.print(", fast ");
        Serial.print(eb.fast());
        Serial.print(", hold ");
        Serial.print(eb.pressing());
        Serial.print(", counter ");
        Serial.print(eb.counter);
        Serial.print(", clicks ");
        Serial.println(eb.getClicks());
    }

    // cornering
    if (eb.left()) Serial.println("left");
    if (eb.right()) Serial.println("right");
    if (eb.leftH()) Serial.println("leftH");
    if (eb.rightH()) Serial.println("rightH");

    // button
    if (eb.press()) Serial.println("press");
    if (eb.click()) Serial.println("click");

    if (eb.release()) {
      Serial.println("release");

      Serial.print("clicks: ");
      Serial.print(eb.getClicks());
      Serial.print(", steps: ");
      Serial.print(eb.getSteps());
      Serial.print(", press for: ");
      Serial.print(eb.pressFor());
      Serial.print(", hold for: ");
      Serial.print(eb.holdFor());
      Serial.print(", step for: ");
      Serial.println(eb.stepFor());
    }

    // condition
    // Serial.println(eb.pressing());
    // Serial.println(eb.holding());
    // Serial.println(eb.busy());
    // Serial.println(eb.waiting());

    // time-out
    if (eb.timeout()) Serial.println("timeout!");

    // retention
    if (eb.hold()) Serial.println("hold");
    if (eb.hold(3)) Serial.println("hold 3");

    // impulse-hold
    if (eb.step()) Serial.println("step");
    if (eb.step(3)) Serial.println("step 3");

    // released after impulse retention
    if (eb.releaseStep()) Serial.println("release step");
    if (eb.releaseStep(3)) Serial.println("release step 3");

    // released
    if (eb.releaseHold()) Serial.println("release hold");
    if (eb.releaseHold(2)) Serial.println("release hold 2");

    // click-check
    if (eb.hasClicks(3)) Serial.println("has 3 clicks");

    // click out
    if (eb.hasClicks()) {
        Serial.print("has clicks: ");
        Serial.println(eb.getClicks());
    }
}
```</details >>
<details >>
<summary>Connect the processor</summary >>

```cpp
#include <EncButton.h>
EncButton eb(2, 3, 4);

void callback() {
    Serial.print("callback: ");
    switch (eb.action()) {
        case EB_PRESS:
            Serial.println("press");
            break;
        case EB_HOLD:
            Serial.println("hold");
            break;
        case EB_STEP:
            Serial.println("step");
            break;
        case EB_RELEASE:
            Serial.println("release");
            break;
        case EB_CLICK:
            Serial.println("click");
            break;
        case EB_CLICKS:
            Serial.print("clicks ");
            Serial.println(eb.getClicks());
            break;
        case EB_TURN:
            Serial.print("turn ");
            Serial.print(eb.dir());
            Serial.print(" ");
            Serial.print(eb.fast());
            Serial.print(" ");
            Serial.println(eb.pressing());
            break;
        case EB_REL_HOLD:
            Serial.println("release hold");
            break;
        case EB_REL_HOLD_C:
            Serial.print("release hold clicks ");
            Serial.println(eb.getClicks());
            break;
        case EB_REL_STEP:
            Serial.println("release step");
            break;
        case EB_REL_STEP_C:
            Serial.print("release step clicks ");
            Serial.println(eb.getClicks());
            break;
    }
}

void setup() {
    Serial.begin(115200);
    eb.attach(callback);
}

void loop() {
    eb.tick();
}
```</details >>
<details >>
<summary>All button types</summary >>

```cpp
#include <EncButton.h>

Button btn(4);
ButtonT<5> btnt;
VirtButton btnv;

void setup() {
    Serial.begin(115200);
}

void loop() {
    // Button
    btn.tick();
    if (btn.click()) Serial.println("btn click");

    // ButtonT
    btnt.tick();
    if (btnt.click()) Serial.println("btnt click");

    // VirtButton
    btnv.tick(!digitalRead(4));  // render
    if (btnv.click()) Serial.println("btnv click");
}
```</details >>
<details >>
<summary>All types of encoders</summary >>

```cpp
#include <EncButton.h>

Encoder enc(2, 3);
EncoderT<5, 6> enct;
VirtEncoder encv;

void setup() {
    Serial.begin(115200);
}

void loop() {
    // The survey is the same for all, 3 ways:

    // 1
    // Tick will return 1 or -1, so this is a step.
    if (enc.tick()) Serial.println(enc.counter);

    // 2
    // can be interviewed through turn()
    enct.tick();
    if (enct.turn()) Serial.println(enct.dir());

    // 3
    // You can not use the survey functions, but receive a direct referral
    int8_t v = encv.tick(digitalRead(2), digitalRead(3));
    if (v) Serial.println(v);  // output 1 or -1
}
```
</details>

<a id="versions"></a>
## Versions
<details>
<summary>Old</summary> >>

- v1.1 - Pullap by separate method
- v1.2 - you can transfer the INPUT PULLUP / INPUT parameter to the designer
- v1.3 - virtual clamping of the encoder button is made in a separate function + minor improvements
- v1.4 - Processing the push and release of the button
- v1.5 - Virtual mode added
- v1.6 - Optimization of work in interruption
- v1.6.1 - Default lift INPUT PULLUP
- v1.7 - big memory optimization, redesigned by FastIO
- v1.8 - individual timeout setting button retention (was common to all)
- v1.8.1 - FastIO removed
- v1.9 - Added separate practice of pressed turn and direction request
- v1.10 - improved release processing, eased callback weight and fixed bugs
- v1.11 - more optimization + button level adjustment
- v1.11.1 - Digispark compatibility
- v1.12 - added a more accurate EB BETTER ENC encoder algorithm
- v1.13 - Experimental EncButton2 added
- v1.14 - ReleaseStep() is added. The release of the button is included in the debunk
- v1.15 - Added setPins() for EncButton2
- v1.16 - added EB HALFSTEP ENC mode for semi-half encoders
- v1.17 - Step added with pre-clicks
- v1.18 - Do not count clicks after activation of step. hold() and held() can also receive preliminary clicks. Redesigned and improved Debones
- v1.18.1 Fixed an error in releaseStep() (did not return the result)
- v1.18.2 - fix compiler warnings
- v1.19 - Speed optimization, reduced weight in sram
- v1.19.1 - slightly increased performance
- v1.19.2 - A little more performance, thanks XRay3D
- v1.19.3 - made a high level of the default button in virtual mode
- v1.19.4 - EncButton2 fix
- v1.20 - Fixed critical error in EncButton2
- v1.21 - EB HALFSTEP ENC now works in normal mode
- v1.22 - improved EB HALFSTEP ENC for normal mode
- v1.23 - getDir() replaced by dir()
- v2.0 
    - EB BETTER ENC algorithm optimized and installed by default, EB BETTER ENC defiin abolished
    - Added setEncType() to configure the encoder type from the program, EB HALFSTEP ENC defiin abolished
    - Added setEncReverse() to change the direction of the encoder from the program
    - Added setStepTimeout() to set pulse retention period, EB STEP defiant abolished
    - Small improvements and optimization
</details>

- v3.0
  - The library is rewritten from scratch, incompatible with previous versions!
    - Completely different initialization of the object
    - Renamed: hold()->holding(), held()->hold()
  - Flash memory optimization: the library weighs less, in some scenarios – a few kilobytes
  - Optimization of code execution speed, including interruption
  - A few bytes less RAM, several levels of optimization to choose from
  - Simpler, more understandable and convenient use
  - More readable source code
  - Classification for use in different scenarios
  - New features, capabilities and handlers for button and encoder
  - Buffering of the encoder in interruption
  - Native processing of two simultaneously pressed buttons as a third button
  - Support for 4 types of encoders
  - Documentation rewritten
  - EncButton now replaces GyverLibs/VirtualButton (archived)
- v3.1
  - Extended button initialization
  - Removed holdEncButton() and toggleEncButton()
  - Added turnH()
  - Encoder interrupts optimized, setEncISR() added
  - Buffering direction and fast turn
  - Strongly optimized action() speed (common processor)
  - External event processing function added
  - Added button processing in interrupt - pressISR()
- v3.2
  - Added tickRaw() and clear() functions for all classes. Allows separate processing (see dock)
  - Improved button processing using interrupts
- v3.3
  - Added functions for obtaining retention time pressFor(), holdFor(), stepFor() (disabled)
  - Added step counter getSteps() (disabled)
- v3.4
  - Access to the Click Counter during a Pushed Turn
  - Added detach() function
- v3.5 
  - Added dependence GyverIO (accelerated pin survey)
  - Added the ability to set your uptime and pin reading functions
- v3.5.2 
  - Optimization
  - Simplified replacement of custom functions
  - Fixed compilation error when using the library in multiple .cpp files
- v3.5.3
  - Added number of clicks to press/release/click/pressing survey
- v3.5.5 - Callback based on std::function for ESP
- v3.5.8 - ReleaseHoldStep() method added
- v3.5.11 - added skipEvents() to ignore button events in complex use cases
- v3.6.0 
  - MultiButton class added to correctly poll multiple buttons with handler call
  - Added connection of the handler with the transfer of the pointer to the object

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
