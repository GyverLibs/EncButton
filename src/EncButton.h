/*
    Ультра лёгкая и быстрая библиотека для энкодера, энкодера с кнопкой или просто кнопки
    Документация:
    GitHub: https://github.com/GyverLibs/EncButton
    Возможности:
    - Максимально быстрое чтение пинов для AVR (ATmega328/ATmega168, ATtiny85/ATtiny13)
    - Оптимизированный вес
    - Быстрые и лёгкие алгоритмы кнопки и энкодера
    - Энкодер: поворот, нажатый поворот, быстрый поворот, счётчик
    - Кнопка: антидребезг, клик, несколько кликов, счётчик кликов, удержание, режим step
    - Подключение - только HIGH PULL!
    - Опциональный режим callback (+22б SRAM на каждый экземпляр)
    
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
    v1.19 - заменены макросы проверки флагов на мембер-функции и энумераторы вместо волшебных констант. Экономия 14-20 байт кода на проверках флагов.
*/

#ifndef _EncButton_h
#define _EncButton_h

// ========= НАСТРОЙКИ (можно передефайнить из скетча) ==========
#define _EB_FAST  30   // таймаут быстрого поворота
#define _EB_DEB   50   // дебаунс кнопки
#define _EB_HOLD  1000 // таймаут удержания кнопки
#define _EB_STEP  500  // период срабатывания степ
#define _EB_CLICK 400  // таймаут накликивания
//#define EB_BETTER_ENC     // точный алгоритм отработки энкодера (можно задефайнить в скетче)

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
#define EB_HOLD _EB_HOLD
#endif
#ifndef EB_STEP
#define EB_STEP _EB_STEP
#endif
#ifndef EB_CLICK
#define EB_CLICK _EB_CLICK
#endif

enum eb_callback {
    TURN_HANDLER,    // 0
    LEFT_HANDLER,    // 1
    RIGHT_HANDLER,   // 2
    LEFT_H_HANDLER,  // 3
    RIGHT_H_HANDLER, // 4
    CLICK_HANDLER,   // 5
    HOLDED_HANDLER,  // 6
    STEP_HANDLER,    // 7
    PRESS_HANDLER,   // 8
    CLICKS_HANDLER,  // 9
    RELEASE_HANDLER, // 10
    HOLD_HANDLER,    // 11
    TURN_H_HANDLER,  // 12
    // clicks amount 13
};

// константы
#define EB_TICK     0
#define EB_CALLBACK 1

#define EB_NO_PIN 255

#define VIRT_ENC    254
#define VIRT_ENCBTN 253
#define VIRT_BTN    252

#ifdef EB_BETTER_ENC
static constexpr int8_t _EB_DIR[] = {
    0, -1, +1, 0,
    +1, 0, 0, -1,
    -1, 0, 0, +1,
    0, +1, -1, 0
};
#endif

// ===================================== CLASS =====================================
template <uint8_t _EB_MODE, uint8_t _S1 = EB_NO_PIN, uint8_t _S2 = EB_NO_PIN, uint8_t _KEY = EB_NO_PIN>
class EncButton {
    using Callback = void (*)();

    enum Flags : uint8_t {
        EncTurn,              // 0 - enc turn
        EncFast,              // 1 - enc fast
        EncWasTurn,           // 2 - enc был поворот
        BtnFlag,              // 3 - флаг кнопки
        Hold,                 // 4 - hold
        ClicksFlag,           // 5 - clicks flag
        ClicksGet,            // 6 - clicks get
        ClicksGetNum,         // 7 - clicks get num
        EncButtonHold,        // 8 - enc button hold
        EncTurnHolded,        // 9 - enc turn holded
        Released,             // 10 - btn released
        Btnlevel,             // 11 - btn level
        BtnReleasedAfterStep, // 12 - btn released after step
        StepFlag,             // 13 - step flag
        DebFlag,              // 14 - deb flag
    };

    enum State : uint8_t {
        // EBState
        Idle,   // 0 - idle
        Left,   // 1 - left
        Right,  // 2 - right
        LeftH,  // 3 - leftH
        RightH, // 4 - rightH
        Click,  // 5 - click
        Held,   // 6 - held
        Step,   // 7 - step
        Press,  // 8 - press
    };

public:
    // можно указать режим работы пина
    EncButton(const uint8_t mode = INPUT_PULLUP) {
        if (_S1 < VIRT_BTN && mode == INPUT_PULLUP)
            pullUp();
        setButtonLevel(LOW);
    }

