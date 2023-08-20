This is an automatic translation, may be incorrect in some places. See sources and examples!

# ENCBUTTON

| ⚠️⚠️⚠️ <BR> ** The new version of V3 is incompatible with the previous ones!Read the documentation below! ** <br> ⚠️⚠️⚠️ |
|--- |

A light and very functional library for an encoder with a button, encoder or buttons with Arduino
- Button
    - Processing of events: pressing, releasing, click, click counter, retention, impulse retention + preliminary clicks for all of the listed modes
    - Program suppression of rubbish
    - support for processing two simultaneously pressed buttons as a third button
- Encoder
    - Processing of events: a normal turn, pressed turn, fast turn
    - Support of four types of incidental encoders
    - high -precision algorithm for determining the position
    - buffer in interruption
- simplicity and versatility of the API for different use scenarios
- virtual regime (for example, for working with a pain expander)
- optimized for work in interruption (encoder)
- The fastest reading of pins for AVR (Atmega328/Atmega168, Attiny85/Attiny13)
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
- [Examples] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** encbutton ** and installed through the library manager in:
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
- I recommend always updating the library: errors and bugs are corrected in the new versions, and soCranberries are optimized and new features are added
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

### Button
### The level of the button
The button can be connected to the microcontroller in two ways and give a high or low signal when pressed.The library provides for setting up `setbuttonlevel (level)`, where the level is an active button signal:
- `High` - the button connects the VCC.Installed by default in `virt`-bibliotexes
- `Low` - the button connects GND.Set by default in the main libraries
! [Scheme] (/doc/btn_scheme.png)

#### Pin
In diagrams with microcontrollers, the connection of the GND button with a PIN suspension to VCC is most often used.The tightening can be external (Pin mode must be put `Input`) or internal (PIN mode` Input_pullup`).In "real" projects, an external lifelong is recommended, becauseIt is less susceptible to interference - the internal has too high resistance.

### Library structure
Encbutton starting with version 3.0 is several libraries (classes) for various use scenarios:
- Basic classes:
  - `virtbutton` - the base class of the virtual button, provides all the possibilities of the buttons
  - `virtencoder` - the base class of the virtual encoder, determines the fact and direction of the rotation of the enkoder
  - `virtencbutton` - the base class of a virtual encoder with a button, provides an encoder poll taking into account the button, *inherits Virtbututton and Virtencoder *
- Main classes:
  - `Button` - button class, *inherits virtbutton *
  - `Encoder` - Encoder class, *inherits virtencoder *
  - `encbutton` - an encoder class with a button, *inherits virtencbutton *
  - versions of `buttont`,` encodert`, `encbuttont` store Pino numbers in the template to maximize memory and speed optimization

> * Virtual * - without specifying a PIN of the microcontroller, works directly with the transmitted value, for example, for a survey of the enemies buttons through pain extensors and shift registers.

<a id="docs"> </a>

## Documentation
### Defaine settings
Be up to the library

`` `CPP
// Disable the encoder counter [Virtencoder, Encoder, Encbutton] (saves 4 bytes of RAM)
#define eb_no_counter

// Disconnect the buffer of the encoder (saves 1 byte of RAM)
#define eb_no_buffer

/*
  Setting up timeouts for all classes
  - replaces the timauts with constants, changing them from the program (setxxxtimeout ()) will not be
  - Setting affects all buttons announced in the program/Encoders
  - saves 1 bytes of RAM for an object for each timeout
  - shows the default values in MS
  - values are not limited to 4000MS, as when installing from the program (SetXXXTimeout ())
*/
#define eb_deb_time 50 // Timesout to extinguish the trim button (button)
#define eb_click_time 500 // Click Stage ClickOV (button)
#define eb_hold_time 500 // Maintenance Times (button)
#define eb_step_time 200 // Impulse retention Timesout (button)
#define EB_FAST_TIME 30 // TIMAUT RAM (ENCODER)
`` `

### classes
> Note: `# Include <encbutton.h>` connects all the library tools!

<details>
<summary> virtbutton </summary>

