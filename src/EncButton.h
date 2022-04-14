/*
    Лёгкая и функциональная библиотека для энкодера, энкодера с кнопкой или просто кнопки с Arduino
    Документация:
    GitHub: https://github.com/GyverLibs/EncButton
    Возможности:
    - Максимально быстрое чтение пинов для AVR (ATmega328/ATmega168, ATtiny85/ATtiny13)
    - Быстрые оптимизированные алгоритмы опроса действий с кнопки/энкодера
    - Лёгкий вес во Flash и SRAM памяти
    - Энкодер: обычный поворот, нажатый поворот, быстрый поворот, доступ к счётчику
        - Поддержка двух типов энкодеров (полношаговый и полушаговый)
        - Высокоточный алгоритм определения позиции, работает даже с некачественными энкодерами
    - Кнопка: антидребезг, клик, несколько кликов, счётчик кликов, удержание, импульсное удержание
    - Ручной опрос действий или режим функций-обработчиков
    - Виртуальный режим (кнопка, энк, энк с кнопкой)
    - Оптимизирована для работы на прерываниях
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
    Опционально используется алгоритм из библиотеки // https://github.com/mathertel/RotaryEncoder

    Версии:
    v1.1 - пуллап отдельныи методом
    v1.2 - можно передать конструктору параметр INPUT_PULLUP / INPUT(умолч)
    v1.3 - виртуальное зажатие кнопки энкодера вынесено в отдельную функцию + мелкие улучшения
    v1.4 - обработка нажатия и отпускания кнопки
    v1.5 - добавлен виртуальный режим
    v1.6 - оптимизация работы в прерывании
    v1.6.1 - PULLUP по умолчанию
    v1.7 - большая оптимизация памяти, переделан FastIO
    v1.8 - индивидуальная настройка таймаута удержания кнопки (была общая на всех)
    v1.8.1 - убран FastIO
    v1.9 - добавлена отдельная отработка нажатого поворота и запрос направления
    v1.10 - улучшил обработку released, облегчил вес в режиме callback и исправил баги
    v1.11 - ещё больше всякой оптимизации + настройка уровня кнопки
    v1.11.1 - совместимость Digispark
    v1.12 - добавил более точный алгоритм энкодера EB_BETTER_ENC
    v1.13 - добавлен экспериментальный EncButton2
    v1.14 - добавлена releaseStep(). Отпускание кнопки внесено в дебаунс
    v1.15 - добавлен setPins() для EncButton2
    v1.16 - добавлен режим EB_HALFSTEP_ENC для полушаговых энкодеров
    v1.17 - добавлен step с предварительными кликами
    v1.18 - не считаем клики после активации step. held() и hold() тоже могут принимать предварительные клики. Переделан и улучшен дебаунс
    v1.18.1 - исправлена ошибка в releaseStep() (не возвращала результат)
    v1.18.2 - fix compiler warnings
    v1.19 - оптимизация скорости, уменьшен вес в sram
    v1.19.1 - ещё чутка увеличена производительность
    v1.19.2 - ещё немного увеличена производительность, спасибо XRay3D
    v1.19.3 - сделал высокий уровень кнопки по умолчанию в виртуальном режиме
    v1.19.4 - фикс EncButton2
    v1.20 - исправлена критическая ошибка в EncButton2
    v1.21 - EB_HALFSTEP_ENC теперь работает для обычного режима
    v1.22 - улучшен EB_HALFSTEP_ENC для обычного режима
    v1.23 - getDir() заменил на dir()
    v2.0 
        - Алгоритм EB_BETTER_ENC оптимизирован и установлен по умолчанию, дефайн EB_BETTER_ENC упразднён
        - Добавлен setEncType() для настройки типа энкодера из программы, дефайн EB_HALFSTEP_ENC упразднён
        - Добавлен setEncReverse() для смены направления энкодера из программы
        - Добавлен setStepTimeout() для установки периода импульсного удержания, дефайн EB_STEP упразднён
        - Мелкие улучшения и оптимизация
*/

#ifndef _EncButton_h
#define _EncButton_h

// ========= НАСТРОЙКИ (можно передефайнить из скетча) ==========
#define _EB_FAST 30         // таймаут быстрого поворота
#define _EB_DEB 50          // дебаунс кнопки
#define _EB_CLICK 400	    // таймаут накликивания