    // подтянуть пины внутренней подтяжкой
    void pullUp() {
        if (_S1 < VIRT_BTN) {       // реальное устройство
            if (_S2 == EB_NO_PIN) { // обычная кнопка
                pinMode(_S1, INPUT_PULLUP);
            } else if (_KEY == EB_NO_PIN) { // энк без кнопки
                pinMode(_S1, INPUT_PULLUP);
                pinMode(_S2, INPUT_PULLUP);
            } else { // энк с кнопкой
                pinMode(_S1, INPUT_PULLUP);
                pinMode(_S2, INPUT_PULLUP);
                pinMode(_KEY, INPUT_PULLUP);
            }
        }
    }

    // установить таймаут удержания кнопки для isHold(), мс (до 30 000)
    void setHoldTimeout(int tout) {
        _holdT = tout >> 7;
    }

    // виртуально зажать кнопку энкодера
    void holdEncButton(bool state) {
        if (state)
            setFlag(EncButtonHold);
        else
            clrFlag(EncButtonHold);
    }

    // уровень кнопки: LOW - кнопка подключает GND (умолч.), HIGH - кнопка подключает VCC
    void setButtonLevel(bool level) {
        if (level)
            clrFlag(Btnlevel);
        else
            setFlag(Btnlevel);
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
            if ((_S1 < VIRT_BTN && _S2 < VIRT_BTN) || _S1 == VIRT_ENC || _S1 == VIRT_ENCBTN) {
                uint8_t state;
                if (_S1 >= VIRT_BTN)
                    state = s1 | (s2 << 1); // получаем код
                else
                    state = fastRead(_S1) | (fastRead(_S2) << 1); // получаем код
                poolEnc(state);
            }

            // обработка кнопки (компилятор вырежет блок если не используется)
            // если S2 не указан (кнопка) или указан KEY или выбран вирт. энкодер с кнопкой или кнопка
            if ((_S1 < VIRT_BTN && _S2 == EB_NO_PIN) || _KEY != EB_NO_PIN || _S1 == VIRT_BTN || _S1 == VIRT_ENCBTN) {
                if (_S1 < VIRT_BTN && _S2 == EB_NO_PIN)
                    _btnState = fastRead(_S1); // обычная кнопка
                if (_KEY != EB_NO_PIN)
                    _btnState = fastRead(_KEY); // энк с кнопкой
                if (_S1 == VIRT_BTN)
                    _btnState = s1; // вирт кнопка
                if (_S1 == VIRT_ENCBTN)
                    _btnState = key;            // вирт энк с кнопкой
                _btnState ^= getFlag(Btnlevel); // инверсия кнопки
                poolBtn();
            }
        }
        _isrFlag = 0;
        return EBState;
    }

    // ===================================== CALLBACK =====================================
    // проверить callback, чтобы не дёргать в прерывании
    void checkCallback() {
        if (_EB_MODE) {
            if (turn())
                exec(0);
            if (turnH())
                exec(12);
            if (EBState > Idle && EBState <= Press)
                exec(EBState);
            if (release())
                exec(10);
            if (hold())
                exec(11);
            if (checkFlag<ClicksGet>()) {
                exec(9);
                if (clicks == _amount)
                    exec(13);
            }
            EBState = Idle;
        }
    }

    // подключить обработчик
    void attach(eb_callback type, Callback handler) {
        _callback[type] = *handler;
    }

    // отключить обработчик
    void detach(eb_callback type) {
        _callback[type] = nullptr;
    }

    // подключить обработчик на количество кликов (может быть только один!)
    void attachClicks(uint8_t amount, Callback handler) {
        _amount = amount;
        _callback[13] = *handler;
    }

    // отключить обработчик на количество кликов
    void detachClicks() {
        _callback[13] = nullptr;
    }

    // ===================================== STATUS =====================================
    uint8_t getState() { return EBState; } // получить статус
    void resetState() { EBState = Idle; }  // сбросить статус

    // ======================================= ENC =======================================
    bool left() { return checkState(Left); }   // поворот влево
    bool right() { return checkState(Right); }  // поворот вправо
    bool leftH() { return checkState(LeftH); }  // поворот влево нажатый
    bool rightH() { return checkState(RightH); } // поворот вправо нажатый

    bool fast() { return getFlag(EncFast); }            // быстрый поворот
    bool turn() { return checkFlag<EncTurn>(); }        // энкодер повёрнут
    bool turnH() { return checkFlag<EncTurnHolded>(); } // энкодер повёрнут нажато

    int8_t getDir() { return _dir; } // направление последнего поворота, 1 или -1
    int16_t counter = 0;             // счётчик энкодера

    // ======================================= BTN =======================================
    bool state() { return _btnState; }               // статус кнопки
    bool press() { return checkState(Press); }           // кнопка нажата
    bool release() { return checkFlag<Released>(); } // кнопка отпущена
    bool click() { return checkState(Click); }           // клик по кнопке

    bool held() { return checkState(Held); }                     // кнопка удержана
    bool hold() { return getFlag(Hold); }                     // кнопка удерживается
    bool step() { return checkState(Step); }                     // режим импульсного удержания
    bool releaseStep() { checkFlag<BtnReleasedAfterStep>(); } // кнопка отпущена после импульсного удержания

    bool held(uint8_t clk) { return (clicks == clk) ? checkState(Held) : 0; }          // кнопка удержана с предварительным накликиванием
    bool hold(uint8_t clk) { return (clicks == clk) ? getFlag(Hold) : 0; }          // кнопка удерживается с предварительным накликиванием
    bool step(uint8_t clk) { return (clicks == clk) ? checkState(Step) : 0; }          // режим импульсного удержания с предварительным накликиванием
    bool releaseStep(uint8_t clk) { return (clicks == clk) ? checkFlag<12>() : 0; } // кнопка отпущена после импульсного удержания с предварительным накликиванием

    uint8_t clicks = 0;                                                               // счётчик кликов
    bool hasClicks(uint8_t num) { return (clicks == num && checkFlag<7>()) ? 1 : 0; } // имеются клики
    uint8_t hasClicks() { return checkFlag<6>() ? clicks : 0; }                       // имеются клики

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

    // ===================================== PRIVATE =====================================

    bool fastRead(const uint8_t pin) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        if (pin < 8)
            return bitRead(PIND, pin);
        else if (pin < 14)
            return bitRead(PINB, pin - 8);
        else if (pin < 20)
            return bitRead(PINC, pin - 14);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
        return bitRead(PINB, pin);
