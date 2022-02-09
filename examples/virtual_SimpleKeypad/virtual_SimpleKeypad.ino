// пример работы в виртуальном режиме совместно с библиотекой SimpleKeypad
// https://github.com/maximebohrer/SimpleKeypad

#include <EncButton.h>
EncButton<EB_TICK, VIRT_BTN> btn0;
EncButton<EB_TICK, VIRT_BTN> btn1;

// пины подключения (по порядку штекера)
byte colPins[] = {7, 6, 5, 4};
byte rowPins[] = {11, 10, 9, 8};

// массив имён кнопок
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

#include <SimpleKeypad.h>
SimpleKeypad pad((char*)keys, rowPins, colPins, 4, 4);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // тикаем все кнопки, передавая сравнение с кодом кнопки в цикле
  // делаем это по таймеру, чтобы не опрашивать клавиатуру постоянно
  static uint32_t tmr;
  if (millis() - tmr >= 10) {
    tmr = millis();
    char key = pad.scan();
    btn0.tick(key == '1');
    btn1.tick(key == '2');
  }

  // забираем действия с кнопок
  if (btn0.click()) Serial.println("click 0");
  if (btn0.held()) Serial.println("held 0");

  if (btn1.press()) Serial.println("press 1");
  if (btn1.step()) Serial.println("step 1");
}