`` `CPP
// ==ward
// Set the deduction timeout, silence.500 (max. 4000 ms)
VOID SetHoldtimeout (Uint16_T Tout);

// Install the timout of impulse retention, silence.200 (max. 4000 ms)
VOID Setsteptimeout (Uint16_T Tout);

// Install the expectations of clicks, silence.500 (max. 4000 ms)
VOID setClicktimeout (Uint16_T Tout);

// Install the Timout of the Anti -Direct, silence.50 (Max. 255 ms)
VOID Setdebtimeout (Uint8_t Tout);

// set the level of the button (HIGH - button closes VCC, Low - closes GND)
// silent.High, that is, True - the button is pressed
VOID Setbuttonlevel (Bool Level);

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

// ================== Poll ================================ward
// Pressure button [event]
Bool Press ();

// button released (in any case) [event]
Bool Release ();

// click on the button (released without holding) [event]
Bool click ();

// Squeezed button (between Press () and Release ()) [condition]
Bool Pressing ();

// The button was withheld (more timeout) [event]
Bool Hold ();

// The button was kept (more timeout) with preliminary clicks [Event]
Bool Hold (uint8_t num);

// The button is held (more timeout) [condition]
Bool Holding ();

// The button is held (more timeout) with preliminary clicks [condition]
Bool Holding (Uint8_T Num);

// Impulse retention [event]
Bool Step ();

// Impulse deduction with preliminary clicks [Event]
Bool Step (Uint8_t Num);

// Several clicks were recorded [event]
Bool HasClicks ();

// recorded the specified number of clicks [event]
Bool HasClicks (Uint8_T Num);

// get the number of clicks
uint8_t getclicks ();

// button released after holding [Event]
Bool ReleaseHold ();

// Button is released after holding with preliminary clicks [Event]
Bool Releasehold (Uint8_T Num);

// Button is released after impulse retention [Event]
Bool ReleaseStep ();

// button is released after impulse deduction with preliminary clicks [Event]
Bool ReleaseStep (Uint8_T Num);

// After interacting with the button (or enkoder Encbutton), the specified time has passed, ms [event]
Bool Timeout (Uint16_T Tout);

// button awaits repeated clicks (between Click () and HasClicks ()) [condition]
Bool Waiting ();

// processing (between the first press and after waiting for clicks) [condition]
Bool Busy ();

// there was an action with a button (or encbutton encoder), will return the event code [event]
Uint16_T Action ();

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

// Initialization of the encoder
VOID Initenc (Bool E0, Bool E1);

// Encoder initialization with a combined value
VOID Initenc (int8_t v);

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

// Introduce the Encoder.Will return 1 or -1 during rotation, 0 in OSTAnovka
Int8_T Tick (Bool E0, Bool E1);
int8_t tick (int8_t state);

// Introduce the encoder without installing flags on a turn (faster).Will return 1 or -1 during rotation, 0 when stopping
Int8_T TickRaW (Bool E0, Bool E1);
Int8_T TickRaW (int8_t state);
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

// virtually squeeze the encoder button
Void Holdencbutton (Bool State);

// virtually switch the encoder button
VOID TOGLEENCBUTTON ();

// =================== Poll =================================
// + all the functions of the survey from Virtbutton and Virtencoder

// Turn to the right [event]
Bool Right ();

// Turn to the left [event]
Bool Left ();

// pressed turn to the right [event]
Bool Righth ();

// pressed turn to the left [event]
Bool Lefth ();

// Pressing the Encoder button [condition]
Bool Encholding ();

// Fast turning of the encoder [condition]
Bool Fast ();

// ====ward
// processing in interruption (only encoder).Will return 0 at rest, 1 or -1 when turning
Int8_t Tickisr (Bool E0, Bool E1);

// processing in interruption (only encoder).Will return 0 at rest, 1 or -1 when turning
Int8_T Tickisr (int8_t e01);

// processing of encoder and buttons
Bool Tick (Bool E0, Bool E1, Bool BTN);

// processing of encoder and buttons
Bool Tick (Int8_t E01, Bool BTN);
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

// indicate the pin and its operating mode
VOID Init (uint8_t npin, uint8_t mode);

// + all the functions of the survey from Virtbutton
// Read the current button value (without debate)
Bool Read ();

// processing function, call in loop
Bool Tick ();
`` `
</details>
<details>
<summary> buttont </summary>

