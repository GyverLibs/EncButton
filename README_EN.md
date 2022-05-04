This is an automatic translation, may be incorrect in some places. See sources and examples!

# EncButton
Lightweight and functional library for encoder, encoder with button or just buttons with Arduino
- Fastest pin reading for AVR (ATmega328/ATmega168, ATtiny85/ATtiny13)
- Fast optimized algorithms for polling actions from a button / encoder
- Light weight in Flash and SRAM memory
- Encoder: normal turn, pressed turn, fast turn, counter access
    - Support for two types of incremental encoders (full step and half step)
    - High-precision position determination algorithm, works even with low-quality encoders
- Button: anti-bounce, press, release, click, multi-click, click counter, hold, pulse hold
- Manual action polling or handler function mode
- Virtual mode (button, encoder, encoder with button)
- Optimized for interrupt operation

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Introduction](#base)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **EncButton** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download Library](https://github.com/GyverLibs/EncButton/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="base"></a>
## Introduction
### Optimization
The library is universal:
- Can work with "physical" button, encoder or encoder with button
- Can work with "virtual" button, encoder or encoder with button
- Can work in two modes: manual polling of actions or mode with handler functions

The library is heavily optimized: the main settings are set as `<template parameters>`, so, depending on the chosen hardware and operating mode, all unnecessary code is hard cut by the compiler and the program takes up a minimum amount of memory.

### Library version
- **EncButton** - stores pin numbers as template parameters, provides maximum speed and light weight. It can be difficult to dynamically create an object, and you can't create an array of objects.
- **EncButton2** - stores pin numbers as class members. You can create dynamically, make arrays, and set pins later in the program. Differs from **EncButton** only in initialization!

### Iron
To work according to the "encoder with a button" scenario, I recommend these ([link] (https://ali.ski/CYir4), [link] (https://ali.ski/49q5hy)) round Chinese modules with soldered anti-bounce circuits :
![scheme](/doc/encAli.png)

You can independently tie the encoder according to the following scheme (RC filters on the encoder channels + tightening all pins to VCC):
![scheme](/doc/enc.png)

### Encoder type
There are two types of incrementalncoders, I called them full step and half step. Full step gives 4 signals per click, half step - 2.
If the library produces one "turn" on two encoder clicks, change the type to `EB_HALFSTEP`.

### Performance
Idle execution time of the tick() function with a real device (button / encoder connected to MK pins) on ATmega328, EncButton library:
| Mode | Time, ms |
| ----- | ---------- |
| Encoder + button | 3.8 |
| Encoder | 2.4 |
| Button | 1.9 |

*For comparison, a standard digitalRead() on an AVR takes 3.5 us*

### Comparison with analogues
- EncButton in button mode is 6µs faster, ~450 bytes Flash and 12 bytes SRAM lighter than my old [GyverButton](https://github.com/GyverLibs/GyverButton) library, while having more features
- EncButton in button encoder mode is 6µs faster, ~400 bytes Flash and 18 bytes SRAM lighter than my old library [GyverEncoder](https://github.com/GyverLibs/GyverEncoder) while having more features

<a id="init"></a>
## Initialization
The library can work in two modes (specified during initialization):
- `EB_TICK` - polling button/encoder actions manually in the main program loop. The best option
- `EB_CALLBACK` - connecting your own handler functions to the desired actions. Adds 28B SRAM per object
More details below in the chapter "Usage"

<details>
<summary>Initialize EncButton</summary>

```cpp
// ============ PULL-UP ============
// During initialization, you can specify the mode of operation of ALL pins
// Default INPUT_PULLUP. If an external pull-up is used, it is better to transfer to INPUT
EncButton<...> enc(pinmode);
// pinmode: INPUT or INPUT_PULLUP

// ========= PHYSICAL HARDWARE ========
// The type of piece of iron is set by the number of specified pins:
EncButton<MODE, A, B, KEY> enc; // encoder with button
EncButton<MODE, A, B> enc; // just an encoder
EncButton<MODE, KEY> btn; // just a button
// A, B, KEY: pin numbers
// MODE: EB_TICK or EB_CALLBACK - manual operation mode or with handlers

// ========= VIRTUAL MODE =========
EncButton<MODE, VIRT_BTN> enc; // virtual button
EncButton<MODE, VIRT_ENCBTN> enc; // virtual enc with button
EncButton<MODE, VIRT_ENC> enc; // virtual enc
// MODE: EB_TICK or EB_CALLBACK - manual operation mode or with handlers
```
</details>
<details>
<summary>Initializing EncButton2</summary>

```cpp
// ================ TICK ===============
EncButton2<EB_ENCBTN> enc(pinmode, A, B, KEY); // encoder with button
EncButton2<EB_ENC> enc(pinmode, A, B); // just an encoder
EncButton2<EB_BTN> enc(pinmode, KEY); // just a button
// pinmode - INPUT/INPUT_PULLUP pin mode
// A, B, KEY: pin numbers

// ============== CALLBACK ==============
EncButton2<EB_ENCBTN, EB_CALLBACK> enc(pinmode, A, B, KEY); // encoder with button
EncButton2<EB_ENC, EB_CALLBACK> enc(pinmode, A, B); // just an encoder
EncButton2<EB_BTN, EB_CALLBACK> enc(pinmode, KEY); // just a button
// pinmode - INPUT/INPUT_PULLUP pin mode
// A, B, KEY: pin numbers

// ============== VIRT TICK ============
EncButton2<VIRT_ENCBTN> enc; // encoder with button
EncButton2<VIRT_ENC> enc; // just an encoder
EncButton2<VIRT_BTN> enc; // just a button

// ============ VIRT CALLBACK ==========
EncButton2<VIRT_ENCBTN, EB_CALLBACK> enc; // encoder with button
EncButton2<VIRT_ENC, EB_CALLBACK> enc; // just an encoder
EncButton2<VIRT_BTN, EB_CALLBACK> enc; // just a button
```
</details>
<details>
<summary>Array of EncButton2 instances</summary>

```cpp
EncButton2<EB_ENCBTN> enc[number];
EncButton2<EB_ENC> enc[number];
EncButton2<EB_BTN> enc[number];

EncButton2<EB_ENCBTN, EB_CALLBACK> enc[number];
EncButton2<EB_ENC, EB_CALLBACK> enc[number];
EncButton2<EB_BTN, EB_CALLBACK> enc[number];
// etc

// You can set pins via setPins()
setPins(uint8_t mode, uint8_t P1, uint8_t P2, uint8_t P3);
// mode - INPUT/INPUT_PULLUP (for all pins)
// specify only the pins needed for the selected mode:
// EB_ENCBTN - A, B, KEY
// EB_ENC - A, B
// EB_BTN - KEY
// see example EucButton2_array
```
</details>

## Documentation
<details>
<summary>FULL CLASS DESCRIPTION</summary>

```cpp
// =============== SETTINGS ==============
void setButtonLevel(bool level); // button level: LOW - button connects GND (default), HIGH - button connects VCC
void setHoldTimeout(int tout); // set the button hold time, ms (up to 8000)
void setStepTimeout(int tout); // set pulse period step, ms (up to 4000)

void holdEncButton(bool state); // virtually hold the encoder button (to trigger pressed turns)
void setEncReverse(bool rev); // true - invert encoder direction (default false)
void setEncType(bool type); // encoder type: EB_FULLSTEP (0) by default, EB_HALFSTEP (1) if the encoder makes one turn in two clicks

// ================= TICK ================
// ticker, call in the main loop of the program loop(). This is where the pins are polled and all calculations are made.
uint8_t tick();

// virtual ticker can receive virtual signal in VIRT_xxx mode:
uint8_t tick(uint8_t s1 = 0, uint8_t s2 = 0, uint8_t key = 0);
// (button signal)
// (encoder A signal, encoder B signal)
// (encoder A signal, encoder B signal, button signal)

// Ticker to call in interrupt. Requires regular tick() in loop() (see tickISR and callbackISR examples)
uint8_t tickISR();

// all tickers return a non-zero value if an event has occurred (see list of statuses below)

// checks and calls connected functions for callback mode
// Built into tick(), but taken out as a separate function for non-standard work scenarios
void checkCallback();

// =============== STATE ================
void resetState(); // reset status
uint8_t getState(); // get button/encoder status
// 0 - idle
// 1-left
// 2-right
// 3 - leftH
// 4-rightH
// 5-click
// 6 - held
// 7-step
// 8-press

// =============== ENCODER ===============
bool turn(); // rotate one click to either side
bool turnH(); // turn one click in any direction while holding the button
boolfast(); // quick turn by one click in any direction
boolright(); // rotate one click to the right
bool left(); // rotate one click to the left
bool rightH(); // rotate one click to the right while holding the button
bool leftH(); // rotate one click to the left while holding the button
int8_t dir(); // last turn direction, 1 or -1
int16_t counter; // access to encoder counter

// ================ BUTTON ================
bool busy(); // will return true if we still need to call tick to poll for timeouts
bool state(); // current button state (true pressed, false not pressed)
boolpress(); // the button was pressed
bool release(); // the button has been released
boolclick(); // click (pressed and released)
bool held(); // the button has been held down
bool hold(); // button is held down
boolstep(); // pulse hold mode
bool step(uint8_t clicks); // impulse hold mode with pre-call
bool releaseStep(); // released after step mode
bool releaseStep(uint8_t clicks); // released after step mode with pre-click
uint8_t hasClicks(); // will return the number of clicks, if any
bool hasClicks(uint8_t num); // check for the specified number of clicks
uint8_t clicks; // access to the click counter

// =============== CALLBACK ===============
void attach(TYPE, void (*handler)()); // connect handler
void detach(TYPE); // disable handler
void attachClicks(uint8_t amount, handler); // connect a handler for the number of clicks (there can be only onein), up to 16 clicks!
void detachClicks(); // disable the handler for the number of clicks

// TYPE can be:
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

// =========== Optional EncButton2 ===========
// set up pins
void setPins(uint8_t mode, uint8_t P1, uint8_t P2, uint8_t P3);
// mode - INPUT/INPUT_PULLUP (for all pins)
// pins - specify only those needed for the selected mode:
// EB_ENCBTN - (A, B, KEY)
// EB_ENC - (A, B)
// EB_BTN - (KEY)


// =============== DEFINE SETTINGS ===============
// define BEFORE INCLUDING THE LIBRARY, default values ​​are shown
#define EB_FAST 30 // encoder fast turn timeout, ms
#define EB_DEB 50 // button debounce, ms
#define EB_CLICK 400 // button click timeout, ms
```
</details>

<a id="usage"></a>
## Features and use cases
### Poll pins
Polling encoder/button pins, calculating timeouts and calling callbacks takes place inside the `tick()` function. This function must be called **once** in the main `loop()` program loop.
```cpp
void loop() {
  btn.tick();
}
```

### EB_TICK mode
In this mode, you need to manually poll for actions from the encoder / button using `if ()`:
- According to the logic of work, all polls should be placed after the call to `tick()`
- Almost all polling functions have a "shot once" mechanism, that is, they return `true` and automatically reset to `false` until the next event occurs.

Example:
```cpp
void loop() {
  btn.tick();
  if (btn.click()) Serial.println("Click");
}
```

#### Button
- `press()` - the button was pressed. *[will return true once]*
- `release()` - the button has been released. *[will return true once]*
- `click()` - the button was clicked, i.e. pressed and released before the hold timeout. *[will return true once]*
- `held()` - the button was held longer than the hold timeout. *[will return true once]*
- `held(clicks)` is the same, but the function takes the number of clicks made before the hold. Note: held() without an argument will intercept the call! See example *preClicks*. *[will return true once]*
- `hold()` - the button was held longer than the hold timeout. *[returns true while held]*
- `hold(clicks)` - the same, but the function takes the number of clicks made before the hold. Note: hold() without an argument will intercept the call! See example *preClicks*. *[returns true while held]*
- `step()` - "impulse hold" mode: after holding the button longer than the timeout, this function *[returns true with EB_STEP period]*. It is convenient to use for step-by-step change of variables: `if (btn.step()) val++;`.
- `step(clicks)` - the same, but the function takes the number of clicks made before the hold. Note: step() without an argument will intercept the call! See *StepMode* and *preClicks* example.
- `releaseStep()` - the button was released after a pulse hold. Can be used to change the sign of a variable's increment. See example *StepMode*. *[will return true once]*
- `releaseStep(clicks)` - the same, but the function takes the number of clicks made before the hold. Note: releaseStep() without an argument will intercept the call! See *StepMode* and *preClicks* example. *[will return true once]*
- `hasClicks(clicks)` - the specified number of clicks were made with a period less than *EB_CLICK*. *[will return true once]*
- `state()` - returns the current state of the button (signal from the pin, without anti-bounce): `true` - pressed, `false` - not pressed.
- `busy()` - will return `true` if you still need to call tick to poll for timeouts
- `hasClicks()` - returns the number of clicks made with a period less than *EB_CLICK*. Otherwise, it will return 0.
- `uint8_t clicks` - public variable, stores the number of clicks made with a period less than *EB_CLICK*. Resets to 0 after a new click.
![diagram](/doc/diagram.png)

#### Encoder
- `turn()` - turn by one click in any directionCranberries *[will return true once]*
- `turnH()` - turn by one click in any direction while holding the button. *[will return true once]*
- `fast()` - a fast turn (with a period of less than *EB_FAST* ms) was made by one click in any direction. *[returns true while the encoder is spinning fast]*
- `right()` - turn one click to the right. *[will return true once]*
- `left()` - turn one click to the left. *[will return true once]*
- `rightH()` - rotate one click to the right while holding the button. *[will return true once]*
- `leftH()` - turn one click to the left while holding the button. *[will return true once]*
- `dir()` - direction of the last turn, 1 or -1.
- `int16_t counter` - public variable, stores encoder counter.

### Callback mode
In this mode, you can connect your own handler function to any button / encoder action. Functions will be automatically called when an event occurs.
To work, you also need to call `tick()` in the main program loop, where reading and processing of signals takes place. The connected handler functions are also called from here. See example *callbackMode*

```cpp
void attach(type, func); // connect handler
void detach(type); // disable handler
void attachClicks(uint8_t amount, func); // connect a handler for the number of clicks (there can be only one), up to 16 clicks!
void detachClicks(); // disable the handler for the number of clicks
```

Where `type` is the event type:
- `TURN_HANDLER` - turn
- `TURN_H_HANDLER` - pressed turn
- `RIGHT_HANDLER` - turn right
- `LEFT_HANDLER` - turn left
- `RIGHT_H_HANDLER` - pressed right turn
- `LEFT_H_HANDLER` - pressed left turn
- `PRESS_HANDLER` - pressing
- `RELEASE_HANDLER` - release
- `CLICK_HANDLER` - click
- `HOLDED_HANDLER` - hold (single operation)
- `HOLD_HANDLER` - hold (permanent operation)
- `STEP_HANDLER` - impulse hold
- `CLICKS_HANDLER` - several clicks

Example:
```cpp
void setup() {
  btn.attach(CLICK_HANDLER, myClick);
}

void myClick() {
  Serial.println("Click");
}

void loop() {
  btn.tick();
}
```

### Virtual mode
The virtual mode allows you to get all the features of the EncButton library in situations where the button is not connected directly to the microcontroller, or another library is used to poll it:
- Analog keyboard (for example, via the [AnalogKey] library (https://github.com/GyverLibs/AnalogKey)). See example *virtual_AnalogKey*
- Matrix keyboard (for example, via the [SimpleKeypad] library (https://github.com/maximebohrer/SimpleKeypad)). See example *virtual_SimpleKeypad* and *virtual_SimpleKeypad_array*
- Buttons or encoders connected via pin expanders or shift registers

Thus, you can get multiple clicks from the matrix keyboard, holding the buttons of the matrix keyboard, impulse hold and other EncButton chips.

To work, you need to pass the current states of the "pins" of the button / encoder to `tick()`: `tick(s1, s2, s3)` in the following order
- Button - (button signal)
- Encoder - (encoder A signal, encoder B signal)
- Push button encoder - (encoder A signal, encoder B signal, button signal)

### Optimization
`tick()` returns the current encoder/button status:
- 0 - no action was taken
- 1 - left + turn
- 2 - right + turn
- 3 - leftH + turnH
-4-rightH + turnH
-5-click
- 6 - held
-7 step
-8-press

This allows you to slightly optimize the program: to further poll the actions of the button / encoder only upon the fact of their completion:
you can put the entire poll in an `if (enc.tick()) {}` block. At the end it is recommended to call `resetState()` to collect unsolicited flags,
so that `tick()` stops "beeping" the action. See the *optimization* example for details.

### Interrupts
To improve the quality of encoder / button processing in the loaded program (so as not to miss a turn or click), it is recommended
duplicate the poll in the interrupt by *CHANGE*. For encoder -Cranberries on both pins. Inside the interrupt handler, we call the special ticker `tickISR()`,
and in the main program loop we leave the usual `tick()` - it is needed in order to correctly calculate all timeouts.
In `EB_CALLBACK` mode, all connected functions are called from `tick()`, that is, not from the interrupt handler, but from the main program loop!

There is also a `busy()` function - it returns `true` until pressing timeout processing is complete, i.e. it is still necessary to call `tick()` in `loop()`.

<a id="example"></a>
## Examples
See **examples** for other examples!

### Full demo EB_TICK
```cpp
// Optional define settings (shown by default)
//#define EB_FAST 30 // fast turn timeout, ms
//#define EB_DEB 50 // button debounce, ms
//#define EB_CLICK 400 // click timeout, ms

#include <EncButton.h>
EncButton<EB_TICK, 2, 3, 4> enc; // encoder with button <A, B, KEY>
//EncButton<EB_TICK, 2, 3> enc; // just encoder <A, B>
//EncButton<EB_TICK, 4> enc; // just a button <KEY>

void setup() {
  Serial.begin(9600);
  //enc.setButtonLevel(HIGH); // button level: LOW - button connects GND (default), HIGH - button connects VCC
  //enc.setHoldTimeout(1000); // set the button hold time, ms (up to 8000)
  //enc.setStepTimeout(500); // set pulse period step, ms (up to 4000)

  //enc.holdEncButton(true); // virtually hold the encoder button (to trigger pressed turns)
  //enc.setEncReverse(true); // true - invert encoder direction (default false)
  //enc.setEncType(EB_HALFSTEP); // encoder type: EB_FULLSTEP (0) by default, EB_HALFSTEP (1) if the encoder makes one turn in two clicks
}

void loop() {
  enc.tick(); // polling happens here

  // =============== ENCODER ===============
  // regular rotation
  if (enc. turn()) {
    Serial.println("turn");

    // Can I have some more:
    serial.println(enc.counter); // display counter
    Serial.println(enc.fast()); // check fast turn
    Serial.println(enc.dir()); // display direction of rotation
  }

  // "pressed rotation"
  if (enc.turnH()) Serial.println("hold + turn");

  if (enc.left()) Serial.println("left"); // left turn
  if (enc.right()) Serial.println("right"); // right turn
  if (enc.leftH()) Serial.println("leftH"); // pressed turn left
  if (enc.rightH()) Serial.println("rightH"); // pressed turn right

  // =============== BUTTON ===============
  if (enc.press()) Serial.println("press");
  if (enc.click()) Serial.println("click");
  if (enc.release()) Serial.println("release");

  if (enc.held()) Serial.println("held"); // will return true once on hold
  //if (enc.hold()) Serial.println("hold"); // will always return true after holding
  if (enc.step()) Serial.println("step"); // pulse hold
  if (enc.releaseStep()) Serial.println("release step"); // released after impulse hold
  
  // check for the number of clicks
  if (enc.hasClicks(1)) Serial.println("action 1 clicks");
  if (enc.hasClicks(2)) Serial.println("action 2 clicks");

  // print the number of clicks
  if (enc.hasClicks()) {
    Serial.print("has clicks");
    Serial.println(enc.clicks);
  }
}
```

### Array of EncButton2 buttons
```cpp
// declare an array of buttons
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

### One button controls multiple variables
```cpp
// use one BUTTON to conveniently change three variables
// first - one click, then hold (press-release-press-hold)
// second - two clicks, then hold
// third - three cellsika then hold
// see port monitor

#include <EncButton.h>
EncButton<EB_TICK, 3> btn;

// variables to change
int val_a, val_b, val_c;

// change steps (signed)
int8_t step_a = 1;
int8_t step_b = 5;
int8_t step_c = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
  btn.tick();

  // pass the number of pre-clicks
  if (btn.step(1)) {
    val_a += step_a;
    Serial.print("val_a: ");
    Serial println(val_a);
  }
  if (btn.step(2)) {
    val_b += step_b;
    Serial.print("val_b: ");
    Serial println(val_b);
  }
  if (btn.step(3)) {
    val_c += step_c;
    Serial.print("val_c: ");
    Serial println(val_c);
  }

  // reverse the step to change
  // pass the number of pre-clicks
  if (btn.releaseStep(1)) step_a = -step_a;
  if (btn.releaseStep(2)) step_b = -step_b;
  if (btn.releaseStep(3)) step_c = -step_c;
}
```

<a id="versions"></a>
## Versions
- v1.1 - pullup by separate method
- v1.2 - you can pass the parameter INPUT_PULLUP / INPUT (default) to the constructor
- v1.3 - virtual holding of the encoder button is moved to a separate function + minor improvements
- v1.4 - handling pressing and releasing a button
- v1.5 - added virtual mode
- v1.6 - optimization of work in interruption
- v1.6.1 - default pullup INPUT_PULLUP
- v1.7 - big memory optimization, redesigned FastIO
- v1.8 - individual button hold timeout setting (was common for all)
- v1.8.1 - removed FastIO
- v1.9 - added separate handling of pressed turn and direction request
- v1.10 - improved released handling, lightened weight in callback mode and fixed bugs
- v1.11 - even more optimizations + button level adjustment
- v1.11.1 - Digispark compatibility
- v1.12 - added more accurate encoder algorithm EB_BETTER_ENC
- v1.13 - added experimental EncButton2
- v1.14 - releaseStep() added. Button release added to debounce
- v1.15 - added setPins() for EncButton2
- v1.16 - added EB_HALFSTEP_ENC mode for half step encoders
- v1.17 - added step with pre-clicks
- v1.18 - do not count clicks after step activation. held() and hold() can also accept pre-clicks. Redesigned and improved debounce
- v1.18.1 - fixed bug in releaseStep() (didn't return result)
- v1.18.2 - fix compiler warnings
- v1.19 - speed optimization, reduced weight in sram
- v1.19.1 - slightly increased performance
- v1.19.2 - performance increased a bit more, thanks to XRay3D
- v1.19.3 - made the button high by default in virtual mode
- v1.19.4 - fix EncButton2
- v1.20 - fixed critical bug in EncButton2
- v1.21 - EB_HALFSTEP_ENC now works for normal mode
- v1.22 - improved EB_HALFSTEP_ENC for normal mode
- v1.23 - getDir() replaced with dir()
- v2.0
    - Algorithm EB_BETTER_ENC is optimized and installed by default, EB_BETTER_ENC definate is abolished
    - Added setEncType() to set the encoder type from the program, the EB_HALFSTEP_ENC defin was removed
    - Added setEncReverse() to change the direction of the encoder from the program
    - Added setStepTimeout() to set pulse hold period, definate EB_STEP removed
    - Minor improvements and optimization
    
<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!