#else
        return digitalRead(pin);
#endif
        return 0;
    }

    // ===================================== POOL ENC =====================================
    void poolEnc(uint8_t state) {
#ifdef EB_BETTER_ENC
        if (_prev != state) {
            _ecount += _EB_DIR[state | (_prev << 2)]; // сдвиг внутреннего счётчика
            _prev = state;
#ifdef EB_HALFSTEP_ENC // полушаговый энкодер
            // спасибо https://github.com/GyverLibs/EncButton/issues/10#issue-1092009489
            if ((state == 0x3 || state == 0x0) && _ecount != 0) {
#else // полношаговый
            if (state == 0x3 && _ecount != 0) { // защёлкнули позицию
#endif
                EBState = (_ecount < 0) ? 1 : 2;
                _ecount = 0;
                if (_S2 == EB_NO_PIN || _KEY != EB_NO_PIN) { // энкодер с кнопкой
                    if (!getFlag(Hold) && (_btnState || getFlag(EncButtonHold)))
                        EBState += Right; // если кнопка не "удерживается"
                }
                _dir = (EBState & Left) ? -1 : 1; // направление
                counter += _dir;               // счётчик
                if (EBState <= Right)
                    setFlag(EncTurn); // флаг поворота для юзера
                else if (EBState <= RightH)
                    setFlag(EncTurnHolded); // флаг нажатого поворота для юзера
                if (millis() - _debTimer < EB_FAST)
                    setFlag(EncFast); // быстрый поворот
                else
                    clrFlag(EncFast); // обычный поворот
                _debTimer = millis();
            }
        }
#else
        if (_encRST && state == 0b11) {                             // ресет и энк защёлкнул позицию
            if (_S2 == EB_NO_PIN || _KEY != EB_NO_PIN) {            // энкодер с кнопкой
                if ((_prev == 1 || _prev == 2) && !getFlag(Hold)) { // если кнопка не "удерживается" и энкодер в позиции 1 или 2
                    EBState = _prev;
                    if (_btnState || getFlag(EncButtonHold))
                        EBState += Right;
                }
            } else { // просто энкодер
                if (_prev == 1 || _prev == 2)
                    EBState = _prev;
            }

            if (EBState > Idle) {                 // был поворот
                _dir = (EBState & Left) ? -1 : 1; // направление
                counter += _dir;               // счётчик
                if (EBState <= Right)
                    setFlag(EncTurn); // флаг поворота для юзера
                else if (EBState <= RightH)
                    setFlag(EncTurnHolded); // флаг нажатого поворота для юзера
                if (millis() - _debTimer < EB_FAST)
                    setFlag(EncFast); // быстрый поворот
                else
                    clrFlag(EncFast); // обычный поворот
            }

            _encRST = 0;
            _debTimer = millis();
        }
        if (state == 0b00)
            _encRST = 1;
        _prev = state;
#endif
    }

    // ===================================== POOL BTN =====================================
    void poolBtn() {
        uint32_t thisMls = millis();
        uint32_t debounce = thisMls - _debTimer;
        if (_btnState) {                     // кнопка нажата
            if (!getFlag(BtnFlag)) {         // и не была нажата ранее
                if (getFlag(DebFlag)) {      // ждём дебаунс
                    if (debounce > EB_DEB) { // прошел дебаунс
                        setFlag(BtnFlag);    // флаг кнопка была нажата
                        EBState = Press;     // кнопка нажата
                        _debTimer = thisMls; // сброс таймаутов
                    }
                } else { // первое нажатие
                    EBState = Idle;
                    setFlag(DebFlag);                                 // запомнили что хотим нажать
                    if (debounce > EB_CLICK || getFlag(ClicksFlag)) { // кнопка нажата после EB_CLICK
                        clicks = 0;                                   // сбросить счётчик и флаг кликов
                        _flags &= ~flags(ClicksFlag, ClicksGet, ClicksGetNum,
                            BtnReleasedAfterStep, StepFlag); // clear (клики)
                    }
                    _debTimer = thisMls;
                }
            } else {                                // кнопка уже была нажата
                if (!getFlag(Hold)) {               // и удержание ещё не зафиксировано
                    if (debounce < (_holdT << 7)) { // прошло меньше удержания
                        if (EBState != Idle && EBState != Press)
                            setFlag(EncWasTurn);               // но энкодер повёрнут! Запомнили
                    } else {                                   // прошло больше времени удержания
                        if (!getFlag(EncWasTurn)) {            // и энкодер не повёрнут
                            EBState = Held;                       // значит это удержание (сигнал)
                            _flags |= flags(Hold, ClicksFlag); // set запомнили что удерживается и отключаем сигнал о кликах
                            _debTimer = thisMls;               // сброс таймаута
                        }
                    }
                } else {                      // удержание зафиксировано
                    if (debounce > EB_STEP) { // таймер степа
                        EBState = Step;       // сигналим
                        setFlag(StepFlag);    // зафиксирован режим step
                        _debTimer = thisMls;  // сброс таймаута
                    }
                }
            }
        } else {                    // кнопка не нажата
            if (getFlag(BtnFlag)) { // но была нажата
                if (debounce > EB_DEB) {
                    if (!getFlag(Hold) && !getFlag(EncWasTurn)) { // энкодер не трогали и не удерживали - это клик
                        EBState = Click;                          // click
                        clicks++;
                    }
                    _flags &= ~flags(EncWasTurn, BtnFlag, Hold); // clear
                    _debTimer = thisMls;                         // сброс таймаута
                    setFlag(Released);                           // кнопка отпущена
                    if (checkFlag<StepFlag>())
                        setFlag(BtnReleasedAfterStep); // кнопка отпущена после step
                }
            } else if (clicks > 0 && debounce > EB_CLICK && !getFlag(ClicksFlag))
                _flags |= flags(ClicksFlag, ClicksGet, ClicksGetNum); // set (клики)
            clrFlag(DebFlag);                                         // сброс ожидания нажатия
        }
    }

    // ===================================== MISC =====================================
private:
    // флаг макро
    void setFlag(Flags x) { _flags |= 1 << x; }
    void clrFlag(Flags x) { _flags &= ~(1 << x); }
    bool getFlag(Flags x) { return _flags & (1 << x); }

    template <typename... Fs>
    constexpr uint16_t flags(Flags f, Fs... fs) const {
        return 1 << f | flags(fs...);
    }
    constexpr uint16_t flags(Flags f) const {
        return 1 << f;
    }

    template <Flags F>
    bool checkFlag() {
        return getFlag(F) ? clrFlag(F), true : false;
    }

    bool checkState(State val) {
        return (EBState == val) ? EBState = Idle, true : false;
    }

    void exec(uint8_t num) {
        if (*_callback[num])
            _callback[num]();
    }

    uint8_t _prev : 2;
    uint8_t EBState : RightH;
    bool _btnState : 1;
    bool _encRST : 1;
    bool _isrFlag : 1;
    uint16_t _flags = 0;

#ifdef EB_BETTER_ENC
    int8_t _ecount = 0;
#endif

    uint32_t _debTimer = 0;
    uint8_t _holdT = (EB_HOLD >> 7);
    int8_t _dir = 0;
    Callback _callback[_EB_MODE ? 14 : 0] {};
    uint8_t _amount = 0;

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