// =========== НЕ ТРОГАЙ ============
#include <Arduino.h>

#ifndef nullptr
#define nullptr NULL
#endif

#ifndef EB_FAST
#define EB_FAST _EB_FAST
#endif
#ifndef EB_DEB
#define EB_DEB _EB_DEB
#endif
#ifndef EB_HOLD
#define EB_HOLD 1000    // совместимость
#endif
#ifndef EB_STEP
#define EB_STEP 500    // совместимость
#endif
#ifndef EB_CLICK
#define EB_CLICK _EB_CLICK
#endif

enum eb_callback {
    TURN_HANDLER,       // 0
    LEFT_HANDLER,       // 1
    RIGHT_HANDLER,      // 2
    LEFT_H_HANDLER,     // 3
    RIGHT_H_HANDLER,    // 4
    CLICK_HANDLER,      // 5
    HOLDED_HANDLER,     // 6
    STEP_HANDLER,       // 7
    PRESS_HANDLER,      // 8
    CLICKS_HANDLER,     // 9
    RELEASE_HANDLER,    // 10
    HOLD_HANDLER,       // 11
    TURN_H_HANDLER,     // 12
    // clicks amount 13
};

// константы
#define EB_TICK 0
#define EB_CALLBACK 1

#define EB_NO_PIN 255

#define VIRT_ENC 254
#define VIRT_ENCBTN 253
#define VIRT_BTN 252

#define EB_FULLSTEP 0
#define EB_HALFSTEP 1

// ===================================== CLASS =====================================
template < uint8_t _EB_MODE, uint8_t _S1 = EB_NO_PIN, uint8_t _S2 = EB_NO_PIN, uint8_t _KEY = EB_NO_PIN >
class EncButton {
public:
    // можно указать режим работы пина
    EncButton(const uint8_t mode = INPUT_PULLUP) {
        if (_S1 < 252 && mode == INPUT_PULLUP) pullUp();
        setButtonLevel(_S1 < 252 ? LOW : HIGH);     // высокий уровень в виртуальном режиме
        #ifdef EB_HALFSTEP_ENC  // совместимость
        _etype = 1;
        #else
        _etype = 0;
        #endif
        _stepT = EB_STEP >> 5;
        _holdT = EB_HOLD >> 6;
    }
    
    // подтянуть пины внутренней подтяжкой
    void pullUp() {
        if (_S1 < 252) {                        // реальное устройство
            if (_S2 == EB_NO_PIN) {             // обычная кнопка
                pinMode(_S1, INPUT_PULLUP);
            } else if (_KEY == EB_NO_PIN) {     // энк без кнопки
                pinMode(_S1, INPUT_PULLUP);
                pinMode(_S2, INPUT_PULLUP);
            } else {                            // энк с кнопкой
                pinMode(_S1, INPUT_PULLUP);
                pinMode(_S2, INPUT_PULLUP);
                pinMode(_KEY, INPUT_PULLUP);
            }
        }
    }
    
    // установить таймаут удержания кнопки для hold(), мс (до 8 000)
    void setHoldTimeout(int tout) {
        _holdT = tout >> 6;
    }
    
    // установить период импульсов step(), мс (до 4 000)
    void setStepTimeout(int tout) {
        _stepT = tout >> 5;
    }
    
    // виртуально зажать кнопку энкодера
    void holdEncButton(bool state) {
        if (state) setF(8);
        else clrF(8);
    }
    
    // уровень кнопки: LOW - кнопка подключает GND (умолч.), HIGH - кнопка подключает VCC
    void setButtonLevel(bool level) {
        if (level) clrF(11);
        else setF(11);
    }
    
    // true - инвертировать направление энкодера (умолч. false)
    void setEncReverse(bool rev) {
        _erev = rev;
    }
    
    // тип энкодера: EB_FULLSTEP (0) по умолч., EB_HALFSTEP (1) если энкодер делает один поворот за два щелчка
    void setEncType(bool type) {
        _etype = type;
    }
        
