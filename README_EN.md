This is an automatic translation, may be incorrect in some places. See sources and examples!

# EncButton

|⚠️⚠️⚠️<br>**The new version v3 is incompatible with previous ones, see [documentation](#docs), [examples](#example) and short [migration guide](#migrate) from v2 to v3!**<br>⚠️⚠️⚠️|
|--- |

Lightweight and very functional library for encoder with button, encoder or button with Arduino
- Button
    - Event handling: press, release, click, click counter, hold, pulse hold + pre-clicks for all listed modes
    - Software debouncing
    - Support for treating two simultaneously pressed buttons as a third button
- Encoder
    - Event handling: normal turn, pressed turn, fast turn
    - Support for four types of incremental encoders
    - Highly accurate position determination algorithm
    - Interrupt buffering
- Simplicity and versatility of the API for different use cases
- Virtual mode (for example, to work with a pin expander)
- Optimized for work in interrupt (encoder)
- Fastest pin reading for AVR (ATmega328/ATmega168, ATtiny85/ATtiny13)
- Fast asynchronous algorithms for polling actions from buttons and encoders
- Tight optimization and low weight in Flash and SRAM memory: 5 bytes of SRAM (per instance) and ~350 bytes of Flash per button processing

Examples of use cases:
- A few clicks - turn on the mode (by the number of clicks)
- A few clicks + short hold - another option to turn on the mode (by the number of clicks)
- Several clicks + hold - gradual change in the value of the selected variable (by number of clicks)
- A few clicks select a variable, the encoder changes it
- Changing the step of changing the variable when turning the encoder - for example, decrease when the button is held down and increase when turning it quickly
- Navigating through the menu when turning the encoder, changing the variable when turning the clamped encoder
- Full menu navigation using two buttons (simultaneous hold to go to the next level, simultaneous press to return to the previous one)
- And so on

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Info](#info)
- [Documentation](#docs)
  - [Compilation settings](#config)
  - [Full description of classes](#class)
  - [Processing and Polling](#tick)
  - [Preclicks](#preclicks)
  - [Direct button reading](#btnread)
  - [Dive into the loop](#loop)
  - [Timeout](#timeout)
  -[busy](#busy)
  - [Receive event](#actions)
  - [Optimization](#optimise)
  - [Callbacks](#callback)
  - [Simultaneous pressing](#double)
  - [Interrupts](#isr)
  - [Array of buttons/encoders](#array)
- [Migrate from v2](#migrate)
- [Examples](#example)
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
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features
- Through the IDE library manager: find the library as during installation and click "Update"
- Manually: **delete the folder with the old version**, and then put the new one in its place."Replacement" cannot be done: sometimes new versions delete files that remain after replacement and can lead to errors!

<a id="info"></a>

## Information
### Encoder
#### Encoder type
The library supports all 4 types of *incremental* encoders, the type can be set with `setEncType(type)`:
- `EB_STEP4_LOW` - active low signal (pull up to VCC).Full cycle (4 phases) in one click.*Installed by default*
- `EB_STEP4_HIGH` - active high signal (pull up to GND).Full cycle (4 phases) in one click
- `EB_STEP2` - half period (2 phases) per click
- `EB_STEP1` - quarter period (1 phase) per click, as well as momentary encoders

![diagram](/doc/enc_type.png)

#### Recommendations
To work according to the "encoder with a button" scenario, I recommend these ([link] (https://ali.ski/cmPI2), [link] (https://ali.ski/sZbTK)) round Chinese modules with soldered anti-bounce circuits(have type `EB_STEP4_LOW` as classified above):
![scheme](/doc/encAli.png)

You can independently tie the encoder according to the following scheme (RC filters on the encoder channels + tightening all pins to VCC):
![scheme](/doc/enc_scheme.png)

> Note: by default in the library, the encoder pins are set to `INPUT` with the expectation of an external pullup.If you have an encoder without a pullup, you can use the internal `INPUT_PULLUP` by specifying this when initializing the encoder (see the documentation below).

### Button
#### Button level
The button can be connected to the microcontroller in two ways and give a high or low signal when pressed.The library provides the setting `setBtnLevel(level)`, where the level is the active signal of the button:
- `HIGH` - button connects VCC.Installed by default in `Virt` libraries
- `LOW` - the button connects GND.Installed by default in core libraries

![scheme](/doc/btn_scheme.png)

#### Pin lift
In circuits with microcontrollers, it is most often used to connect a button to GND with a pin pull to VCC.The pullup can be external (pin mode must be set to `INPUT`) or internal (pin mode `INPUT_PULLUP`).In "real" projects, an external tightening is recommended, becauseit is less prone to interference - the internal one has too high resistance.

<a id="docs"></a>

## Documentation

<a id="config"></a>

### Settings defines
Announce before linking the library

```cpp
// disable the attach event handler (saves 2 bytes of RAM)
#define EB_NO_CALLBACK

// disable encoder counter [VirtEncoder, Encoder, EncButton] (saves 4 bytes of RAM)
#define EB_NO_COUNTER

// disable encoder buffering (saves 2 bytes of ram)
#define EB_NO_BUFFER

/*
  Setting timeouts for all classes
  - Replaces timeouts with constants, it will not be possible to change them from the program (SetXxxTimeout())
  - The setting affects all buttons/encoders declared in the program
  - Saves 1 byte of RAM per object per timeout
  - Default values shown in ms
  - Values are not limited to 4000ms, as when setting from the program (SetXxxTimeout())
*/
#define EB_DEB_TIME 50 // button debounce timeout (button)
#define EB_CLICK_TIME 500 // click timeout (button)
#define EB_HOLD_TIME 500 // hold timeout (button)
#define EB_STEP_TIME 200 //pulse hold timeout (button)
#define EB_FAST_TIME 30 // fast turn timeout (encoder)
```

<a id="class"></a>

### Classes
How to work with the documentation: Starting from version 3.0, EncButton represents several libraries (classes) for various usage scenarios, they inherit each other to extend functionality.Thus, the library is an "onion", each layer of which has access to the functions of the lower layers:
- Base classes:
  - `VirtButton` - the base class of the virtual button, provides all the features of the button
  - `VirtEncoder` - the base class of the virtual encoder, determines the fact and direction of rotation of the encoder
  - `VirtEncButton` - base class of a virtual encoder with a button, provides polling of the encoder taking into account the button, *inherits VirtButton and VirtEncoder*
- Main classes:
  - `Button`, `ButtonT` - button class, *inherits VirtButton*
  - `Encoder`, `EncoderT` - encoder class, *inherits VirtEncoder*
  - `EncButton`, `EncButtonT` - encoder class with a button, *inherits VirtEncButton, VirtButton, VirtEncoder*

Thus, to study all the available functions of a particular library, you need to look not only at it, but also at what it inherits.For example, to process a button using `Button`, you need to open the description of `Button` and `VirtButton` below.

> *Virtual* - without specifying the pin of the microcontroller, works directly with the transferred value, for example, to poll encoder buttons through pin expanders and shift registers.

> `T`- versions of libraries require pins to be specified by constants (numbers).The pin numbers will be stored in the program memory, this speeds up the work and makes the code 1 byte lighter for each pin.

> Note: `#include <EncButton.h>` includes all library tools!

<details>
<summary>Button functions table</summary>

||VirtButton |VirtEncButton |button |EncButton |
|-----------------|:----------:|:-------------:|:------:|:---------:|
|read |||✔ ||
|readBtn ||||✔ |
|setHoldTimeout |✔ |✔ |✔ |✔ |
|setStepTimeout |✔ |✔ |✔ |✔ |
|setClickTimeout |✔ |✔ |✔ |✔ |
|setDebTimeout |✔ |✔ |✔ |✔ |
|setBtnLevel |✔ |✔ |✔ |✔ |
|pressISR |✔ |✔ |✔ |✔ |
|reset |✔ |✔ |✔ |✔ |
|clear |✔ |✔ |✔ |✔ |
|attach |✔ |✔ |✔ |✔ |
|press |✔ |✔ |✔ |✔ |
|release |✔ |✔ |✔ |✔ |
|click |✔ |✔ |✔ |✔ |
|pressing |✔ |✔ |✔ |✔ |
|hold |✔ |✔ |✔ |✔ |
|holding |✔ |✔ |✔ |✔ |
|step |✔ |✔ |✔ |✔ |
|hasclicks |✔ |✔ |✔ |✔ |
|getclicks |✔ |✔ |✔ |✔ |
|releaseHold |✔ |✔ |✔ |✔ |
|release step |✔ |✔ |✔ |✔ |
|time out |✔ |✔ |✔ |✔ |
|waiting |✔ |✔ |✔ |✔ |
|busy |✔ |✔ |✔ |✔ |
|action |✔ |✔ |✔ |✔ |
</details>

<details>
<summary>Encoder function table</summary>

||VirtEncoder |encoder |VirtEncButton |EncButton |
|----------------|:-----------:|:-------:|:-------------:|:---------:|
|readEnc ||||✔ |
|initEnc |✔ |✔ |Cranberries ✔ |✔ |
|setEncReverse |✔ |✔ |✔ |✔ |
|setEncType |✔ |✔ |✔ |✔ |
|setEncISR |✔ |✔ |✔ |✔ |
|turn |✔ |✔ |✔ |✔ |
|dir |✔ |✔ |✔ |✔ |
|tick Raw |✔ |✔ |✔ |✔ |
|counter |✔ |✔ |✔ |✔ |
|setFastTimeout |||✔ |✔ |
|turnH |||✔ |✔ |
|fast |||✔ |✔ |
|right |||✔ |✔ |
|left |||✔ |✔ |
|right H |||✔ |✔ |
|leftH |||✔ |✔ |
|encHolding |||✔ |✔ |
|action |||✔ |✔ |
|time out |||✔ |✔ |
|attach |||✔ |✔ |
</details>

<details>
<summary>VirtButton</summary>

```cpp
// ================ SETTINGS ================
// set hold timeout, default500 (max. 4000ms)
void setHoldTimeout(uint16_t tout);

// set pulse hold timeout, default200 (max. 4000ms)
void setStepTimeout(uint16_t tout);

// set click wait timeout, default500 (max. 4000ms)
void setClickTimeout(uint16_t tout);

// set debounce timeout, default50 (max. 255ms)
void setDebTimeout(uint8_t tout);

// set button level (HIGH - button closes VCC, LOW - closes GND)
// defaultHIGH, i.e. true - the button is pressed
void setBtnLevel(bool level);

// connect an event handler function (of the form void f())
void attach(void (*handler)());

// ================== RESET ===================
// reset system flags (force end of processing)
void reset();

// force the event flags to be cleared
void clear();

// ================ PROCESSING ================
// processing the button with a value
bool tick(bool s);

// treat the virtual button as a simultaneous press of two other buttons
bool tick(VirtButton& b0, VirtButton& b1);

// button pressed in button interrupt
void pressISR();

// ================== POLL ==================
// button pressed [event]
boolpress();

// button released (in any case) [event]
bool release();

// button click (released without holding) [event]
boolclick();

// button pressed (between press() and release()) [state]
bool pressing();

// button was held down (more than timeout) [event]
bool hold();

// button was held down (more than timeout) with pre-clicks [event]
bool hold(uint8_t num);

// button is held down (greater than timeout) [state]
bool holding();

// button is held down (more than timeout) with pre-clicks [state]
bool holding(uint8_t num);

// pulse hold [event]
boolstep();

// impulse hold with pre-clicks [event]
bool step(uint8_t num);

// multiple clicks detected [event]
bool hasClick();

// fixed the specified number of clicks [event]
bool hasClicks(uint8_t num);

// get the number of clicks
uint8_t getClicks();

// button released after holding down [event]
bool releaseHold();

// button released after holding down with pre-clicks [event]
bool releaseHold(uint8_t num);

// button released after pulse hold [event]
bool releaseStep();

// button released after pulse hold with pre-clicks [event]
bool releaseStep(uint8_t num);

// after interacting with the button (or EncButton encoder), the specified time has passed, ms [event]
bool timeout(uint16_t tout);

// button waits for repeated clicks (between click() and hasClicks()) [stateCranberries]
bool waiting();

// processing in progress (between first click and after waiting for clicks) [state]
bool busy();

// there was an action from the button, will return the event code [event]
uint16_t action();

```
</details>
<details>
<summary>VirtEncoder</summary>

```cpp
// ==================== SETTINGS ====================
// invert encoder direction (default 0)
void setEncReverse(bool rev);

// set encoder type (EB_STEP4_LOW, EB_STEP4_HIGH, EB_STEP2, EB_STEP1)
void setEncType(uint8_ttype);

// use encoder handling in interrupt
void setEncISR(bool use);

// encoder initialization
void initEnc(bool e0, bool e1);

// initialization of the encoder with a combined value
void initEnc(int8_tv);

// ===================== POLL ======================
// there was a turn [event]
bool turn();

// encoder direction (1 or -1) [state]
int8_t dir();

// counter
int32_t counter;

// ==================== PROCESSING ====================
// interrogate encoder in interrupt.Will return 1 or -1 when rotating, 0 when stopped
int8_t tickISR(bool e0, bool e1);
int8_t tickISR(int8_t state);

// poll the encoder.Will return 1 or -1 when rotating, 0 when stopped
int8_t tick(bool e0, bool e1);
int8_t tick(int8_t state);
int8_t tick();// the processing itself in the interrupt

// interrogate the encoder without setting flags for rotation (faster).Will return 1 or -1 when rotating, 0 when stopped
int8_t tickRaw(bool e0, bool e1);
int8_t tickRaw(int8_t state);
```
</details>
<details>
<summary>VirtEncButton</summary>

- Functions from `VirtButton` are available
- Available functions from `VirtEncoder`

```cpp
// ================== SETTINGS ===================
// set fast turn timeout, ms
void setFastTimeout(uint8_t tout);

// ==================== POLL ====================
// pressed encoder turn [event]
bool turnH();

// fast encoder turn [state]
boolfast();

// turn right [event]
boolright();

// turn left [event]
bool left();

// clicked turn right [event]
bool rightH();

// pressed turn left [event]
bool leftH();

// encoder button pressed [state]
bool encHolding();

// there was an action from a button or encoder, will return the event code [event]
uint16_t action();

// ==================== PROCESSING ====================
// processing in interrupt (encoder only).Will return 0 at rest, 1 or -1 when rotated
int8_t tickISR(bool e0, bool e1);

// processing in interrupt (encoder only).Will return 0 at rest, 1 or -1 when rotated
int8_t tickISR(int8_t e01);

// handle encoder and button
bool tick(bool e0, bool e1, bool btn);
bool tick(int8_t e01, bool btn);
bool tick(bool btn);// encoder in interrupt
```
</details>
<details>
<summary>Button</summary>

- Functions from `VirtButton` are available
- Default button mode - `LOW`

```cpp
button;
Button(uint8_t pin);// with pin
Button(uint8_t npin, uint8_t mode);// + operation mode (default INPUT_PULLUP)
Button(uint8_t npin, uint8_t mode, uint8_t btnLevel);// + button level (default LOW)
```
```cpp
// specify the pin and its mode of operation
void init(uint8_t npin, uint8_t mode);

// read the current value of the button (without debounce) considering setBtnLevel
boolread();

// processing function, call in loop
bool tick();
```
</details>
<details>
<summary>ButtonT</summary>

- Functions from `VirtButton` are available
- Default button mode - `LOW`

```cpp
ButtonT<uint8_tpin>;// with pin
ButtonT<uint8_t pin> (uint8_t mode);// + operation mode (default INPUT_PULLUP)
ButtonT<uint8_t pin> (uint8_t mode, uint8_t btnLevel);// + button level (default LOW)
```
```cpp
// specify the mode of operation
void init(uint8_t mode);

// read the current value of the button (without debounce) considering setBtnLevel
boolread();

// processing function, call in loop
bool tick();
```
</details>
<details>
<summary>Encoder</summary>

- Available functions from `VirtEncoder`

```cpp
encoder;
Encoder(uint8_t encA, uint8_t encB);// with pins
Encoder(uint8_t encA, uint8_t encB, uint8_t mode);// + operation mode (default INPUT)
```
```cpp
// specify pins and their mode of operation
void init(uint8_t encA, uint8_t encB, uint8_t mode);

// handling function to call in encoder interrupt
int8_t tickISR();

// processing function to call in loop
int8_t tick();
```
</details>
<details>
<summary>EncoderT</summary>

- Available functions from `VirtEncoder`

```cpp
EncoderT<uint8_t encA, uint8_t encB>;// with pins
EncoderT<uint8_t encA, uint8_t encB> (uint8_t mode);// + operation mode (default INPUT)
```
```cpp
// specify the mode of operation of the pins
void init(uint8_t mode);

// handling function to call in encoder interrupt
int8_t tickISR();

// processing function to call in loop
int8_t tick();
```
</details>
<details>
<summary>EncButton</summary>

- Functions from `VirtButton` are available
- Available functions from `VirtEncoder`
- Functions from `VirtEncButton` are available

```cpp
EncButton;

// set up pins (enc, enc, button)
EncButton(uint8_t encA, uint8_t encB, uint8_t btn);

// configure pins (enc, enc, button, pinmode enc, pinmode button, button level)
EncButton(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);
```
```cpp
// configure pins (enc, enc, button, pinmode enc, pinmode button, button level)
void init(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);

// handling function to call in encoder interrupt
int8_t tickISR();

// processing function, call in loop
bool tick();

// read the value of the button, taking into account setBtnLevel
bool readBtn();

// read encoder value
int8_t readEnc();
```
</details>
<details>
<summary>EncButtonT</summary>

- Functions from `VirtButton` are available
- Available functions from `VirtEncoder`
- Functions from `VirtEncButton` are available

```cpp
// with pins
EncButtonT<uint8_t encA, uint8_t encB, uint8_t btn>;

// + pin operation mode, button level
EncButtonT<uint8_t encA, uint8_t encB, uint8_t btn> (uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);
```
```cpp
// set the pin operation mode, button level
void init(uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW);

// handling function to call in encoder interrupt
int8_t tickISR();

// processing function, call in loop
bool tick();

// read button value
bool readBtn();

// read encoder value
int8_t readEnc();
```
</details>

<a id="tick"></a>

### Processing and Polling
All libraries have a common **processing function** (ticker `tick`) that receives the current signal from the button and encoder
- This function must be called in the main loop of the program (for virtual - with the transfer of value)
- The function returns `true` when an event occurs (for an encoder - `1` or `-1` when turning, `0` if it is absent. Thus, turning in any direction is regarded as `true`)
- There are separate functions to call in interrupt, they have `ISR` suffix, see documentation below

The library processes the signal inside this function, the result can be obtained from **functions of polling** events.They are of two types:
- `[event]` - the function will return `true` once when the event occurs.Reset after the next call to the processing function (e.g. click, encoder rotation)
- `[state]` - the function returns `true` while this state is active (for example, the button is held down)

For ease of perception, the processing function should be placed at the beginning of the loop, and the polls should be done below:
```cpp
void loop() {
  btn.tick();// survey

  if (btn.click()) Serial.println("click");// displays once when clicked
  if (btn.click()) Serial.println("click");// same click!
}
```
> Unlike previous versions of the library, polling functions are reset not internally, but *inside the processing function*.Thus, in the example above, when clicking on the button in the port, the message `click` will be displayed twice.This allows you to use the polling functions several times during the current iteration of the loop.Cranberries for creating complex logic of the program.

#### Handling the button
The library handles the button like this:
- Pressing with software debouncing (hold longer than deb timeout), results in `press` event, `pressing` and `busy` states
- Hold longer than hold timeout hold - event `hold`, state `holding`
- Hold longer than hold timeout + step timeout - impulse event `step`, triggered with step period
- Button release, result - `release` event, removal of `pressing` and `holding` states
  - Release before hold timeout - `click` event
  - Release after holding - `releaseHold` event
  - Release after impulse hold - `releaseStep` event
- Waiting for a new click within the click timeout, `waiting` state
- If there is no new click - remove the `busy` state, processing is over
- If `timeout` is expected - a timeout event with the specified period from the current moment
- Processing the button in the interrupt informs the library about the fact of pressing, all other processing is performed normally in `tick ()

> Better to see once than read a hundred times.Run the demo example and click on the button

##### Click
![click](/doc/click.gif)

##### Hold
![hold](/doc/hold.gif)

##### Step
![step](/doc/step.gif)

Online simulation available [here](https://wokwi.com/projects/373591584298469377)

#### Encoder processing
- A "fast" turn is a turn made less than the configured timeout from the previous turn
- Turns handled in the interrupt become active (raise events) after calling `tick()`

#### Handling an encoder with a button
- Turning the encoder while holding the button removes and blocks all subsequent events and clicks, except for the `release` event.Pressed button states do not change
- Turning the encoder also affects the system timeout (function `timeout()`) - will work after the specified time after turning the encoder

<a id="preclicks"></a>

### Pre-clicks
The library counts the number of button clicks and some polling functions can be handled separately with *pre-clicks*.For example 3 clicks, then hold.This greatly expands the capabilities of a single button.There are two options for working with such events:
```cpp
  // 1
  if (btn.hold()) {
    if (btn.getClicks() == 2) Serial.println("hold 2 clicks");
  }

  // 2
  if (btn.hold(2)) Serial.println("hold 2 clicks");
```

In the first option, you can get the number of clicks for further processing manually, and in the second, the library will do it itself if the number of clicks for the action is known in advance.

<a id="btnread"></a>

### Direct button reading
In some scenarios, it may be necessary to get the state of the button "here and now", for example, to determine whether the button is held down immediately after the microcontroller starts (program starts).The `tick()` function must be called constantly in a loop so that the button is processed with contact bounce damping and other calculations, so the following construction **will not work**:
```cpp
void setup() {
  btn.tick();
  if (btn.press()) Serial.println("Button pressed at start");
}
```

For such scenarios, the following functions will help, return `true` if the button is pressed:
- `read()` for Button and ButtonT libraries
- `readBtn()` for EncButton and EncButtonT libraries

> The polling of the button is performed taking into account the previously configured button level (setBtnLevel)!You do not need to manually invert the logic additionally:

```cpp
void setup() {
  // btn.setBtnLevel(LOW);// you can adjust the level

  if (btn.read()) Serial.println("Button pressed at start");
}
```

<a id="loop"></a>

### Diving into the loop
Let's say you need to process the button synchronously and with damping of the bounce.For example, if the button is pressed at the start of the microcontroller, get its hold or even impulse hold inside the `setup` block, that is, before the start of the main program execution.You can use the `busy` state and poll the button from the loop:
```cpp
void setup() {
  Serial.begin(115200);

  btn.tick();
  whilecranberry e (btn.busy()) {
    btn.tick();
    if (btn.hold()) Serial.println("hold");
    if (btn.step()) Serial.println("step");
  }

  Serial.println("program start");
}
```
How it works: the first tick polls the button, if the button is pressed, the busy state is immediately activated and the system enters the `while` loop.Inside it, we continue to tick and receive events from the button.When the button is released and all events are triggered, the busy flag will be lowered and the program will automatically exit the loop.You can rewrite this construction into a loop with a postcondition, more beautifully:
```cpp
do {
  btn.tick();
  if (btn.hold()) Serial.println("hold");
  if (btn.step()) Serial.println("step");
} while(btn.busy());
```

<a id="timeout"></a>

### Timeout
In the classes associated with the button (Button, EncButton) there is a `timeout(time)` function - it will return `true` once, if the specified time has passed after the completion of actions with the button/encoder.This can be used to save options after input, like so:
```cpp
void loop() {
  eb.tick();

  // ...

  if (eb.timeout(2000)) {
    // after interacting with the encoder, 2 seconds have passed
    // EEPROM.put(0, settings);
  }
}
```

<a id="busy"></a>

###busy
The `busy()` function returns `true` while the button is being processed, i.e.while the system is waiting for actions and exit timeouts.This can be used to optimize the code, for example, to avoid some long and heavy parts of the program while the button is being processed:
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

### Get an event
Available in all classes **with button**:
- `VirtButton`
- `Button`
- `VirtEncButton`
- `EncButton`

The `action()` function, when an event occurs, returns the event code (other than zero, which in itself is an indication of the presence of an event):
- `EB_PRESS` - pressing the button
- `EB_HOLD` - the button is held
- `EB_STEP` - impulse hold
- `EB_RELEASE` - the button is released
- `EB_CLICK` - single click
- `EB_CLICKS` - signal about several clicks
- `EB_TURN` - encoder rotation
- `EB_REL_HOLD` - button released after holding
- `EB_REL_HOLD_C` - the button is released after being held with a preliminary.clicks
- `EB_REL_STEP` - button released after step
- `EB_REL_STEP_C` - the button is released after a step with a preliminary.clicks

> The result of the `action()` function is reset after the next call to `tick()`, i.e. it is available for the entire current iteration of the main loop

The received event code can be processed via `switch`:
```cpp
switch(eb.action()) {
  case EB_PRESS:
    // ...
    break;
  case EB_HOLD:
    // ...
    break;
  // ...
}
```

<a id="optimise"></a>

### Optimization
#### Library weight
To reduce the weight of the library as much as possible (in particular in RAM), you need to set timeouts as constants via define (saving 1 byte per timeout), disable the event handler, buffer counters and use the T-class (saving 1 byte per pin):
```cpp
#define EB_NO_CALLBACK
#define EB_NO_COUNTER
#define EB_NO_BUFFER
#define EB_DEB_TIME 50 // button debounce timeout (button)
#define EB_CLICK_TIME 500 // click timeout (button)
#define EB_HOLD_TIME 500 // hold timeout (button)
#define EB_STEP_TIME 200 // pulse hold timeout (button)
#define EB_FAST_TIME 30 // fast turn timeout (encoder)
#include <EncButton.h>
EncButtonT<2, 3, 4>eb;
```
In this case, an encoder with a button will take only 8 bytes in SRAM, and just a button - 5.

#### Execution speed
To reduce the time for checking system event flags (slightly, but nicely), you can put all polls in a condition on `tick()`, since `tick()` returns `true` only when an event occurs:
```cpp
void loop() {
  if (eb.tick()) {
    if (eb.turn()) ...;
    if (eb.click()) ...;
  }
}
```

Also, polling events using the `action()` function is faster than manually polling individual event functions, so the library will work most efficiently in this format:
```cpp
void loop() {
  if (eb.tick()) {
    switch(eb.action()) {
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

<a id="callback"></a>

### Callbacks
You can connect an external event handler function, it will be called when any event occurs.This feature works in all classes **with a button**:
- `VirtButton`
- `Button`
- `VirtEncButton`
- `EncButton`

```cpp
EncButton eb(2, 3, 4);

void callback() {
  switch(eb.action()) {
    case EB_PRESS:
      // ...
      break;
    case EB_HOLD:
      // ...
      break;
    // ...
  }
}

void setup() {
  eb.attach(callback);
}

void loop() {
  eb.tick();
}
```

<a id="double"></a>

### Simultaneous pressing
The library natively supports working with two simultaneously pressed buttons as a third button.For this you need:
1. Create a virtual button `VirtButton`
2. Call processing of real buttons
3. Pass these buttons to the virtual button for processing (these can be objects of classes `VirtButton`, `Button`, `EncButton` + their `T`- versions)
4. Next poll events

```cpp
Button b0(4);
button b1(5);
VirtButton b2;// 1

void loop() {
  b0.tick();// 2
  b1.tick();// 2
  b2.tick(b0, b1);// 3

  // 4
  if (b0.click()) Serial.println("b0 click");
  if (b1.click()) Serial.println("b1 click");
  if (b2.click()) Serial.println("b0+b1 click");
}
```

The library itself will "reset" unnecessary events from real buttons if they were pressed together, with the exception of the `press` event.Thus, a full-fledged third button is obtained from the other two with a convenient poll.

<a id="isr"></a>

### Interrupts
#### Encoder
To process the encoder in the downloaded program, you need:
- Connect both of its pins to hardware interrupts by `CHANGE`
- Set `setEncISR(true)`
- Call a special interrupt ticker in the handler
- The main ticker also needs to be called in a `loop` to work correctly - events are generated in the main ticker:
```cpp
// example for ATmega328 and EncButton
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

Note: the use of work in the interrupt allows you to correctly process the position of the encoder and not miss a new turn.The twist event received from the interrupt will become available *after* calling `tick` in the main loop of the program, which allows you to not break the sequence of the main loop:
- Buffering disabled: `turn` event fired only once, regardless of the number of encoder clicks made between two `tick` calls (clicks handled in interrupt)
- Buffering is enabled: the `turn` event will be called as many times as there were actually clicks of the encoder, this allows you not to skip turns at all and not load the system in an interrupt.**Buffer size - 5 raw encoder clicks**

Notes:
- The `setEncISR` function only works in non- virtual classes.If it is enabled, the main ticker `tick` simply does not poll the encoder pins, which saves CPU time.Processing occurs only in the interrupt
- The encoder counter is always up to date and can lead the buffered turns in the program with large delays in the main loop!
- On different platforms, interrupts can work differently (for example, on ESPxx - you need to add the `IRAM_ATTR` attribute to the function, see the documentation for your platform!)
- A handler attached to `attach()` will be called from `tick()`, i.e. *not from an interrupt*!

#### Virtual classes
There is a ticker in the virtual ones, to which you do not need to transfer the state of the encoder, if it is processed in an interrupt, this allows you not to poll the idle pins.For example:

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
    e.tick();// don't pass pin states
}
```

#### Button
To process a button in an interrupt, you need:
- Connect an interrupt to **click** a button, taking into account its physical connection and level:
  - If the button closes `LOW` - interrupt `FALLING`
  - If the button closes `HIGH` - interrupt `RISING`
- Call `pressISR()` in an interrupt handler

```cpp
buttonb(2);

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

Note: the button is handled primarily by `tick()`, and the `pressISR()` function just tells the library that the button was pressed outside of `tick()`.This allows you not to miss a button press while the program was busy with something else.

<a id="array"></a>

### Array of buttons/encoders
You can create an array only from non-template classes (without the letter `T`), because the pin numbers will have to be specified already in the runtime later in the program.For example:
```cpp
Button btns[5];
EncButton ebs[3];

void setup() {
  btns[0].init(2);// specify pin
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

### Porting to other platforms
In the utils.h/utils.cpp files, you can replace the Arduino-dependent functions with others

<a id="migrate"></a>

## Migration guide from v2 to v3
### Initialization
```cpp
// VIRTUAL
VirtEncButton eb;// encoder with button
VirtButton b;// button
VirtEncoder e;// encoder

// REAL
// encoder with button
EncButton eb(enc0, enc1, btn);// encoder pins and buttons
EncButton eb(enc0, enc1, btn, modeEnc);// + encoder pin mode (default INPUT)
EncButton eb(enc0, enc1, btn, modeEnc, modeBtn);// + button pin mode (default INPUT_PULLUP)
EncButton eb(enc0, enc1, btn, modeEnc, modeBtn, btnLevel);// + button level (default LOW)
// template
EncButton<enc0, enc1, btn> eb;// encoder pins and buttons
EncButton<enc0, enc1, btn> eb(modeEnc);// + encoder pin mode (default INPUT)
EncButton<enc0, enc1, btn> eb(modeEnc, modeBtn);// + button pin mode (default INPUT_PULLUP)
EncButton<enc0, enc1, btn> eb(modeEnc, modeBtn, btnLevel);// + button level (default LOW)

// button
Buttonb(pin);// pin
Button b(pin, mode);// + button pin mode (default INPUT_PULLUP)
Button b(pin, mode, btnLevel);// + button level (default LOW)
// template
ButtonT<pin>b;// pin
ButtonT<pin> b(mode);// + button pin mode (default INPUT_PULLUP)
ButtonT<pin> b(mode, btnLevel);// + button level (default LOW)

// encoder
Encoder e(enc0, enc1);// encoder pins
Encoder e(enc0, enc1, mode);// + encoder pin mode (default INPUT)
// template
EncoderT<enc0, enc1>e;// encoder pins
EncoderT<enc0, enc1> e(mode);// + encoder pin mode (default INPUT)
```

### Functions
|v2 |v3 |
|-----------|--------------|
|`held()` |`hold()` |
|`hold()` |`holding()` |
|`state()` |`pressing()` |
|`setPins()`|`init()` |

- The order of specifying pins has changed (see the dock above)
- `clearFlags()` replaced with `clear()` (reset event flags) and `reset()` (reset system processing flags, finish processing)

### Operation logic
In v3, the event polling functions (click, turn...) are not reset immediately after they are called - they are reset on the next `tick()` call, thus retaining their value in all subsequent calls in the current iteration of the main program loop.

<a id="example"></a>
## Examples
See **examples** for other examples!
<details>
<summary>Full EncButton Demo</summary>

```cpp
// #define EB_NO_CALLBACK // disable the attach event handler (economyIT 2 bytes of RAM)
// #define EB_NO_COUNTER // disable the encoder counter (saves 4 bytes of RAM)
// #define EB_NO_BUFFER // disable encoder buffering (saves 1 byte of RAM)

// #define EB_DEB_TIME 50 // button debounce timeout (button)
// #define EB_CLICK_TIME 500 // click timeout (button)
// #define EB_HOLD_TIME 500 // hold timeout (button)
// #define EB_STEP_TIME 200 // pulse hold timeout (button)
// #define EB_FAST_TIME 30 // fast turn timeout (encoder)

#include <EncButton.h>
EncButton eb(2, 3, 4);
//EncButton eb(2, 3, 4, INPUT);// + encoder pin mode
//EncButton eb(2, 3, 4, INPUT, INPUT_PULLUP);// + button pin mode
//EncButton eb(2, 3, 4, INPUT, INPUT_PULLUP, LOW);// + button level

void setup() {
    Serial.begin(115200);

    // default values shown
    eb.setBtnLevel(LOW);
    eb.setClickTimeout(500);
    eb.setDebTimeout(50);
    eb.setHoldTimeout(500);
    eb.setStepTimeout(200);

    eb.setEncReverse(0);
    eb.setEncType(EB_STEP4_LOW);
    eb.setFastTimeout(30);
}

void loop() {
    eb.tick();

    // handle rotation general
    if (eb.turn()) {
        Serial.print("turn: dir");
        Serial.print(eb.dir());
        Serial.print(", fast");
        Serial.print(eb.fast());
        Serial.print(", hold");
        Serial.print(eb.encHolding());
        Serial.print(", counter");
        Serial.println(eb.counter);
    }

    // handle rotation separately
    if (eb.left()) Serial.println("left");
    if (eb.right()) Serial.println("right");
    if (eb.leftH()) Serial.println("leftH");
    if (eb.rightH()) Serial.println("rightH");

    // button
    if (eb.press()) Serial.println("press");
    if (eb.release()) Serial.println("release");
    if (eb.click()) Serial.println("click");

    // states
    // Serial.println(eb.pressing());
    // Serial.println(eb.holding());
    // Serial.println(eb.busy());
    // Serial.println(eb.waiting());

    // timeout
    if (eb.timeout(1000)) Serial.println("timeout!");

    // hold
    if (eb.hold()) Serial.println("hold");
    // if (eb.hold()) {
    // Serial.print("hold + ");
    // Serial.print(eb.getClicks());
    // Serial.println("clicks");
    // }

    if (eb.hold(2)) Serial.println("hold 2");
    if (eb.hold(3)) Serial.println("hold 3");

    // pulse hold
    if(eb.step()) Serial.println("step");
    if (eb.step(2)) Serial.println("step 2");
    if (eb.step(3)) Serial.println("step 3");

    // released after impulse hold
    if (eb.releaseStep()) Serial.println("release step");
    if (eb.releaseStep(2)) Serial.println("release step 2");
    if (eb.releaseStep(3)) Serial.println("release step 3");

    // released after hold
    if (eb.releaseHold()) Serial.println("release hold");
    if (eb.releaseHold(2)) Serial.println("release hold 2");

    // check for the number of clicks
    if (eb.hasClicks(1)) Serial.println("has 1 clicks");
    if (eb.hasClicks(3)) Serial.println("has 3 clicks");

    // print the number of clicks
    if (eb.hasClicks()) {
        Serial.print("has clicks: ");
        Serial.println(eb.clicks);
    }
}
```
</details>
<details>
<summary>Connecting a handler</summary>

```cpp
#include <EncButton.h>
EncButton eb(2, 3, 4);

void callback() {
    Serial.print("callback: ");
    switch(eb.action()) {
        case EB_PRESS:
            Serial.println("press");
            break;
        case EB_HOLD:
            Serial println("hold");
            break;
        case EB_STEP:
            Serial.println("step");
            break;
        case EB_RELEASE:
            Serial println("release");
            break;
        case EB_CLICK:
            Serial.println("click");
            break;
        case EB_CLICKS:
            Serial.print("clicks");
            Serial.println(eb.getClicks());
            break;
        case EB_TURN:
            serial print("turn");Serial.print(eb.dir());
            serial print(" ");
            Serial.print(eb.fast());
            serial print(" ");
            Serial.println(eb.encHolding());
            break;
        case EB_REL_HOLD:
            Serial.println("release hold");
            break;
        case EB_REL_HOLD_C:
            Serial.print("release hold clicks");
            Serial.println(eb.getClicks());
            break;
        case EB_REL_STEP:
            Serial.println("release step");
            break;
        case EB_REL_STEP_C:
            Serial.print("release step clicks");
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
```
</details>
<details>
<summary>All button types</summary>

```cpp
#include <EncButton.h>

Button btn(4);
ButtonT<5> btnt;
VirtButton btnv;

void setup() {
    Serial.begin(115200);
}

void loop() {
    // button
    btn.tick();
    if (btn.click()) Serial.println("btn click");

    // ButtonT
    btnt.tick();
    if (btnt.click()) Serial.println("btnt click");

    // VirtButton
    btnv.tick(!digitalRead(4));// pass boolean value
    if (btn.click()) Serial.println("btnv click");
}
```
</details>
<details>
<summary>All encoder types</summary>

```cpp
#include <EncButton.h>

Encoder enc(2, 3);
EncoderT<5, 6> enct;
VirtEncoder encv;

void setup() {
    Serial.begin(115200);
}

void loop() {
    // the poll is the same for everyone, 3 ways:

    // 1
    // tick will return 1 or -1, so this is a step
    if (enc.tick()) Serial.println(enc.counter);

    // 2
    // can be polled via turn()
    enct.tick();
    if (enct.turn()) Serial.println(enct.dir());

    // 3
    // you can not use the polling functions, but get the direction directly
    int8_t v = encv.tick(digitalRead(2), digitalRead(3));
    if (v) Serial.println(v);// prints 1 or -1
}
```
</details>

<a id="versions"></a>
## Versions
<details>
<summary>Older</summary>

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
- v1.14 - releaseStep() added.Button release added to debounce
- v1.15 - added setPins() for EncButton2
- v1.16 - added EB_HALFSTEP_ENC mode for half step encoders
- v1.17 - added step with pre-clicks
- v1.18 - do not count clicks after step activation.hold() and held() can also accept pre-clicks.Redesigned and improved debounce
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
    - extsetEncType() is enabled to set the encoder type from the program, the EB_HALFSTEP_ENC defin is deprecated
    - Added setEncReverse() to change the direction of the encoder from the program
    - Added setStepTimeout() to set pulse hold period, definate EB_STEP removed
    - Minor improvements and optimization
</details>

- v3.0
  - Library rewritten from scratch, incompatible with previous versions!
    - Completely different object initialization
    - Renamed: hold()->holding(), held()->hold()
  - Optimization of Flash memory: the library weighs less, in some scenarios - a few kilobytes
  - Optimization of code execution speed, including interrupts
  - A few bytes less RAM, multiple optimization levels to choose from
  - Easier, clearer and more convenient use
  - More readable source code
  - Split into classes for use in different scenarios
  - New functions, features and handlers for button and encoder
  - Encoder buffering in interrupt
  - Native processing of two simultaneously pressed buttons as a third button
  - Support for 4 types of encoders
  - Rewritten documentation
  - EncButton now replaces GyverLibs/VirtualButton (archived)
- v3.1
  - Extended button initialization
  - Removed holdEncButton() and toggleEncButton()
  - Added turnH()
  - Optimized encoder interrupts, added setEncISR()
  - Buffering direction and quick turn
  - Highly optimized speed of action() (common handler)
  - Added connection of an external event handler function
  - Added button handling in interrupt - pressISR()
    
<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better yet, immediately write to [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!

When reporting bugs or incorrect work of the library, it is necessary to indicate:
- Library version
- What MK is used
- SDK version (for ESP)
- Arduino IDE Version
- Whether built-in examples work correctly, which use functions and constructs that lead to a bug in your code
- What code was loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimal code