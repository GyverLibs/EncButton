// пример работы в виртуальном режиме совместно с библиотекой SimpleKeypad
// https://github.com/maximebohrer/SimpleKeypad
// передаём EncButton сразу всю клавиатуру через массивы и циклы

#include <EncButton.h>
VirtButton btn[16];

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
  for (int i = 0; i < 16; i++) btn[i].setDebTimeout(0);
}

void loop() {
  for (int i = 0; i < 16; i++) btn[i].tick(0);
  
  // массово тикаем все кнопки, передавая сравнение с кодом кнопки в цикле
  // делаем это по таймеру, чтобы не опрашивать клавиатуру постоянно
  static uint32_t tmr;
  if (millis() - tmr >= 10) {
    tmr = millis();
    char key = pad.scan();
    char* keysPtr = (char*)keys;  // указатель для удобства опроса
    for (int i = 0; i < 16; i++) btn[i].tick(key == keysPtr[i]);
  }

  // забираем действия с кнопок
  if (btn[0].click()) Serial.println("click 0");
  if (btn[0].hold()) Serial.println("hold 0");

  if (btn[1].press()) Serial.println("press 1");
  if (btn[1].step()) Serial.println("step 1");
}