- Available functions from `virtbutton`
- default buttons mode - `Low`

`` `CPP
Buttont <uint8_t pin>;// indicating Pin
Buttont <uint8_t pin> (uint8_t mode);// + mode of operation (silence input_pullup)

// specify the operating mode
VOID Init (Uint8_t Mode);

// + all the functions of the survey from Virtbutton

// Read the current button value (without debate)
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

// Indicate pins and their operating mode
VOID Init (Uint8_t Enca, Uint8_t Encb, Uint8_t Mode);

// Function of processing for calling in an interruption of encoder
int8_t tickisr ();

// Function of processing for calling in LOOP
Bool Tick ();
`` `
</details>
<details>
<summary> encodert </summary>

- Available functions from `virtencoder`

`` `CPP
Encodert <uint8_t enca, uint8_t encb>;// indicating Pinov
Encodert <uint8_t enca, uint8_t encb> (Uint8_t Mode);// + mode of operation (silence. Input)

// specify the mode of operation of Pinov
VOID Init (Uint8_t Mode);

// Function of processing for calling in an interruption of encoder
int8_t tickisr ();

// Function of processing for calling in LOOP
Bool Tick ();
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

// Reference Pines (ENK, ENK, button, Pinmode ENK, Pinmode button)
ENCBUTTON (UINT8_T ENCA, UINT8_T ENCB, UINT8_T BTN, UINT8_T MODEENC = Input, Uint8_t Modebtn = Input_Pullup);

// Reference Pines (ENK, ENK, button, Pinmode ENK, Pinmode button)
VOID Init (Uint8_t Enca, Uint8_t Encb, Uint8_t BTN, UINT8_T MODEENC = Input, Uint8_t Modebtn = Input_Pullup);

// +all the functions of the survey from Virtbutton, Virtencbutton and Virtencoder

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
<details>
<summary> encbuttont </summary>

- Available functions from `virtbutton`
- Available functions from `virtencoder`
- Available functions from `virtencbutton`

`` `CPP
ENCBUTTONT <uint8_T ENCA, UINT8_T ENCB, UINT8_T BTN>;// indicating Pinov
Encbuttont <uint8_t enca, uint8_t encb, uint8_t btn> (Uint8_t Modeenc = Input, Uint8_t Modebtn = Input_Pullup);// + Pino operation mode

// Configure Pino operation mode
VOID Init (Uint8_t Modeenc = Input, Uint8_t Modebtn = Input_Pullup);

// + all the functions of the survey from Virtbutton, Virtencbutton and Virtencoder

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

### Processing and Poll
All libraries have a general ** function of processing ** (ticker `tick`), which receives the current signal from the button and encoder
- this function must be called in the main cycle of the program (for virtual - with the transmission of the meaning)
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
> Unlike previous versions of the library, the survey functions are not reset inside themselves, but *inside the processing function *.Thus, in the example above, when clicking on the button in the port, the message `click` is twice.This allows you to use the survey functions several times for the current iteration of the cycle to create a complex logic of the program.

### button processing
The library processes the button as follows:
- Pressing with software suppression of rubbish (holding longer than the Deb time), the result is the event `Press`, the state of` Pressing` and `Busy`
- retention longer than the Hold Hold time - the event `hold`, the state` holding`
- retention longer than the Hold + Timesat Thai Timeshot - a pulse event `step`, triggers with a STEP period
- release of the button, the result - the event `Release`, the removal of the states` Pressing` and `Holding`
  - release to the deduction time - event `click`
  - release after holding - event `Releasehold`
  - release after impulse deduction - event `ReleaseStep`
- Waiting for a new click during the Click timeout, the state `Waiting`
- If there is no new click - the removal of the state of `Busy`, the processing is completed

> It is better to see once than read a hundred times.Launch the example of Demo and put on the button

### click
! [click] (/doc/click.gif)

### hold
! [Hold] (/doc/hold.gif)

