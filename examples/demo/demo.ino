// полное демо
#include <Arduino.h>
// #define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
// #define EB_NO_CALLBACK      // отключить обработчик событий attach (экономит 2 байта оперативки)
// #define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки)
// #define EB_NO_BUFFER        // отключить буферизацию энкодера (экономит 1 байт оперативки)

// #define EB_DEB_TIME 50      // таймаут гашения дребезга кнопки (кнопка)
// #define EB_CLICK_TIME 500   // таймаут ожидания кликов (кнопка)
// #define EB_HOLD_TIME 600    // таймаут удержания (кнопка)
// #define EB_STEP_TIME 200    // таймаут импульсного удержания (кнопка)
// #define EB_FAST_TIME 30     // таймаут быстрого поворота (энкодер)

#include <EncButton.h>
EncButton eb(2, 3, 4);
// EncButton eb(2, 3, 4, INPUT); // + режим пинов энкодера
// EncButton eb(2, 3, 4, INPUT, INPUT_PULLUP); // + режим пинов кнопки

void setup() {
    Serial.begin(115200);

    // показаны значения по умолчанию
    eb.setBtnLevel(LOW);
    eb.setClickTimeout(500);
    eb.setDebTimeout(50);
    eb.setHoldTimeout(600);
    eb.setStepTimeout(200);

    eb.setEncReverse(0);
    eb.setEncType(EB_STEP4_LOW);
    eb.setFastTimeout(30);

    // сбросить счётчик энкодера
    eb.counter = 0;
}

void loop() {
    eb.tick();

    // обработка поворота общая
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

    // обработка поворота раздельная
    if (eb.left()) Serial.println("left");
    if (eb.right()) Serial.println("right");
    if (eb.leftH()) Serial.println("leftH");
    if (eb.rightH()) Serial.println("rightH");

    // кнопка
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

    // состояния
    // Serial.println(eb.pressing());
    // Serial.println(eb.holding());
    // Serial.println(eb.busy());
    // Serial.println(eb.waiting());

    // таймаут
    if (eb.timeout(1000)) Serial.println("timeout!");

    // удержание
    if (eb.hold()) Serial.println("hold");
    if (eb.hold(3)) Serial.println("hold 3");

    // импульсное удержание
    if (eb.step()) Serial.println("step");
    if (eb.step(3)) Serial.println("step 3");

    // отпущена после импульсного удержания
    if (eb.releaseStep()) Serial.println("release step");
    if (eb.releaseStep(3)) Serial.println("release step 3");

    // отпущена после удержания
    if (eb.releaseHold()) Serial.println("release hold");
    if (eb.releaseHold(2)) Serial.println("release hold 2");

    // проверка на количество кликов
    if (eb.hasClicks(3)) Serial.println("has 3 clicks");

    // вывести количество кликов
    if (eb.hasClicks()) {
        Serial.print("has clicks: ");
        Serial.println(eb.getClicks());
    }
}