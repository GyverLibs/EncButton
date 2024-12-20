This is an automatic translation, may be incorrect in some places. See sources and examples!

# ENCBUTTON

|⚠️⚠️⚠️ <br> ** The new version of V3 is incompatible with the previous ones, see [documentation] (#docs), [examples] (# Example) and brief [migration guide] (#migrate) from v2 to v3! ** <*** <BR> ⚠️⚠️⚠️ |
| ----------------------------------------------------------------------------------------------------------------------------------------------------- |

A light and very functional library for an encoder with a button, encoder or buttons with Arduino
- Button
    - processing of events: pressing, releasing, click, click counter, retention, impulse retention, deduction time + preliminary clicks for all modes
    - Program suppression of rubbish
    - support for processing two simultaneously pressed buttons as a third button
- Encoder
    - Processing of events: a normal turn, pressed turn, fast turn
    - Support of four types of incidental encoders
    - high -precision algorithm for determining the position
    - buffer in interruption
- simple and understandable use
- a huge number of opportunities and their combinations for different scenarios for using even one button
- virtual regime (for example, for working with a pain expander)
- optimized to work in interruption
- The fastest reading of pins for AVR, ESP8266, ESP32 (used by gyverio)
- Fast asynchronous algorithms of survey of actions from the button and encoder
- rigid optimization and low weight in Flash and SRAM memory: 5 bytes SRAM (on an instance) and ~ 350 bytes Flash to process the button

Examples of use scenarios:
- Several clicks - inclusion of the regime (according to the number of clicks)
- Several clicks + short retention - another option for turning on the mode (according to the number of clicks)
- several clicks + holding - a gradual change in the value of the selected variable (on the number of clicks)
- Several clicks choose a variable, encoder changes it
- changing the step of changes in the variable during the rotation of the encoder - for example, a decrease with a closed button and an increase with rapid rotation
- navigation by menu during the rotation of the encoder, a change in the variable during the rotation of a clamped encoder
- full -fledged navigation by the menu when using two buttons (simultaneous retention to go to the next level, simultaneous pressing for return to the previous one)
- And so on

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [installation] (# Install)
- [Information] (# Info)
- [documentation] (#docs)
  - [compilation settings] (#config)
  - [full description of classes] (#class)
  - [processing and survey] (#tick)
  - [preliminary clicks] (# PRECLICS)
  - [direct reading of the button] (#btnread)
  - [immersion in the cycle] (#loop)
  - [Timeout] (# Timeout)
  - [Busy] (# Busy)
  - [receipt of an event]
  - [Optimization] (# Optimise)
  - [Collback] (#callback)
  - [Simultaneous pressing] (# Double)
  - [interruption] (# ISR)
  - [array of buttons/encoders] (# Array)
  - [custom functions] (# Custom)
  - [Timer survey] (# Timer)
  - [Mini examples, scenarios] (# Examples-Mini)
- [migration with v2] (#migrate)
- [Examples] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- For work, a library is required [gyverio] (https://github.com/gyverlibs/gyverio)
-Library can be found by the name ** encbutton ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/encbuton/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="info"> </a>

## Information
## encoder
#### type of encoder
The library supports all 4 types of * incidental * encoders, the type can be configured using `setenctype (type)`:
- `eb_step4_Low` - active low signal (tightening to VCC).Full period (4 phases) per click.*Set by default*
- `eb_step4_high` - an active high signal (tightening to GND).Full period (4 phases) for one click
- `eb_step2` - half of the period (2 phases) per click
- `eb_step1` - a quarter of the period (1 phase) per click, as well as encoders without fixation

! [Diagram] (/doc/enc_type.png)

#### Recommendations
To work according to the Encoder with the button, I recommend these ([link] (https://ali.ski/cmpi2), [link] (https://ali.ski/szbtk)) Round Chinese modules with broken anti -ship chains(have the type `eb_step4_low` according to the classification above):
! [Scheme] (/doc/encali.png)

You can tie an encoder yourself according to the following scheme (RC filters to the encoder channels + tightening of all pens to VCC):
! [Scheme] (/doc/enc_scheme.png)

> Note: by default in the library of Pino Encoder, you are configured by `Input` with the calculation of an external luster.If you have an encoder without lifting, you can use the internal `Input_pullup`, indicating this in the initialization of the encoder (see the documentation below).

### Button
### The level of the button
The button can be connected to the microcontroller in two ways and give a high or low signal when pressed.The library provides for setting up `setbtnlevel (level)`, where the level is an active button signal:
- `High` - the button connects the VCC.Installed by default in `virt`-bibliotexes
- `Low` - the button connects GND.Set by default in the main libraries

! [Scheme] (/doc/btn_scheme.png)

#### Pin
In diagrams with microcontrollers, the connection of the GND button with a PIN suspension to VCC is most often used.The tightening can be external (Pin mode must be put `Input`) or internal (PIN mode` Input_pullup`).In "real" projects, an external lifelong is recommended, becauseIt is less susceptible to interference - the internal has too high resistance.

<a id="docs"> </a>

## Documentation

<a id="config"> </a>

### Defaine settings
Be up to the library

`` `CPP

// Disable PressFor/Holdfor/StepFor support and Stepov counter (saves 2 bytes of RAM)
#define eb_no_for

// Disable the event processor attach (saves 2 bytes of RAM)
#define eb_no_callback

// Disable the encoder counter [Virtencoder, Encoder, Encbutton] (saves 4 bytes of RAM)
#define eb_no_counter

// Disconnect the buffer of the encoder (saves 2 bytes of RAM)
#define eb_no_buffer

/*
  Setting up timeouts for all classes
  - replaces the timauts constants, changeCranberries from the program (setxxxtimeout ()) will not be
  - Setting affects all buttons announced in the program/Encoders
  - saves 1 bytes of RAM for an object for each timeout
  - shows the default values in MS
  - values are not limited to 4000MS, as when installing from the program (SetXXXTimeout ())
*/
#define eb_deb_time 50 // Timesout to extinguish the trim button (button)
#define eb_click_time 500 // Click Stayout (button)
#define eb_hold_time 600 // Maintenance Times (button)
#define eb_step_time 200 // Impulse retention Timesout (button)
#define EB_FAST_TIME 30 // TIMAUT RAM (ENCODER)
`` `

<a id="class"> </a>

### classes
How to work with the documentation: Encbutton starting with version 3.0 is several libraries (classes) for various use scenarios, they inherit each other to expand functionality.Thus, the library is a “onion”, each layer of which has access to the functions of the lower layers:
- Basic classes:
  - `virtbutton` - the base class of the virtual button, provides all the possibilities of the buttons
  - `virtencoder` - the base class of the virtual encoder, determines the fact and direction of the rotation of the enkoder
  - `virtencbutton` - the base class of a virtual encoder with a button, provides an encoder poll taking into account the button, *inherits Virtbututton and Virtencoder *
- Main classes:
  - `Button`,` buttont` - button class, *inherits virtbutton *
  - `Encoder`,` Encodert` - Encoder class, *inherits virtencoder *
  - `ENCBUTTON`,` ENCBUTTONT` - ENCODER class with a button, *inherits VirtenCbutton, Virtbutton, Virtencoder *

Thus, to study all the available functions of a particular library, you need to watch not only it, but also what it inherits.For example, to process the button using `Button`, you need to open below the description of` button` and `virtbutton`.

> * Virtual * - without specifying a PIN of the microcontroller, works directly with the transmitted value, for example, for a survey of the enemies buttons through pain extensors and shift registers.

> `T'- version of libraries require indicating Pino constants (numbers).Pino numbers will be stored in the memory of the program, this accelerates the work and makes the code easier by 1 byte for each pin.

> Note: `# Include <encbutton.h>` connects all the library tools!

<details>
<summary> Table of Functions of the button </ Summary>

||Virtbutton |Virtencbutton |Button |Encbutton |
| ---------------- |: -------------------------------- |: ----------------: |:--------: |: ---------: |
|Read |||✔ ||
|Readbtn ||||✔ |
|TickRaW |✔ |✔ |✔ |✔ |
|SetHoldtimeout |✔ |✔ |✔ |✔ |
|Setsteptimeout |✔ |✔ |✔ |✔ |
|SetClicktimeout |✔ |✔ |✔ |✔ |
|Setdebtimeout |✔ |✔ |✔ |✔ |
|Setbtnlevel |✔ |✔ |✔ |✔ |
|Pressisr |✔ |✔ |✔ |✔ |
|Reset |✔ |✔ |✔ |✔ |
|Clear |✔ |✔ |✔ |✔ |
|Attach |✔ |✔ |✔ |✔ |
|Detach |✔ |✔ |✔ |✔ |
|Press |✔ |✔ |✔ |✔ |
|Release |✔ |✔ |✔ |✔ |
|Click |✔ |✔ |✔ |✔ |
|Pressing |✔ |✔ |✔ |✔ |
|Hold |✔ |✔ |✔ |✔ |
|Holding |✔ |✔ |✔ |✔ |
|STEP |✔ |✔ |✔ |✔ |
|Hasclicks |✔ |✔ |✔ |✔ |
|GetClicks |✔ |✔ |✔ |✔ |
|Getsteps |✔ |✔ |✔ |✔ |
|Releasehold |✔ |✔ |✔ |✔ |
|ReleaseStep |✔ |✔ |✔ |✔ |
|Waiting |✔ |✔ |✔ |✔ |
|Busy |✔ |✔ |✔ |✔ |
|Action |✔ |✔ |✔ |✔ |
|Timeout |✔ |✔ |✔ |✔ |
|Pressfor |✔ |✔ |✔ |✔ |
|Holdfor |✔ |✔ |✔ |✔ |
|STEPFOR |✔ |✔ |✔ |✔ |
</details>

<details>
<summary> Encoder functions table </ Summary>

||Virtencoder |Encoder |Virtencbutton |Encbutton |
| -------------- |: ----------------------------- |: -------: |: -----------------------------: |: ---------: |
|Readenc ||||✔ |
|Initenc |✔ |✔ |✔ |✔ |
|Setencreverse |✔ |✔ |✔ |✔ |
|Setenctype |✔ |✔ |✔ |✔ |
|Setencisr |✔ |✔ |✔ |✔ |
|Clear |✔ |✔ |✔ |✔ |
|Turn |✔ |✔ |✔ |✔ |
|Dir |✔ |✔ |✔ |✔ |
|TickRaW |✔ |✔ |✔ |✔ |
|Pollenc |✔ |✔ |✔ |✔ |
|Counter |✔ |✔ |✔ |✔ |
|SetFasttimeout |||✔ |✔ |
|Turnh |||✔ |✔ |
|Fast |||✔ |✔ |
|Right |||✔ |✔ |
|Left |||✔ |✔ |
|Righth |||✔ |✔ |
|Lefth |||✔ |✔ |
|Action |||✔ |✔ |
|Timeout |||✔ |✔ |
|Attach |||✔ |✔ |
|Detach |||✔ |✔ |
</details>

<details>
<summary> virtbutton </summary>

`` `CPP
// ==ward
// Set the deduction timeout, silence.600 (max. 4000 ms)
VOID SetHoldtimeout (Uint16_T Tout);

// Install the timout of impulse retention, silence.200 (max. 4000 ms)
VOID Setsteptimeout (Uint16_T Tout);

// Install the expectations of clicks, silence.500 (max. 4000 ms)
VOID setClicktimeout (Uint16_T Tout);

// Install the Timout of the Anti -Direct, silence.50 (Max. 255 ms)
VOID Setdebtimeout (Uint8_t Tout);

// set the level of the button (HIGH - button closes VCC, Low - closes GND)
// silent.High, that is, True - the button is pressed
VOID setbtnlevel (Bool LEVEL);

// Connect the function-processor of events (type VOID F ())
VOID attach (VOID (*handler) ());

// Disconnect the event-handle function
VOID Detach ();

// ==============ward
// throw off system flags (forcibly finish processing)
VOID Reset ();

// forcibly drop the flags of events
Void Clear ();

// ====ward
// button processing with value
Bool Tick (Bool S);

// Processing of the virtual button as simultaneous pressing two other buttons
Bool Tick (Virtbutton & B0, Virtbutton & B1);

// button pressed in the interruption of the button
VOID Pressisr ();

// Processing the button without reseting events and calling collobes
Bool Tickrad (Bool S);

// ================== Poll ================================ward
// Pressure button [event]
Bool Press ();
Bool Press (Uint8_T Clicks);

// button released (in any case) [evente]
Bool Release ();
Bool Release (Uint8_T Clicks);

// click on the button (released without holding) [event]
Bool click ();
Bool Click (Uint8_T Clicks);

// Squeezed button (between Press () and Release ()) [condition]
Bool Pressing ();
Bool Pressing (Uint8_T Clicks);

// The button was withheld (more timeout) [event]
Bool Hold ();
Bool Hold (Uint8_T Clicks);

// The button is held (more timeout) [condition]
Bool Holding ();
Bool Holding (Uint8_T Clicks);

// Impulse retention [event]
Bool Step ();
Bool Step (Uint8_T Clicks);

// Several clicks were recorded [event]
Bool HasClicks ();
Bool HasClicks (Uint8_T Clicks);

// button released after holding [Event]
Bool ReleaseHold ();
Bool ReleaseHold (Uint8_T Clicks);

// Button is released after impulse retention [Event]
Bool ReleaseStep ();
Bool ReleaseStep (Uint8_T Clicks);

// get the number of clicks
uint8_t getclicks ();

// Get the number of steps
uint16_t getsteps ();

// button awaits repeated clicks (between Click () and HasClicks ()) [condition]
Bool Waiting ();

// processing (between the first press and after waiting for clicks) [condition]
Bool Busy ();

// there was an action from the button, the event code [event] will return
Uint16_T Action ();

// ==========================================================================
// After interacting with the button (or enkoder Encbutton), the specified time has passed, ms [event]
Bool Timeout (Uint16_T MS);

// The time that the button is held (from the beginning of the press), ms
uint16_t Pressfor ();

// The button is held longer than (from the beginning of pressing), ms [condition]
Bool Pressfor (Uint16_T MS);

// The time that the button is held (from the beginning of retention), ms
uint16_t holdfor ();

// The button is held longer than (from the beginning of retention), ms [condition]
Bool Holdfor (Uint16_T MS);

// The time that the button is held (from the beginning of the step), ms
uint16_t stepfor ();

// The button is held longer than (from the beginning of the step), ms [condition]
Bool StepFor (Uint16_T MS);
`` `
</details>
<details>
<summary> virtencoder </summary>

`` `CPP
// ===============================================
// Invert the direction of the encoder (silence 0)
VOID Setencreverse (Bool Rev);

// Install the type of encoder (eb_step4_low, eb_step4_high, eb_step2, eb_step1)
VOID Setenctype (Uint8_T Type);

// use encoder processing in interruption
VOID Setencisr (Bool Use);

// Initialization of the encoder
VOID Initenc (Bool E0, Bool E1);

// Encoder initialization with a combined value
VOID Initenc (int8_t v);

// throw off the flags of events
Void Clear ();

// ===================== Support ==========================
// there was a turn [event]
Bool Turn ();

// Direction of the encoder (1 or -1) [condition]
int8_t die ();

// counter
Int32_T Counter;

// ====ward
// Interrogate the encoder in interruption.Will return 1 or -1 during rotation, 0 when stopping
Int8_t Tickisr (Bool E0, Bool E1);
Int8_T Tickisr (int8_t state);

// Introduce the Encoder.Will return 1 or -1 during rotation, 0 when stopping
Int8_T Tick (Bool E0, Bool E1);
int8_t tick (int8_t state);
int8_t tick ();// The processing itself in interruption

// Introduce the Encoder without resetting the turning event.Will return 1 or -1 during rotation, 0 when stopping
Int8_T TickRaW (Bool E0, Bool E1);
Int8_T TickRaW (int8_t state);
Int8_T TickRaW ();// The processing itself in interruption

// Introduce the encoder without installing flags on a turn (faster).Will return 1 or -1 during rotation, 0 when stopping
Int8_t Pollenc (Bool E0, Bool E1);
Int8_t Pollenc (Int8_T State);
`` `
</details>
<details>
<summary> virtencbutton </summary>

- Available functions from `virtbutton`
- Available functions from `virtencoder`

`` `CPP
// ============ward
// Install a timeout of fast turning, ms
VOID setfasttimeout (Uint8_t Tout);

// throw the flags of encoder and buttons
Void Clear ();

// =================== Poll =================================
// Any turn of the encoder [event]
Bool Turn ();

// pressed turn of enkoder [event]
Bool Turnh ();

// Fast turning of the encoder [condition]
Bool Fast ();

// Open Turn to the right [event]
Bool Right ();

// Non -pressed turnfrom left [event]
Bool Left ();

// pressed turn to the right [event]
Bool Righth ();

// pressed turn to the left [event]
Bool Lefth ();

// there was an action from a button or encoder, will return the event code [event]
Uint16_T Action ();

// ====ward
// processing in interruption (only encoder).Will return 0 at rest, 1 or -1 when turning
Int8_t Tickisr (Bool E0, Bool E1);
Int8_T Tickisr (int8_t e01);

// processing of encoder and buttons
Bool Tick (Bool E0, Bool E1, Bool BTN);
Bool Tick (Int8_t E01, Bool BTN);
Bool Tick (Bool BTN);// Encoder in interruption

// Processing the encoder and buttons without discharge flags and calling collobes
Bool Tickrad (Bool E0, Bool E1, Bool BTN);
Bool Tickrade (Int8_t E01, Bool BTN);
Bool Tickrade (Bool BTN);// Encoder in interruption
`` `
</details>
<details>
<summary> Button </summary>

- Available functions from `virtbutton`
- default buttons mode - `Low`

`` `CPP
Button;
Button (uint8_t pin);// indicating Pin
Button (uint8_t npin, uint8_t mode);// + mode of operation (silence input_pullup)
Button (uint8_t npin, uint8_t mode, uint8_t btnlevel);// + button level (silence)
`` `
`` `CPP
// indicate the pin and its operating mode
VOID Init (uint8_t npin, uint8_t mode);

// Read the current value of the button (without debate) taking into account Setbtnlevel
Bool Read ();

// processing function, call in loop
Bool Tick ();

// Processing the button without reseting events and calling collobes
Bool Tickrade ();
`` `
</details>
<details>
<summary> buttont </summary>

- Available functions from `virtbutton`
- default buttons mode - `Low`

`` `CPP
Buttont <uint8_t pin>;// indicating Pin
Buttont <uint8_t pin> (uint8_t mode);// + mode of operation (silence input_pullup)
Buttont <uint8_t pin> (uint8_t mode, uint8_t btnlevel);// + button level (silence)
`` `
`` `CPP
// specify the operating mode
VOID Init (Uint8_t Mode);

// Read the current value of the button (without debate) taking into account Setbtnlevel
Bool Read ();

// processing function, call in loop
Bool Tick ();
`` `
</details>
<details>
<summary> encoder </summary>

- Available functions from `virtencoder`

`` `CPP
Encoder;
Encoder (Uint8_t Enca, Uint8_T ENCB);// indicating Pinov
Encoder (Uint8_t Enca, Uint8_t Encb, Uint8_t Mode);// + mode of operation (silence. Input)
`` `
`` `CPP
// Indicate pins and their operating mode
VOID Init (Uint8_t Enca, Uint8_t Encb, Uint8_t Mode);

// Function of processing for calling in an interruption of encoder
int8_t tickisr ();

// Function of processing for calling in LOOP
int8_t tick ();
`` `
</details>
<details>
<summary> encodert </summary>

- Available functions from `virtencoder`

`` `CPP
Encodert <uint8_t enca, uint8_t encb>;// indicating Pinov
Encodert <uint8_t enca, uint8_t encb> (Uint8_t Mode);// + mode of operation (silence. Input)
`` `
`` `CPP
// specify the mode of operation of Pinov
VOID Init (Uint8_t Mode);

// Function of processing for calling in an interruption of encoder
int8_t tickisr ();

// Function of processing for calling in LOOP
int8_t tick ();
`` `
</details>
<details>
<summary> encbutton </summary>

- Available functions from `virtbutton`
- Available functions from `virtencoder`
- Available functions from `virtencbutton`

`` `CPP
ENCBUTTON;

// Set the Pins (ENK, ENK, button)
ENCBUTTON (UINT8_T ENCA, UINT8_T ENCB, UINT8_T BTN);

// Reference Pins (ENK, ENK, button, Pinmode ENK, Pinmode button, button level)
ENCBUTTON (UINT8_T ENCA, UINT8_T ENCB, UINT8_T BTN, UINT8_T MODEENC = Input, Uint8_t Modebtn = Input_Pullup, Uint8_T BTNLEVEL = LOW);
`` `
`` `CPP
// Reference Pins (ENK, ENK, button, Pinmode ENK, Pinmode button, button level)
VOID Init (Uint8_t Enca, Uint8_t Encb, Uint8_t BTN, UINT8_T MODEENC = Input, Uint8_t Modebtn = Input_Pullup, Uint8_T BTNLEVEL = LOW);

// Function of processing for calling in an interruption of encoder
int8_t tickisr ();

// processing function, call in loop
Bool Tick ();

// Read the value of the button taking into account Setbtnlevel
Bool Readbtn ();

// Read the value of the encoder
Int8_T Readenc ();
`` `
</details>
<details>
<summary> encbuttont </summary>

- Available functions from `virtbutton`
- Available functions from `viRencoder`
- Available functions from `virtencbutton`

`` `CPP
// indicating Pinov
ENCBUTTONT <uint8_T ENCA, UINT8_T ENCB, UINT8_T BTN>;

// + Pino operation mode, button level
ENCBUTTONT <uINT8_T ENCA, UINT8_T ENCB, UINT8_T BTN> (Uint8_t Modeenc = Input, Uint8_t Modebtn = Input_Pullup, Uint8_T BTNlevel = Low);
`` `
`` `CPP
// Configure Pino operation mode, button level
VOID Init (Uint8_t Modeenc = Input, Uint8_t Modebtn = Input_pullup, Uint8_t Btnlevel = Low);

// Function of processing for calling in an interruption of encoder
int8_t tickisr ();

// processing function, call in loop
Bool Tick ();

// Read the button value
Bool Readbtn ();

// Read the value of the encoder
Int8_T Readenc ();
`` `
</details>

<a id="tick"> </a>

### Processing and Poll
All libraries have a general ** function of processing ** (ticker `tick`), which receives the current signal from the button and encoder
- this function must be caused once in the main cycle of the program (for virtual - with the transmission of the meaning)
- the function returns `true` when the event occurs (for encoder -` 1` or `-1` when turning,` 0` in its absence. Thus, the turn in any direction is regarded as `true`)
- There are separate functions for calling in interruption, they have a suffix `isr`, see documentation below

The library processes the signal inside this function, the result can be obtained from ** functions of the survey ** events.They are of two types:
- `[event]` - the function will return `true` once upon the event of an event.It will be reset after the next call call (for example, click, turning enncoder)
- `[condition]` - the function returns `true`, while this condition is actively (for example, the button is held)

For simplicity of perception, the processing function must be placed at the beginning of the cycle, and polls do below:
`` `CPP
VOID loop () {
  btn.tick ();// survey

  if (btn.click ()) serial.println ("click");// will display once when clicking
  if (btn.click ()) serial.println ("click");// The same click!
}
`` `
> Unlike previous versions of the library, the survey functions are not reset inside themselves, but *inside the processing function *.Thus, in the example above, when clicking on the button in the port, the message `click ()` is displayed twice.This allows you to use the survey functions several times for the current iteration of the cycle to create a complex logic of the program.

#### several functions of processing
For obvious reasons, it is impossible to cause the processing function more than once per cycle - each next call will drop events from the previous one and the code will work incorrectly.So - you can’t:
`` `CPP
// you can not do it this way
VOID loop () {
  btn.tick ();
  if (btn.click ()) ...

  // ....

  btn.tick ();
  if (btn.hold ()) ...
}
`` `

If you really need to get into a deaf cycle and interrogate the button there, then it can: you can:
`` `CPP
// so it is possible
VOID loop () {
  btn.tick ();
  if (btn.click ()) ...

  While (True) {
    btn.tick ();
    if (btn.hold ()) ...
    if (btn.click ()) Break;
  }
}
`` `

If the library is used with an connected event handler `Attach ()` (see below), then you can call `tick ()` anywhere and as many times as you like, the events will be processed in the handler:
`` `CPP
// so it is possible
VOID CB () {
  switch (btn.action ()) {
    // ...
  }
}

VOID setup () {
  btn.attach (CB);
}

VOID loop () {
  btn.tick ();
  // ...
  btn.tick ();
  // ...
  btn.tick ();
}
`` `

### "loaded" program
The Encbutton - ** asynchronous ** library: it does not wait until the button is completed, and allows the program to be executed further.This means that for the correct operation of the library, the main cycle of the program should be performed as quickly as possible and not contain delays and other "deaf" cycles within itself.To ensure proper processing of the button, it is not recommended to have a main delay cycle lasting more than 50-100 ms.A few tips:
-beginners: to study the lesson cycle [how to write a sketch] (https://alexgyver.ru/lessns/how-to-sketch/)
  - write asynchronous code in `loop ()`
  - Any synchronous structure on `delay ()` can be made asynchronous using `millis ()` `
  - if in the program * each * Iteration gThe cranberries of the bay cycle are performed longer than 50-100ms-in most cases the program is written incorrectly, with the exception of some special cases
- connect the button to the hardware interruption (see below)
- avoid the execution of "heavy" sections of the code while the button is processing, for example, by placing them in the condition `If (! Button.busy ()) {heavy code}`} `
- If it is impossible to optimize the main cycle - call the ticker in another "stream" and use the processor:
  - in interruption of a timer with a period of ~ 50ms or more often
  - on another core (for example, ESP32)
  - In another Task Freertos
  - inside `yield ()` (inside `delay ()`)

#### Separate processing
> It makes sense only with a manual survey of events!With a connected processing function, it is enough to call the usual `tick ()` between the heavy sections of the program

Also, in a loaded program, you can divide the processing and reset of events: instead of `tick ()` use `tickRAW ()` between heavy sections of the code and manual reset `Clear ()`.The order is as follows:
- Surrender actions (Click, Press, Turn ...)
- Call `Clear ()`
- call `tickRaW ()` between heavy sections of code

`` `CPP
VOID loop () {
  if (btn.click ()) ...
  if (btn.press ()) ...
  if (btn.step ()) ...

  btn.clear ();

  // ...
  BTN.TickRAW ();
  // ...
  BTN.TickRAW ();
  // ...
  BTN.TickRAW ();
  // ...
}
`` `
This will allow to interview the button/encoder in a not very well written program, where the main cycle is littered with heavy code.Inside the `Tickrade ()` Events accumulate that are dismantled once in the cycle, and then manually reset.

> In this scenario, the Encoder's buffering in the interruption does not work and all events are not processed `Releasexxx`

#### Processing inside Delay
If it is difficult to get rid of the `delay ()` inside the main cycle of the program, then on some platforms you can place your code inside it.Thus, you can even get encoder processing in a cycle with deals without using interruptions:
`` `CPP
// Code insertion in Delay
VOID yield () {
  EB.TickRAW ();
}

VOID loop () {
  if.click ()) ...
  if (btn.turn ()) ...

  eb.clear ();

  // ...
  Delay (10);
  // ...
  DELAY (50);
  // ...
}
`` `

> In this scenario, the Encoder's buffering in the interruption does not work and all events are not processed `Releasexxx`

#### button processing
The library processes the button as follows:
- Pressing with software suppression of rubbish (holding longer than the Deb time), the result is the event `Press`, the state of` Pressing` and `Busy`
- retention longer than the Hold Hold time - the event `hold`, the state` holding`
- holding longer than the Hold + Timeshot Timeshu Taimout - a pulse event `step`, triggers with the STEP period while the button holds
- release of the button, the result - the event `Release`, the removal of the states` Pressing` and `Holding`
  - release to the deduction time - event `click`
  - release after holding - event `Releasehold`
  - release after impulse deduction - event `ReleaseStep`
  - Events `Releasehold` and` ReleaseStep` mutually exclusive, if the button was withheld `step` -` Releasehold` will no longer work
- Waiting for a new click during the Click timeout, the state `Waiting`
- If there is no new click - the removal of the state of `Busy`, the processing is completed
  - If the button is pressed again - processing a new click
  - The Clicks Clicks `getClicks ()` is discarded after the events `Releasehold`/` Releastep`, which check the preliminary clicks.In the general processor `Action ()` Events `EB_REL_Hold_C` or` EB_REL_STEP_C`
  - The number of clicks made must be checked by the `Hasclicks` event, and you can also interview almost all the events of the buttons that go to` Releasexxx`
- If `Timeout` is expected - Timeout event with the specified period from the current moment
- processing the button in the interruption informs the library about the fact of pressing, the rest of the processing is performed regularly in `Tick ()` `

> The difference is `Click (n)` `Hasclicks (n)`: `Click (n)` will return `true` in any case when the number of clicks coincides, even if more clicks are made.`HasClicks (n)` will return `true` only inCranberry, if the exactly indicated number of clicks was made and there were no more clicks!

> It is better to see once than read a hundred times.Launch an example of Demo and go on the button, or try [online symulation in Wokwi] (https://wokwi.com/projects/373591584298469377)

##### Click
! [click] (/doc/click.gif)

##### Hold
! [Hold] (/doc/hold.gif)

##### STEP
! [STEP] (/DOC/STEP.GIF)

Online symulation is available [here] (https://wokwi.com/projects/373591584298469377)

#### Encoder Processing
- "Fast" turn is considered a turn committed less than tuned timaut from the previous turn
- the turns processed in interruption become active (cause events) after calling `tick ()`
- Access to the encoder’s counter `Counter` is a public variable of the class, you can do anything with it:
`` `CPP
Serial.println (eb.counter);// read
Eb.counter += 1234;// change
eb.counter = 0;// Knock
`` `

#### encoder processing with a button
- The turning of the encoder with a clamped button removes and blocks all subsequent events and clicks, with the exception of the event `redase`.The states of the pressed button do not change
- The turning of the encoder also affects the system timout (the `timeout ()` function) will work after the indicated time after turning the enkoder
- the Klikov counter is available when pressed: several clicks, click of a button, turn

<a id="preclicks"> </a>

### Preliminary clicks
The library considers the number of clicks by the button and some survey functions can separately be processed with *preliminary clicks *.For example, 3 clicks, then retention.This greatly expands the capabilities of one button.There are two options for working with such events:
`` `CPP
  // 1
  if (btn.hold ()) {
    if (btn.getclics () == 2) serial.println ("Hold 2 Clicks");
  }

  // 2
  if (btn.hold (2)) serial.println ("Hold 2 Clicks");
`` `

In the first version, you can get the number of clicks for further processing manually, and in the second - the library will do this itself if the number of clicks for action is known in advance.

<a id="btnread"> </a>

## direct reading button
In some scenarios, you need to get the state of the "here and now" button, for example, determine whether the button is held immediately after starting the microcontroller (program start).The function `tick ()` must be called constantly in the cycle so that the button is processing with the extinguishing of the ratio of contacts and other calculations, so the design of the next type ** will not work **:
`` `CPP
VOID setup () {
  btn.tick ();
  if (btn.press ()) serial.println ("button pressed at start");
}
`` `

The following functions will help for such scenarios, return `true` if the button is pressed:
- `read ()` for libraries button and buttont
- `Readbtn ()` for library libraries and encbuttont

> The button survey is performed taking into account the previously configured button level (Setbtnlevel)!It is not necessary to manually invert the logic:

`` `CPP
VOID setup () {
  // btn.setbtnlevel (LOW);// you can configure the level

  if (btn.read ()) serial.println ("button pressed at start");
}
`` `

<a id="loop"> </a>

### immersion in the cycle
Suppose you need to process the button synchronously and with the extinguishing of the rattles.For example, if the button is clamped at the start of the microcontroller, get its retention or even the pulse retention inside the `setup` unit, that is, before the start of the main program.You can use the state of `Busy` and interview the button from the cycle:
`` `CPP
VOID setup () {
  Serial.Begin (115200);

  btn.tick ();
  While (btn.busy ()) {
    btn.tick ();
    if (btn.hold ()) serial.println ("Hold");
    if (btn.step ()) serial.println ("step");
  }

  Serial.println ("Program Start");
}
`` `
How it works: the first tick interrogates the button, if the button is pressed - the state of the Busy is immediately activated and the system enters the `While` cycle.Inside it, we continue to tick and get events from the button.When the button is released and all events will work - the Busy flag will drop and the program will automatically leave the cycle.You can rewrite this design to the cycle with a postcryption, more beautiful:
`` `CPP
do {
  B.tn.tick ();
  if (btn.hold ()) serial.println ("Hold");
  if (btn.step ()) serial.println ("step");
} While (btn.busy ());
`` `

<a id="timeout"> </a>

## Timeout
In the classes associated with the button (Button, Encbutton) there is a function `Timeout (Time)` - it will once return `true` if the indicated time has passed after the action with the button/encoder.This can be used to preserve parameters after entering, for example:
`` `CPP
VOID loop () {
  eb.tick ();

  // ...

  if.timeout ()) {
    // after interaction with encoder 1 second passed
    // eeprom.put (0, settings);
  }
}
`` `

<a id="busy"> </a>

### Busy
The `Busy () function` Returns `True` while the button processing is underway, i.e.So far, the system awaits actions and the release of timeouts.This can be used to optimize the code, for example, avoid some long and heavy parts of the program during the button processing:
`` `CPP
VOID loop () {
  eb.tick ();

  // ...

  if (! eb.busy ()) {
    // Potentially long and difficult code
  }
}
`` `

<a id="Actions"> </a>

### Obtaining an event
Available in all classes ** with the ** button:
- `virtbutton`
- `Button`
- `virtencbutton`
- `encbutton`

The function `Action ()` when the event occurs, the event is returned (different from scratch, which in itself is an indication of the existence of an event):
- `eb_press` - click on the button
- `eb_hold` - the button is kept
- `eb_step` - impulse retention
- `eb_release` - the button is released
- `eb_click` - Single click
- `eb_clicks` - A few click signal
- `eb_turn` - turn of the encoder
- `eb_rel_hold` - the button is released after holding
- `EB_REL_HOLD_C` - the button is released after holding off the premises.clicks
- `EB_REL_STEP` - the button is released after the step
- `EB_REL_STEP_C` - the button is released after the step with a border.clicks

> The result of the function `Action ()` is reset after the next call `tick ()`, that is, is available on the entire current iteration of the main cycle

The obtained event code can be processed through `switch`:
`` `CPP
switch (eb.action ()) {
  Case eb_press:
    // ...
    Break;
  Case eb_hold:
    // ...
    Break;
  // ...
}
`` `

<a id="OPTIMISE"> </a>

## Optimization
#### Library weight
To maximally reduce the weight of the library (in particular in RAM), you need to set the Timatui constants through Define (saving 1 bytes per timaut), turn off the events processor, counters-buffers and use the T-class (saving 1 byte per pin):
`` `CPP
#define eb_no_for
#define eb_no_callback
#define eb_no_counter
#define eb_no_buffer
#define eb_deb_time 50 // Timesout to extinguish the trim button (button)
#define eb_click_time 500 // Click Stayout (button)
#define eb_hold_time 600 // Maintenance Times (button)
#define eb_step_time 200 // Impulse retention Timesout (button)
#define EB_FAST_TIME 30 // TIMAUT RAM (ENCODER)
#include <encbutton.h>
ENCBUTTONT <2, 3, 4> EB;
`` `
In this case, an encoder with a button will take only 8 bytes in SRAM, and just a button - 5.

#### Fulfillment speed
To reduce the time for checking the system flags of events (insignificantly, but pleasant), you can place all the polls in the condition by `tick ()`, since `tick ()` returns `true` only when ** events **: the events **:
`` `CPP
VOID loop () {
  if (eb.tick ()) {
    if.turn ()) ...;
    if.click ()) ...;
  }
}
`` `

Also, a survey of events using the `Action () function is performed faster than a manual survey of individual functions of events, so the library will work in this format as efficiently as possible:
`` `CPP
VOID loop () {
  if (eb.tick ()) {
    switch (eb.action ()) {
      Case eb_press:
        // ...
        Break;
      Case eb_hold:
        // ...
        Break;
      // ...
    }
  }
}
`` `

For polling ** states ** buttons `Pressing ()`, `Holding ()`, `WATING ()` You can place them inside the conditions of `BUSY ()` so as not to interview them until they are guaranteed:
`` `CPP
if (btn.busy ()) {
  if (btn.pressing ()) ...
  if (btn.holding ()) ...
  if (btn.waiting ()) ...
}
`` `

<a id="callback"> </a>

### Collback
You can connect the external function-shacklerCranberry, it will be caused when any event occurs.This opportunity works in all classes ** with the ** button:
- `virtbutton`
- `Button`
- `virtencbutton`
- `encbutton`

`` `CPP
ENCBUTTON EB (2, 3, 4);

Void callback () {
  switch (eb.action ()) {
    Case eb_press:
      // ...
      Break;
    Case eb_hold:
      // ...
      Break;
    // ...
  }
}

VOID setup () {
  eb.attach (callback);
}

VOID loop () {
  eb.tick ();
}
`` `

<a id="duble"> </a>

### Simultaneous pressing
The library supports work with two simultaneously pressed buttons as with the third button.For this you need:
1. To make a virtual button `virtbutton`
2. Call the processing of real buttons
3. Pass these buttons to the virtual button to process (these can be objects of classes `virtbutton`,` button`, `encbutton` + their` t`- version)
4. Next to interrogate events

`` `CPP
Button b0 (4);
Button b1 (5);
Virtbutton B2;// 1

VOID loop () {
  b0.tick ();// 2
  b1.tick ();// 2
  B2.Tick (B0, B1);// 3

  // 4
  if (b0.click ()) serial.println ("b0 click");
  if (b1.click ()) serial.println ("b1 click");
  if (b2.click ()) serial.println ("b0+b1 click");
}
`` `

The library itself will “drop” unnecessary events from real buttons if they were pressed together, with the exception of the event `Press`.Thus, a full -fledged third button of two others with a convenient survey is obtained.

<a id="isr"> </a>

## interrupt
### encoder
For processing an encoder in a loaded program you need:
- Connect both of his pins to hardware interruptions by `Change`
- install `setencisr (true)`
- call a special ticker for interruption in the handler
- The main ticker also needs to be called in `loop` for corrething work - events are generated in the main ticker:
`` `CPP
// Example for Atmega328 and Encbutton
ENCBUTTON EB (2, 3, 4);

/*
// ESP8266/ESP32
IRAM_ATTR VOID ISR () {
  eb.tickisr ();
}
*/

VOID isr () {
  eb.tickisr ();
}
VOID setup () {
  Attachinterrupt (0, Isr, Change);
  Attachinterrupt (1, ISR, Change);
  eb.setencisr (true);
}
VOID loop () {
  eb.tick ();
}
`` `

Note: The use of work in the interruption allows you to correctly process the encoder position and not miss a new turn.An event with a turn obtained from an interruption will become available * after * call `Tick` in the main cycle of the program, which allows not to violate the sequence of the main cycle:
- Buferization is disabled: the `turn` event is activated only once, regardless of the number of clicks of the encoder made between the two challenges of` tick` (clicks are processed in interruption)
- The buffering is included: the `turn` event will be caused as many times as there were really clicks of the encoder, this allows you to not miss the turns and not load the system in the interruption.** Boofer size - 5 unprocessed clicks of encoder **

Notes:
- The `setencisr` function works only in non - virtual classes.If it is turned on, the main ticker `tick` simply does not interview Encoder's pins, which saves processor time.Processing occurs only in interruption
- The encoder counter is always relevant and can be ahead of buffering turns in the program with large delays in the main cycle!
- on different interrupt platforms, they can work differently (for example, on ESPXX - you need to add the functions of the atrica `` IRAM_ATTR`, see documentation on your platform!)
- a processor connected to `Attach ()` will be called from `Tick ()`, that is, *not from interruption *!

### virtual classes
In the virtual ones there is a ticker in which it is not necessary to transmit the state of the encoder, if it is processed in an interruption, this allows you to not interview pins in idle.For example:

`` `CPP
Virtencoder e;

VOID isr () {
    E.tickisr (DigitalRead (2), DigitalRead (3));
}
VOID setup () {
    Attachinterrupt (0, Isr, Change);
    Attachinterrupt (1, ISR, Change);

    E.Setencisr (1);
}
VOID loop () {
    E.tick ();// Do not transmit the states of Pinov
}
`` `

#### Button
To process the button in the interruption, you need:
- Connect an interruption on ** press ** buttons taking into account its physical connection and level:
  - If the button is deputy`Low` - Interruption` Falling`
  - if the button closes `high` - interruption` rising`
- call `Pressisr ()` in the interruption processor

`` `CPP
Button b (2);

/*
// ESP8266/ESP32
IRAM_ATTR VOID ISR () {
  B.Pressisr ();
}
*/

VOID isr () {
  B.Pressisr ();
}
VOID setup () {
  Attachinterrupt (0, ISR, Falling);
}
VOID loop () {
  B.tick ();
}
`` `

Note: the button is processed mainly `tick ()`, and the function `Pressisr ()` just informs the library that the button was pressed outside `Tick ()`.This allows you not to miss the pressing of the button until the program was busy with something else.

<a id="array"> </a>

### Array of buttons/Encoder
You can create an array only from non -step classes (without the letter `t`), because Pinov numbers will have to be indicated already in the radio further in the program.For example:
`` `CPP
Button btns [5];
ENCBUTTON EBS [3];

VOID setup () {
  btns [0] .init (2);// Indicate the pin
  btns [1] .init (5);
  btns [2] .init (10);
  // ...

  EBS [0] .init (11, 12, 13, Input);
  EBS [1] .init (14, 15, 16);
  // ...
}
VOID loop () {
  for (int i = 0; i <5; i ++) btns [i] .Tick ();
  for (int i = 0; i <3; i ++) EBS [i] .Tick ();

  if (btns [2] .Click ()) serial.println ("BTN2 click");
  // ...
}
`` `

<a id="cubom"> </a>

### Caste functions
The library supports the task of its functions for reading PIN and getting time without editing library files.To do this, you need to implement the corresponding function in your .cpp or.
- `bool eb_read (uint8_t pin)` - for its pine reading function
- `void eb_mode (uint8_t pin, uint8_t mode)` - for your analogue Pinmode
- `uint32_t eb_uptime ()` - for your analogue millis ()

Example:

`` `CPP
#include <encbutton.h>

Bool eb_read (uint8_t pin) {
    Return DigitalRead (PIN);
}

VOID eb_mode (uint8_t pin, uint8_t mode) {
    Pinmode (PIN, Mode);
}

uint32_t eb_uptime () {
    Return Millis ();
}
`` `

<a id="timer"> </a>

### Survey by timer
Sometimes it may be necessary to call `tick ()` not on every iteration, but by the timer.For example, for a virtual button from the Pino Expand, when reading the Pino Expand is a long operation, and it often does not make sense to call it.You can’t do this, events will be active during the timer!
`` `CPP
VOID loop () {
  // Timer for 50 ms
  Static uint32_t tmr;
  if (millis () - tmr> = 50) {
    TMR = Millis ();
    btn.tick (Readsomepin ());
  }

  // will be actively within 50 ms !!!
  if (btn.click ()) foo ();
}
`` `

In this situation, you need to do this: tick along the timer, process events there and drop flags at the end:
`` `CPP
VOID loop () {
  // Timer for 50 ms
  Static uint32_t tmr;
  if (millis () - tmr> = 50) {
    TMR = Millis ();
    // TIK
    btn.tick (Readsomepin ());

    // analysis of events
    if (btn.click ()) foo ();

    // Reset of the flags
    btn.clear ();
  }
}
`` `

Or you can connect the handler and call `clear ()` at the end of the function:
`` `CPP
Void callback () {
  switch (btn.action ()) {
    // ...
  }

  // Reset of the flags
  btn.clear ();
}

VOID loop () {
  // Timer for 50 ms
  Static uint32_t tmr;
  if (millis () - tmr> = 50) {
    TMR = Millis ();
    btn.tick (Readsomepin ());
  }
}
`` `

In the case of calling the timer, the anti -departments will be partially provided by the timer itself and in the library it can be turned off (set the period 0).

For the correct operation of timeouts, conditions and a click counter, you need another approach: buffering the states read according to the timer and transfer them to the TIC in the main cycle.For example:
`` `CPP
Bool Readbuf = 0;// buffer Pina

VOID loop () {
  // Timer for 50 ms
  Static uint32_t tmr;
  if (millis () - tmr> = 50) {
    TMR = Millis ();
    Readbuf = Readsomepin ();// Reading in the buffer
  }

  // tick from the buffer
  BTN.Tick (Readbuf);

  if (btn.click ()) foo ();
}
`` `

<a id="EXAMPles-mini"> </a>

### Mini examples, scripts
`` `CPP
// Change the values of the variables

// Turn of the encoder
if (enc.turn ()) {
  // Change in step 5
  var += 5 * enc.dir ();

  // Change in step 1 with a normal turn, 10 with fast
  Var += ENC.FAST ()?10: 1;

  // Change in step 1 with a normal turn, 10 with pressed
  vAR += ENC.Pressing ()?10: 1;

  // Change one variable when turning, the other - with a pressed turn
  if (enc.pressing ()) Var0 ++;
  Else Var1 ++;

  // If the button is pressed - preliminary clicks are available
  // Choose a variable for changes in the premises.Clicks
  if (enc.pressing ()) {
    Switch (enc.getClicks ()) {
      CASE 1: VAR0 += ENC.DIR ();
        Break;
      CASE 2: VAR1 += ENC.DIR ();
        Break;
      CASE 3: VAR2 += ENC.DIR ();
        Break;
    }
  }
}

// Impulse retention at every step is increasing the variable
if (btn.step ()) var ++;

// Change the direction of change in the variable after letting go from STEP
if (btn.step ()) var += dir;
if (btn.releastep ()) die = -dir;

// Change the selected variable using STEP
if (btn.step (1)) Var1 ++;// Click-holding
if (btn.step (2)) Var2 ++;// Click-Click-holding
if (btn.step (3)) var3 ++;// Click-Click-Click-hold

// if you keep the STEP for more than 2 seconds - an incremental +5, so far less - +1
if (btn.step ()) {
  if (btn.stepfor (2000)) var += 5;
  Else Var += 1;
}

// inclusion of the mode by the number of clicks
if (btn.hasclicks ()) mode = btn.getclicks ();

// inclusion of the mode by several clicks and retention
if (btn.hold (1)) mode = 1;// Click-holding
if (btn.hold (2)) mode = 2;// Click-Click-holding
if (btn.hold (3)) mode = 3;// Click-Click-Click-hold

// or so
if (btn.hold ()) mode = btn.getclicks ();

// Button is released, look how much it was held
if (btn.release ()) {
  // from 1 to 2 seconds
  if (btn.pressfor ()> 1000 && btn.pressfor () <= 2000) mod = 1;
  // from 2 to 3 seconds
  Else if (BTN.PressFor ()> 2000 && BTN.PressFor () <= 3000) Mode = 2;
}
`` `

<a id="migrate"> </a>

## guide for migration from v2 to v3
## H initialization
`` `CPP
// virtual
Virtencbutton eb;// Encoder with button
Virtbutton b;// button
Virtencoder e;// Encoder

// Real
// Encoder with button
ENCBUTTON EB (ENC0, ENC1, BTN);// Encoder Pins and buttons
ENCBUTTON EB (ENC0, ENC1, BTN, MODEENC);// + Pino Pino Encoder Pin (silence. Input)
ENCBUTTON EB (ENC0, ENC1, BTN, MODEENC, MODEBTN);// + Pin mode buttons (silent. Input_pullup)
ENCBUTTON EB (ENC0, ENC1, BTN, MODEENC, MODEBTN, BTNlevel);// + button level (silence)
// template
ENCBUTTON <ENC0, ENC1, BTN> EB;// Encoder Pins and buttons
ENCBUTTON <ENC0, ENC1, BTN> EB (Modeenc);// + Pino Pino Encoder Pin (silence. Input)
ENCBUTTON <ENC0, ENC1, BTN> EB (Modeenc, Modebtn);// + Pin mode buttons (silent. Input_pullup)
ENCBUTTON <ENC0, ENC1, BTN> EB (Modeenc, Modebtn, Btnlevel);// + button level (silence)

// button
Button b (pin);// PIN
Button b (PIN, Mode);// + Pin mode buttons (silent. Input_pullup)
Button B (PIN, Mode, Btnlevel);// + button level (silence)
// template
Buttont <pin> b;// PIN
Buttont <pin> b (mode);// + Pin mode buttons (silent. Input_pullup)
Buttont <pin> b (mode, btnlevel);// + button level (silence)

// Encoder
ENCODER E (ENC0, ENC1);// Pines of Encoder
ENCODER E (ENC0, ENC1, Mode);// + Pino Pino Encoder Pin (silence. Input)
// template
Encodert <enc0, enc1> e;// Pines of Encoder
Encodert <Enc0, Enc1> E (Mode);// + Pino Pino Encoder Pin (silence. Input)
`` `

### functions
|v2 |v3 |
| ------------- | -------------------------------------
|`HELD ()` |`Hold ()` |
|`Hold ()` |`Holding ()` |
|`state ()` |`Pressing ()` |
|`setpins ()` |`Init ()` |

- The procedure for indicating Pinov has changed (see DEMPLE above)
- `Clearflags ()` replaced by `Clear ()` (drop the flags of events) and `reset ()` (drop systemic flags of processing, finish processing)

### Logic of Work
In the V3, the functions of an event survey (Click, Turn ...) are not discarded immediately after their call - they are discarded at the next call `Tick ()`, thus retain their meaning in all subsequent challenges on the current iteration of the main cycle of the program.** Therefore, `tick ()` needs to be called only 1 time per cycle, otherwise there will be missions of actions! ** Read about thisabove.

<a id="EXAMPLE"> </a>
## Examples
The rest of the examples look at ** Examples **!
<details>
<summary> Full demo encbutton </summary>

`` `CPP
// #define eb_no_for // Disable Pressfor/Holdfor/StepFor support and Stepov counter (saves 2 bytes of RAM)
// #define eb_no_callback // Disable the event processor Attach (saves 2 bytes of RAM)
// #define eb_no_counter // Disable the enkoder counter (saves 4 bytes of RAM)
// #define EB_NO_BUFFER // Disable the buffer of the encoder (saves 1 byte of RAM)

// #define eb_deb_time 50 // Timesout to darebells button (button)
// #define eb_click_time 500 // Clicks standstatics (button)
// #define eb_hold_time 600 // Maintenance Times (button)
// #define eb_step_time 200 // pulse retention rate (button)
// #define EB_FAST_TIME 30 // Quick turn Timesout (Encoder)

#include <encbutton.h>
ENCBUTTON EB (2, 3, 4);
// ENCBUTTON EB (2, 3, 4, Input);// + Pino Pino mode
// ENCBUTTON EB (2, 3, 4, Input, Input_pullup);// + button pins mode
// ENCBUTTON EB (2, 3, 4, Input, Input_pullup, Low);// + button level

VOID setup () {
    Serial.Begin (115200);

    // shows the default values
    eb.setbtnlevel (Low);
    EB.SetClicktimeout (500);
    eb.Setdebtimeout (50);
    Eb.SetHoldtimeout (600);
    eb.setsteptimeout (200);

    eb.setencreverse (0);
    eb.setenctype (eb_step4_low);
    eb.setfasttimeout (30);

    // throw the Encoder counter
    eb.counter = 0;
}

VOID loop () {
    eb.tick ();

    // General rotation processing
    if.turn ()) {
        Serial.print ("Turn: Dir");
        Serial.print (eb.dir ());
        Serial.print (", fast");
        Serial.print (eb.fast ());
        Serial.print (", Hold");
        Serial.print (eb.pressing ());
        Serial.print (", Counter");
        Serial.print (eb.counter);
        Serial.print (", clicks");
        Serial.println (eb.getClicks ());
    }

    // Turning rotation processing
    if.left ()) serial.println ("Left");
    if.right ()) serial.println ("right");
    if.left ()) serial.println ("Lefth");
    if.righth ()) serial.println ("righth");

    // button
    if.press ()) serial.println ("Press");
    if.click ()) serial.println ("click");

    if.release ()) {
      Serial.println ("Release");

      Serial.print ("Clicks:");
      Serial.print (eb.getClicks ());
      Serial.print (", stps:");
      Serial.print (eb.getsteps ());
      Serial.print (", Press for:");
      Serial.print (eb.pressfor ());
      Serial.print (", Hold for:");
      Serial.print (eb.holdfor ());
      Serial.print (", step for:");
      Serial.println (eb.stepfor ());
    }

    // States
    // serial.println (eb.pressing ());
    // serial.println (eb.holding ());
    // serial.println (eb.busy ());
    // serial.println (eb.waiting ());

    // Timesout
    if (eb.timeout ()) serial.println ("Timeout!");

    // Holding
    if.hold ()) serial.println ("Hold");
    if.hold (3)) serial.println ("Hold 3");

    // Impulse retention
    if.step ()) serial.println ("step");
    if.step (3)) serial.println ("STEP 3");

    // released after impulse deduction
    if (eb.releastep ()) serial.println ("Release Step");
    if (eb.releastep (3)) serial.println ("Release Step 3");

    // released after holding
    if.releasehold ()) serial.println ("Release Hold");
    if (eb.releasehold (2)) serial.println ("Release Hold 2");

    // Check for the number of clicks
    if.hasclicks (3)) Serial.println ("Has 3 Clicks");

    // Bring the number of clicks
    if.hasclicks ()) {
        Serial.print ("Has Clicks:");
        Serial.println (eb.getClicks ());
    }
}
`` `
</details>
<details>
<summary> connection of the handler </summary>

`` `CPP
#include <encbutton.h>
ENCBUTTON EB (2, 3, 4);

Void callback () {
    Serial.print ("callback:");
    switch (eb.action ()) {
        Case eb_press:
            Serial.println ("Press");
            Break;
        Case eb_hold:serial.println ("Hold");
            Break;
        Case eb_step:
            Serial.println ("STEP");
            Break;
        Case eb_release:
            Serial.println ("Release");
            Break;
        Case eb_click:
            Serial.println ("click");
            Break;
        Case eb_clicks:
            Serial.print ("Clicks");
            Serial.println (eb.getClicks ());
            Break;
        Case eb_turn:
            Serial.print ("turn");
            Serial.print (eb.dir ());
            Serial.print ("");
            Serial.print (eb.fast ());
            Serial.print ("");
            Serial.println (eb.pressing ());
            Break;
        Case eb_rel_hold:
            Serial.println ("Release Hold");
            Break;
        CASE EB_REL_HOLD_C:
            Serial.print ("Release Hold Clicks");
            Serial.println (eb.getClicks ());
            Break;
        CASE EB_REL_STEP:
            Serial.println ("Release Step");
            Break;
        CASE EB_REL_STEP_C:
            Serial.print ("Release Step Clicks");
            Serial.println (eb.getClicks ());
            Break;
    }
}

VOID setup () {
    Serial.Begin (115200);
    eb.attach (callback);
}

VOID loop () {
    eb.tick ();
}
`` `
</details>
<details>
<summary> All types of buttons </summary>

`` `CPP
#include <encbutton.h>

Button BTN (4);
Buttont <5> btnt;
Virtbutton BTNV;

VOID setup () {
    Serial.Begin (115200);
}

VOID loop () {
    // Button
    btn.tick ();
    if (btn.click ()) serial.println ("btn click");

    // Buttont
    btnt.tick ();
    if (btnt.click ()) serial.println ("BTNT CLICK");

    // virtbutton
    btnv.tick (! DigitalRead (4));// transmit logical value
    if (btnv.click ()) serial.println ("btnv click");
}
`` `
</details>
<details>
<summary> All types of encoder </summary>

`` `CPP
#include <encbutton.h>

ENCODER ENC (2, 3);
ENCODERT <5, 6> ENCT;
Virtencoder encv;

VOID setup () {
    Serial.Begin (115200);
}

VOID loop () {
    // The survey is the same for everyone, 3 ways:

    // 1
    // Tick will return 1 or -1, then this is a step
    if (enc.tick ()) serial.println (enc.counter);

    // 2
    // can be interviewed through turn ()
    enct.tick ();
    if (enct.turn ()) serial.println (enct.dir ());

    // 3
    // you can not use survey functions, but get the direction directly
    int8_t v = encv.tick (DigitalRead (2), DigitalRead (3));
    if (v) serial.println (v);// Derive 1 or -1
}
`` `
</details>

<a id="versions"> </a>
## versions
<details>
<summary> Old </ Summary>

- V1.1 - Pullap separately by the method
- V1.2 - You can transfer the parameter input_pullup / input (silent) to the designer
- V1.3 - Virtual clamping of the encoder button is made into a separate function + minor improvements
- V1.4 - Processing of pressing and releasing the button
- v1.5 - added virtual mode
- V1.6 - Optimization of work in interruption
- V1.6.1 - Saching by default Input_pullup
- V1.7 - a large memory optimization, remade Fastio
- V1.8 - Individual tuning of the TIMUUT Maintenance of the button (was common at all)
- v1.8.1 - removed Fastio
- v1.9 - added a separate development of a pressed turn and a request for direction
- V1.10 - improved ReleASDE processing, eased the weight in callback and corrected the bugs
- V1.11 - even more than any optimization + setting button level
- V1.11.1 - Digispark compatibility
- V1.12 - added a more accurate algorithm of enkoder Eb_better_enc
- V1.13 - Added experimental ENCBUTTON2
- V1.14 - added ReleaseStep ().The release of the button is included in the debate
- v1.15 - added Setpins () for Encbutton2
- V1.16 - added EB_HALFSTEP_Enc mode for hemisphere encoders
- v1.17 - added STEP with preliminary clicks
- V1.18 - We do not consider clicks after the activation of STEP.Hold () and Held () can also take preliminary clicks.Redistributed and improved debate
- V1.18.1 - Fixed error in ReleaseStep () (did not return the result)
- V1.18.2 - Fix Compiler Warnings
- V1.19 - speed optimization, reduced weight in SRAM
- v1.19.1 - still a bit increased performance
- v1.19.2 - not yetCranberries increased a lot of performance, thanks xray3d
- v1.19.3 - made a high level of the default button in virtual mode
- V1.19.4 - Fix Encbutton2
- V1.20 - Critical error is fixed in Encbutton2
- V1.21 - EB_HALFSTEP_ENC now works for a normal mode
- V1.22 - Improved EB_HALFSTEP_Enc for a normal mode
- V1.23 - Getdir () replaced with DIR ()
- V2.0
    - The eb_better_enc algorithm is optimized and set by default, the define eb_better_enc is abolished
    - added setenctype () to configure the type of encoder from the program, the define EB_HALFSTEP_ENC is abolished
    - added Setencreverse () to change the direction of the encoder from the program
    - added setteptimeout () to set the period of impulse deduction, the define EB_STEP is abolished
    - Small improvements and optimization
</details>

- V3.0
  - The library is rewritten from scratch, with previous versions is incompatible!
    - completely different initialization of the object
    -renamed: Hold ()-> Holding (), HELD ()-> HOLD ()
  - Optimization of Flash memory: the library weighs less, in some scenarios - by several kilobytes
  - optimization of the speed of code execution, including in interruption
  - several bytes less than RAM, several optimization levels to choose from
  - a simpler, understandable and convenient use
  - more readable source code
  - Breaking into classes for use in different scenarios
  - new functions, capabilities and handlers for the button and encoder
  - Encoder's buffer in interruption
  - native processing of two simultaneously pressed buttons as a third button
  - support of 4 types of encoder
  - The documentation is rewritten
  - Encbutton is now replacing Gyverlibs/Virtualbutton (archived)
- V3.1
  - The initialization of the button is expanded
  - removed Holdencbutton () and Toggleencbutton ()
  - added Turnh ()
  - Optimized the interruptions of encoder, added Setencisr ()
  - Buerization of the direction and quick turn
  - strongly optimized the speed of Action () (general processor)
  - Added connection of the external function-processor of events
  - Added button processing in interruption - Pressisr ()
- V3.2
  - Added the functions of TickRaW () and Clear () for all classes.Allows for separate processing (see document)
  - improved processing button using interruptions
- V3.3
  - Added functions of receiving PressFor (), HoldFor (), StepFor () (disconnected)
  - Added meter of the steps Getsteps () (disconnected)
- V3.4
  - access to the click counter during a pressed turn
  - Added function Detach ()
- V3.5
  - added dependence of Gyverio (accelerated Pino survey)
  - added the opportunity to set your pharmacy and pine reading functions
- V3.5.2
  - Optimization
  - Simplified replacement of custom functions
  - Fixed a compilation error when using a library in several .cpp files
- V3.5.3
  - Added the number of clicks to the Press/Release/Click/Pressing poll
- V3.5.5 - Collback based on the STD :: Function for ESP
    
<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code