#### STEP
! [STEP] (/DOC/STEP.GIF)

### encoder processing
- "Fast" turn is considered a turn committed less than tuned timaut from the previous turn
- the turns processed in interruption become active after calling the processing function
- Buerization of Encoder turns in an interruption does not remember the direction of turning, but only his fact

### encoder processing with a button
- PainThe cranberries of the encoder mouth with a closed button removes and blocks all subsequent events and clicks, with the exception of the event `Release`.The states of the pressed button do not change
- The turning of the encoder also affects the system timout (the `timeout ()` function) will work after the indicated time after turning the enkoder

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

### Obtaining an event
Available in all classes ** with the ** button:
- `virtbutton`
- `Button`
- `virtencbutton`
- `encbutton`

The function `Action ()` when the event occurs, the event is returned (different from scratch, which in itself is an indication of the existence of an event):
`Eb_press` - click on the button
`Eb_hold` - the button is kept
`Eb_step` - impulse retention
`Eb_release` - the button is released
`Eb_click` - Single click
`Eb_clicks` - a few click signal
`EB_TURN` - turn of encoder
`Eb_rel_hold` - the button is released after holding
`EB_REL_HOLD_C` - the button is released after holding off the premises.clicks
`Eb_rel_step` - the button is released after the step
`EB_REL_STEP_C` - the button is released after a step with a premium.clicks

> The result of the `Action () function is reset after calling the processing function, that is, it is available on the entire cycle iteration.

## event programming
The library does not implement the connection of the event-processor of events, but lovers of collels can organize a program, for example, as follows for any of the classes, becauseThe processing function once returns `true` when an event occurs, and` Action` returns the event code:

`` `CPP
ENCBUTTON EB (2, 3, 4);

