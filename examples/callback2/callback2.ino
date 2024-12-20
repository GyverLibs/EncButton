// опрос событий через функцию-обработчик

#include <Arduino.h>
#include <EncButton.h>

EncButton eb(2, 3, 4);

void cb() {
    // здесь EB_self - указатель на сам объект

    Serial.print("callback: ");
    switch (eb.getAction()) {
        case EBAction::Press:
            Serial.println("press");
            break;
        case EBAction::Hold:
            Serial.println("hold");
            break;
        case EBAction::Step:
            Serial.println("step");
            break;
        case EBAction::Release:
            Serial.print("release. steps: ");
            Serial.print(eb.getSteps());
            Serial.print(", press for: ");
            Serial.print(eb.pressFor());
            Serial.print(", hold for: ");
            Serial.print(eb.holdFor());
            Serial.print(", step for: ");
            Serial.println(eb.stepFor());
            break;
        case EBAction::Click:
            Serial.println("click");
            break;
        case EBAction::Clicks:
            Serial.print("clicks ");
            Serial.println(eb.getClicks());
            break;
        case EBAction::Turn:
            Serial.print("turn ");
            Serial.print(eb.dir());
            Serial.print(" ");
            Serial.print(eb.fast());
            Serial.print(" ");
            Serial.println(eb.pressing());
            break;
        case EBAction::ReleaseHold:
            Serial.println("release hold");
            break;
        case EBAction::ReleaseHoldClicks:
            Serial.print("release hold clicks ");
            Serial.println(eb.getClicks());
            break;
        case EBAction::ReleaseStep:
            Serial.println("release step");
            break;
        case EBAction::ReleaseStepClicks:
            Serial.print("release step clicks ");
            Serial.println(eb.getClicks());
            break;
        case EBAction::Timeout:
            Serial.println("timeout");
            break;
        default: break;
    }
}

void setup() {
    Serial.begin(115200);
    eb.attach(cb);
}

void loop() {
    eb.tick();
}