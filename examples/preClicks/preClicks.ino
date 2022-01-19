// срабатывание функций held/hold/step после предварительных кликов

#include <EncButton.h>
EncButton<EB_TICK, 3> btn;

void setup() {
  Serial.begin(9600);
}

void loop() {
  btn.tick();
  if (btn.click()) Serial.println("click");

  // вызов без количества кликов перехватит все остальные вызовы!
  //if (btn.held()) Serial.println("held any clicks");
  if (btn.held(0)) Serial.println("held after 0 clicks");
  if (btn.held(2)) Serial.println("held after 2 clicks");

  // вызов без количества кликов перехватит все остальные вызовы!
  //if (btn.hold()) Serial.println("hold any clicks");
  //if (btn.hold(0)) Serial.println("hold after 0 clicks");
  //if (btn.hold(2)) Serial.println("hold after 2 clicks");

  // вызов без количества кликов перехватит все остальные вызовы!
  //if (btn.step()) Serial.println("step after any clicks");
  if (btn.step(0)) Serial.println("step after 0 clicks");
  if (btn.step(2)) Serial.println("step after 2 clicks");

  // вызов без количества кликов перехватит все остальные вызовы!
  //if (btn.releaseStep()) Serial.println("release step after any clicks");
  if (btn.releaseStep(0)) Serial.println("release step after 0 clicks");
  if (btn.releaseStep(2)) Serial.println("release step after 2 clicks");
}
