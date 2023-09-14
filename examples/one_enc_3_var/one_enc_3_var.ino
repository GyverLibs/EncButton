// управление тремя переменными при помощи энкодера:
// - нащёлкай кнопкой нужную переменную (1, 2 или 3 клика)
// - 1 переменная просто изменяется с постоянным шагом
// - 2 переменная: шаг 1, при зажатой кнопке - шаг 5
// - 3 переменная: шаг 1, при быстром вращении - шаг 5

#include <Arduino.h>
#include <EncButton.h>
EncButton eb(2, 3, 4);

int var1 = 0;
int var2 = 0;
int var3 = 0;
uint8_t select = 1;  // выбранная переменная

void setup() {
    Serial.begin(115200);
}

void loop() {
    eb.tick();

    // выбор переменной для изменения
    if (eb.hasClicks()) {
        select = eb.getClicks();
        Serial.println(String("Select: ") + select);
    }

    if (eb.turn()) {
        // меняем переменную
        switch (select) {
            case 1:
                // изменение с шагом 5
                var1 += 5 * eb.dir();
                break;
            case 2:
                // изменение с шагом 1, при зажатой кнопке шаг 5
                var2 += (eb.pressing() ? 5 : 1) * eb.dir();
                break;
            case 3:
                // изменение с шагом 1, при быстром вращении шаг 5
                var3 += (eb.fast() ? 5 : 1) * eb.dir();
                break;
        }
        Serial.println(String("vars ") + var1 + ',' + var2 + ',' + var3);
    }
}