    // ===================================== TICK =====================================
    // тикер, вызывать как можно чаще
    // вернёт отличное от нуля значение, если произошло какое то событие
    uint8_t tick(uint8_t s1 = 0, uint8_t s2 = 0, uint8_t key = 0) {
        tickISR(s1, s2, key);
        checkCallback();
        return EBState;
    }
    
    // тикер специально для прерывания, не проверяет коллбэки
    uint8_t tickISR(uint8_t s1 = 0, uint8_t s2 = 0, uint8_t key = 0) {
        if (!_isrFlag) {
            _isrFlag = 1;
            // обработка энка (компилятор вырежет блок если не используется)
            // если объявлены два пина или выбран вирт. энкодер или энкодер с кнопкой
            if ((_S1 < 252 && _S2 < 252) || _S1 == VIRT_ENC || _S1 == VIRT_ENCBTN) {
                uint8_t state;
                if (_S1 >= 252) state = s1 | (s2 << 1);             // получаем код
                else state = fastRead(_S1) | (fastRead(_S2) << 1);  // получаем код
                poolEnc(state);
            }

            // обработка кнопки (компилятор вырежет блок если не используется)
            // если S2 не указан (кнопка) или указан KEY или выбран вирт. энкодер с кнопкой или кнопка
            if ((_S1 < 252 && _S2 == EB_NO_PIN) || _KEY != EB_NO_PIN || _S1 == VIRT_BTN || _S1 == VIRT_ENCBTN) {
                if (_S1 < 252 && _S2 == EB_NO_PIN) _btnState = fastRead(_S1);   // обычная кнопка
                if (_KEY != EB_NO_PIN) _btnState = fastRead(_KEY);              // энк с кнопкой
                if (_S1 == VIRT_BTN) _btnState = s1;                            // вирт кнопка
                if (_S1 == VIRT_ENCBTN) _btnState = key;                        // вирт энк с кнопкой
                _btnState ^= readF(11);                                         // инверсия кнопки
                if (_btnState || readF(15)) poolBtn();                          // опрос если кнопка нажата или не вышли таймауты
            }
            _isrFlag = 0;
        }
        return EBState;
    }
    
    // ===================================== CALLBACK =====================================
    // проверить callback, чтобы не дёргать в прерывании
    void checkCallback() {
        if (_EB_MODE) {
            if (turn()) exec(0);
            if (turnH()) exec(12);
            if (EBState > 0 && EBState <= 8) exec(EBState);
            if (release()) exec(10);
            if (hold()) exec(11);
            if (checkFlag(6)) {
                exec(9);
                if (clicks == _amount) exec(13);
            }
            EBState = 0;
        }
    }
    
    // подключить обработчик
    void attach(eb_callback type, void (*handler)()) {
        _callback[type] = *handler;
    }
    
    // отключить обработчик
    void detach(eb_callback type) {
        _callback[type] = nullptr;
    }
    
    // подключить обработчик на количество кликов (может быть только один!)
    void attachClicks(uint8_t amount, void (*handler)()) {
        _amount = amount;
        _callback[13] = *handler;
    }
    
    // отключить обработчик на количество кликов
    void detachClicks() {
        _callback[13] = nullptr;
    }
    
    // ===================================== STATUS =====================================
    uint8_t getState() { return EBState; }          // получить статус
    void resetState() { EBState = 0; }              // сбросить статус
    
    // ======================================= ENC =======================================
    bool left() { return checkState(1); }           // поворот влево
    bool right() { return checkState(2); }          // поворот вправо
    bool leftH() { return checkState(3); }          // поворот влево нажатый
    bool rightH() { return checkState(4); }         // поворот вправо нажатый
    
    bool fast() { return readF(1); }                // быстрый поворот
    bool turn() { return checkFlag(0); }            // энкодер повёрнут
    bool turnH() { return checkFlag(9); }           // энкодер повёрнут нажато
    
    int8_t dir() { return _dir ? -1 : 1; }          // направление последнего поворота, 1 или -1
    int16_t counter = 0;                            // счётчик энкодера
    
    // ======================================= BTN =======================================
    bool busy() { return readF(15); }               // вернёт true, если всё ещё нужно вызывать tick для опроса таймаутов
    bool state() { return _btnState; }              // статус кнопки
    bool press() { return checkState(8); }          // кнопка нажата
    bool release() { return checkFlag(10); }        // кнопка отпущена
    bool click() { return checkState(5); }          // клик по кнопке
    
