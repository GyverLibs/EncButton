// пример работы в виртуальном режиме совместно с библиотекой AnalogKey
// https://github.com/GyverLibs/AnalogKey

#include <EncButton.h>
EncButton<EB_TICK, VIRT_BTN> btn0;
EncButton<EB_TICK, VIRT_BTN> btn1;

#include <AnalogKey.h>
// создаём массив значений сигналов с кнопок
int16_t sigs[16] = {
  1023, 927, 856, 783,
  671,  632,  590,  560,
  504,  480,  455,  440,
  399,  319,  255,  230
};

// указываем пин, количество кнопок и массив значений
AnalogKey<A0, 16, sigs> keys;


void setup() {
  Serial.begin(9600);
}

void loop() {
  btn0.tick(keys.status(0));
  btn1.tick(keys.status(1));

  // забираем действия с кнопок
  if (btn0.click()) Serial.println("click 0");
  if (btn0.held()) Serial.println("held 0");

  if (btn1.press()) Serial.println("press 1");
  if (btn1.step()) Serial.println("step 1");
}