Void Callback (Encbutton & Eb) {
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

VOID loop () {
  if (eb.tick ()) callback (EB);
}
`` `

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

## interrupt
To process the encoder in the loaded program, both of it should be connected to its PIN on hardware interruptions by `Change` and call a special ticker for interruption in the processor.The main ticker also needs to be called in `loop` for corrething (events are generated in the main ticker):
`` `CPP
// Example for Atmega328 and Encbutton
ENCBUTTON EB (2, 3, 4);

VOID isr () {
  eb.tickisr ();
}
VOID setup () {
  Attachinterrupt (0, Isr, Change);
  Attachinterrupt (1, ISR, Change);
}
VOID loop () {
  eb.tick ();
}
`` `

Note: The use of work in the interruption allows you to correctly process the encoder position and not miss a new turn.The event with a turn obtained from an interruption will become available * after * call `tick` in the main cycle of the program:
- If the buffer is disabled: event`Turn` is activated only once, regardless of the number of clicks of encoder made between two challenges` Tick` (clicks processed in interruption)
- If the buffering is included: the `turn` event will be caused as many times as there were really clicks of the encoder, this allows you to not miss the turns and not load the system in the interruption.Buferization * does not remember the direction * rotation for the sake of the ease of the library, the direction inside the turning poll will always be the last direction of click!

> Note: the encoder counter is always relevant and can be ahead of buffering turns in the program with large delays in the main cycle!

> Note: on different interrupt platforms, they can work in different ways (for example, on ESPXX - you need to add the functions of the attribute `IRAM_ATTR`, see documentation on your platform!)

### Porting to other platforms
In Utils.h/Utils.cpp files, you can replace arduino-dependent functions with others

<a id="EXAMPLE"> </a>

## Examples
The rest of the examples look at ** Examples **!
<details>
<summary> Full demo encbutton </summary>

`` `CPP
// #define eb_no_counter // Disable the enkoder counter (saves 4 bytes of RAM)
// #define EB_NO_BUFFER // Disable the buffer of the encoder (saves 1 byte of RAM)

// #define eb_deb_time 50 // Timesout to darebells button (button)
// #define eb_click_time 500 // Clicks standstatics (button)
// #define eb_hold_time 500 // Maintenance Times (button)
// #define eb_step_time 200 // pulse retention rate (button)
// #define EB_FAST_TIME 30 // Quick turn Timesout (Encoder)

#include <encbutton.h>
ENCBUTTON EB (2, 3, 4);

VOID setup () {
    Serial.Begin (115200);

    // shows the default values
    eb.setbuttonlevel (Low);
    EB.SetClicktimeout (500);
    eb.Setdebtimeout (50);
    EB.SetHoldtimeout (500);
    eb.setsteptimeout (200);

    eb.setencreverse (0);
    eb.setenctype (eb_step4_low);
    eb.setfasttimeout (30);
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
        Serial.print (eb.encholding ());
        Serial.print (", Counter");
        Serial.println (eb.counter);
    }

    // Turning rotation processing
    if.left ()) serial.println ("Left");
    if.right ()) serial.println ("right");
    if.left ()) serial.println ("Lefth");
    if.righth ()) serial.println ("righth");

    // button
    if.press ()) serial.println ("Press");
    if (eb.release ()) serial.println ("Release");
    if.click ()) serial.println ("click");

    // States
    // serial.println (eb.pressing ());
    // serial.println (eb.holding ());
    // serial.println (eb.busy ());
    // serial.println (eb.waiting ());

    // Timesout
    if (eb.timeout (1000)) serial.println ("Timeout!");

    // Holding
    if.hold ()) serial.println ("Hold");
    // if.hold ()) {
    // serial.print ("Hold +");
    // serial.print (eb.getClicks ());
    // serial.println ("Clicks");
    //}

    if.hold (2)) serial.println ("Hold 2");
    if.hold (3)) serial.println ("Hold 3");

    // Impulse retention
    if.step ()) serial.println ("step");
    if (eb.step (2)) serial.println ("STEP 2");
    if.step (3)) serial.println ("STEP 3");

    // released after impulse deduction
    if (eb.releastep ()) serial.println ("Release Step");
    if (eb.releastep (2)) serial.println ("Release STEP 2");
    if (eb.releastep (3)) serial.println ("Release Step 3");

    // released after holding
    if.releasehold ()) serial.println ("Release Hold");
    if (eb.releasehold (2)) serial.println ("Release Hold 2");

    // Check for the number of clicks
    if.hasclicks (1)) serial.println ("Has 1 Clicks");
    if.hasclicks (3)) Serial.println ("Has 3 Clicks");

    // Bring the number of clicks
    if.hasclicks ()){
        Serial.print ("Has Clicks:");
        Serial.println (eb.clicks);
    }
}
`` `
</details>
<details>
<summary> Demo Encbutton, General Processor </ Summary>

`` `CPP
#include <encbutton.h>
ENCBUTTON EB (2, 3, 4);

VOID setup () {
    Serial.Begin (115200);
}

VOID CB () {
    Serial.print ("callback:");
    switch (eb.action ()) {
        Case eb_press:
            Serial.println ("Press");
            Break;
        Case eb_hold:
            Serial.println ("Hold");
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
            Serial.println (eb.encholding ());
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

VOID loop () {
    if (eb.tick ()) CB ();
}
`` `
</details>
<details>
<summary> All types of buttons </summary>

`` `CPP
#include <encbutton.h>

Button BTN (4);
Buttont <5> btnt;
Virtbutton BTNC;

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
    btnc.tick (! DigitalRead (4));// transmit logical value
    if (btn.click ()) serial.println ("btnc click");
}
`` `
</details>
<details>
<summary> All types of encoder </summary>

`` `CPP
#include <encbutton.h>

ENCODER ENC (2, 3);
ENCODERT <5, 6> ENCT;
Virtencoder ENCC;

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
    Int8_t V = ENCC.Tick (DigitalRead (2), DigitalRead (3));
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
- v1.17 - added from preliminaryclicks
- V1.18 - We do not consider clicks after the activation of STEP.Hold () and Held () can also take preliminary clicks.Redistributed and improved debate
- V1.18.1 - Fixed error in ReleaseStep () (did not return the result)
- V1.18.2 - Fix Compiler Warnings
- V1.19 - speed optimization, reduced weight in SRAM
- v1.19.1 - still a bit increased performance
- V1.19.2 - Productivity increased a little more, thanks XRY3D
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