    bool held() { return checkState(6); }           // кнопка удержана
    bool hold() { return readF(4); }                // кнопка удерживается
    bool step() { return checkState(7); }           // режим импульсного удержания
    bool releaseStep() { return checkFlag(12); }    // кнопка отпущена после импульсного удержания
    
    bool held(uint8_t clk) { return (clicks == clk) ? checkState(6) : 0; }              // кнопка удержана с предварительным накликиванием
    bool hold(uint8_t clk) { return (clicks == clk) ? readF(4) : 0; }                   // кнопка удерживается с предварительным накликиванием
    bool step(uint8_t clk) { return (clicks == clk) ? checkState(7) : 0; }              // режим импульсного удержания с предварительным накликиванием
    bool releaseStep(uint8_t clk) { return (clicks == clk) ? checkFlag(12) : 0; }       // кнопка отпущена после импульсного удержания с предварительным накликиванием
    
    uint8_t clicks = 0;                             // счётчик кликов
    bool hasClicks(uint8_t num) { return (clicks == num && checkFlag(7)) ? 1 : 0; }     // имеются клики
    uint8_t hasClicks() { return checkFlag(6) ? clicks : 0; }                           // имеются клики
    
    // ===================================================================================
    // =================================== DEPRECATED ====================================
    bool isStep() { return step(); }
    bool isHold() { return hold(); }
    bool isHolded() { return held(); }
    bool isHeld() { return held(); }
    bool isClick() { return click(); }
    bool isRelease() { return release(); }
    bool isPress() { return press(); }
    bool isTurnH() { return turnH(); }
    bool isTurn() { return turn(); }
    bool isFast() { return fast(); }
    bool isLeftH() { return leftH(); }
    bool isRightH() { return rightH(); }
    bool isLeft() { return left(); }
    bool isRight() { return right(); }
    int8_t getDir() { return dir(); }
    
