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