    // ===================================== PRIVATE =====================================
private:
    bool fastRead(const uint8_t pin) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        if (pin < 8) return bitRead(PIND, pin);
        else if (pin < 14) return bitRead(PINB, pin - 8);
        else if (pin < 20) return bitRead(PINC, pin - 14);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
        return bitRead(PINB, pin);
#else
        return digitalRead(pin);
#endif
        return 0;
    }
    
    // ===================================== POOL ENC =====================================
    void poolEnc(uint8_t state) {
        if (_prev != state) {
            uint8_t p = (state | (_prev << 2)) << 1;
            _prev = state;
            _ecount += ((0x49941661 >> p) & 0b11) - 1;
            if ((state == 0b11 || (_etype && !state)) && _ecount) {
                uint16_t ms = millis() & 0xFFFF;
                EBState = (_ecount < 0) ? 1 : 2;
                if (_erev) EBState = 3 - EBState;
                _ecount = 0;
                if (_S2 == EB_NO_PIN || _KEY != EB_NO_PIN) {                // энкодер с кнопкой
                    if (!readF(4) && (_btnState || readF(8))) EBState += 2; // если кнопка не "удерживается"
                }
                _dir = EBState & 1;                             // направление
                counter += _dir ? -1 : 1;                       // счётчик
                if (EBState <= 2) setF(0);                      // флаг поворота для юзера
                else if (EBState <= 4) setF(9);                 // флаг нажатого поворота для юзера
                if (ms - _debTmr < EB_FAST) setF(1);            // быстрый поворот
                else clrF(1);                                   // обычный поворот
                _debTmr = ms;
            }
        }
    }
    
    // ===================================== POOL BTN =====================================
    void poolBtn() {
        uint16_t ms = millis() & 0xFFFF;
        uint16_t debounce = ms - _debTmr;
        if (_btnState) {                                                // кнопка нажата
            setF(15);                                                   // busy флаг
            if (!readF(3)) {                                            // и не была нажата ранее
                if (readF(14)) {                                        // ждём дебаунс
                    if (debounce > EB_DEB) {                            // прошел дебаунс
                        setF(3);                                        // флаг кнопка была нажата
                        EBState = 8;                                    // кнопка нажата
                        _debTmr = ms;                                   // сброс таймаутов
                    }
                } else {                                                // первое нажатие
                    EBState = 0;
                    setF(14);                                           // запомнили что хотим нажать                    
                    if (debounce > EB_CLICK || readF(5)) {              // кнопка нажата после EB_CLICK
                        clicks = 0;                                     // сбросить счётчик и флаг кликов
                        flags &= ~0b0011000011100000;                   // clear 5 6 7 12 13 (клики)
                    }
                    _debTmr = ms;
                }
            } else {                                                    // кнопка уже была нажата
                if (!readF(4)) {                                        // и удержание ещё не зафиксировано
                    if (debounce < (uint16_t)(_holdT << 6)) {           // прошло меньше удержания
                        if (EBState != 0 && EBState != 8) setF(2);      // но энкодер повёрнут! Запомнили
                    } else {                                            // прошло больше времени удержания
                        if (!readF(2)) {                                // и энкодер не повёрнут
                            EBState = 6;                                // значит это удержание (сигнал)
                            flags |= 0b00110000;                        // set 4 5 запомнили что удерживается и отключаем сигнал о кликах
                            _debTmr = ms;                               // сброс таймаута
                        }
                    }
                } else {                                                // удержание зафиксировано
                    if (debounce > (uint16_t)(_stepT << 5)) {           // таймер степа
                        EBState = 7;                                    // сигналим
                        setF(13);                                       // зафиксирован режим step
                        _debTmr = ms;                                   // сброс таймаута
                    }
                }
            }
        } else {                                                        // кнопка не нажата
            if (readF(3)) {                                             // но была нажата
                if (debounce > EB_DEB) {
                    if (!readF(4) && !readF(2)) {                       // энкодер не трогали и не удерживали - это клик
                        EBState = 5;                                    // click
                        clicks++;
                    }
                    flags &= ~0b00011100;                               // clear 2 3 4                    
                    _debTmr = ms;                                       // сброс таймаута
                    setF(10);                                           // кнопка отпущена
                    if (checkFlag(13)) setF(12);                        // кнопка отпущена после step
                }
            } else if (clicks && !readF(5)) {                           // есть клики
                if (debounce > EB_CLICK) flags |= 0b11100000;           // set 5 6 7 (клики)
            } else clrF(15);                                            // снимаем busy флаг
            checkFlag(14);                                              // сброс ожидания нажатия
        }
    }
    
    // ===================================== MISC =====================================
    bool checkState(uint8_t val) {
        return (EBState == val) ? EBState = 0, 1 : 0;
    }
    bool checkFlag(uint8_t val) {
        return readF(val) ? clrF(val), 1 : 0;
    }
    void exec(uint8_t num) {
        if (*_callback[num]) _callback[num]();
    }
    
    uint16_t flags = 0;   
    inline void setF(const uint8_t x) __attribute__((always_inline)) {flags |= 1 << x;}
    inline void clrF(const uint8_t x) __attribute__((always_inline)) {flags &= ~(1 << x);}
    inline bool readF(const uint8_t x) __attribute__((always_inline)) {return flags & (1 << x);}

    uint8_t EBState : 4;
    int8_t _ecount : 4;
    
    uint8_t _amount : 4;
    uint8_t _prev : 2;
    bool _erev : 1;
    bool _etype : 1;
    
    uint8_t _holdT : 7;
    bool _btnState : 1;
    
    uint8_t _stepT : 7;
    bool _dir : 1;

    bool _isrFlag = 0;
    uint16_t _debTmr = 0;
    void (*_callback[_EB_MODE ? 14 : 0])() = {};
    
    // flags
    // 0 - enc turn
    // 1 - enc fast
    // 2 - enc был поворот
    // 3 - флаг кнопки
    // 4 - hold
    // 5 - clicks flag
    // 6 - clicks get
    // 7 - clicks get num
    // 8 - enc button hold    
    // 9 - enc turn holded
    // 10 - btn released
    // 11 - btn level
    // 12 - btn released after step
    // 13 - step flag
    // 14 - deb flag
    // 15 - busy flag

    // EBState
    // 0 - idle
    // 1 - left
    // 2 - right
    // 3 - leftH
    // 4 - rightH
    // 5 - click
    // 6 - held
    // 7 - step
    // 8 - press
};

#endif
