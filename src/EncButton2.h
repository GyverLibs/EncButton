#ifndef _EncButton2_h
#define _EncButton2_h

// ========= НАСТРОЙКИ (можно передефайнить из скетча) ==========
#define _EB_FAST 30         // таймаут быстрого поворота
#define _EB_DEB 50          // дебаунс кнопки
#define _EB_HOLD 1000       // таймаут удержания кнопки
#define _EB_STEP 500        // период срабатывания степ
#define _EB_CLICK 400	    // таймаут накликивания
//#define EB_BETTER_ENC     // точный алгоритм отработки энкодера (можно задефайнить в скетче)

// =========== НЕ ТРОГАЙ ============
#include <Arduino.h>

#ifndef nullptr
#define nullptr NULL
#endif

// флаг макро
#define _EB_setFlag(x) (flags |= 1 << x)
#define _EB_clrFlag(x) (flags &= ~(1 << x))
#define _EB_readFlag(x) ((flags >> x) & 1)

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

#ifdef EB_BETTER_ENC
static const int8_t _EB_DIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
  0,  1, -1,  0
};
#endif

// константы
#define EB_TICK 0
#define EB_CALLBACK 1

#define EB_BTN 1
#define EB_ENCBTN 2
#define EB_ENC 3
#define VIRT_BTN 4
#define VIRT_ENCBTN 5
#define VIRT_ENC 6

#define EB_PIN_AM (_EB_TYPE <= 3 ? _EB_TYPE : 0)

// ===================================== CLASS =====================================
template < uint8_t _EB_TYPE, uint8_t _EB_MODE = EB_TICK >
class EncButton2 {
public:
    // pinMode, pin1, pin2, pin3
    EncButton2(uint8_t mode = INPUT, uint8_t P1 = 255, uint8_t P2 = 255, uint8_t P3 = 255) {
        setButtonLevel(LOW);
        setPins(mode, P1, P2, P3);
    }
    
    // установить пины
    void setPins(uint8_t mode, uint8_t P1 = 255, uint8_t P2 = 255, uint8_t P3 = 255) {
        if (_EB_TYPE == EB_BTN) {
            pinMode(P1, mode);
            _pins[0] = P1;
        } else if (_EB_TYPE == EB_ENC) {
            pinMode(P1, mode);
            pinMode(P2, mode);
            _pins[0] = P1;
            _pins[1] = P2;
        } else if (_EB_TYPE == EB_ENCBTN) {
            pinMode(P1, mode);
            pinMode(P2, mode);
            pinMode(P3, mode);
            _pins[0] = P1;
            _pins[1] = P2;
            _pins[2] = P3;
        }
    }
    
    // подтянуть пины внутренней подтяжкой
    void pullUp() {
    }
    
    // установить таймаут удержания кнопки для isHold(), мс (до 30 000)
    void setHoldTimeout(int tout) {
        _holdT = tout >> 7;
    }
    
    // виртуально зажать кнопку энкодера
    void holdEncButton(bool state) {
        if (state) _EB_setFlag(8);
        else _EB_clrFlag(8);
    }
    
    // уровень кнопки: LOW - кнопка подключает GND (умолч.), HIGH - кнопка подключает VCC
    void setButtonLevel(bool level) {
        if (level) _EB_clrFlag(11);
        else _EB_setFlag(11);
    }
    
    // ===================================== TICK =====================================
    // тикер, вызывать как можно чаще
    // вернёт отличное от нуля значение, если произошло какое то событие
    uint8_t tick(uint8_t s1 = 0, uint8_t s2 = 0, uint8_t key = 0) {
        tickISR(s1, s2, key);
        checkCallback();
        return EBState;
    }
    
#define EB_BTN 1
#define EB_ENCBTN 2
#define EB_ENC 3
#define VIRT_BTN 4
#define VIRT_ENCBTN 5
#define VIRT_ENC 6
    
    // тикер специально для прерывания, не проверяет коллбэки
    uint8_t tickISR(uint8_t p0 = 0, uint8_t p1 = 0, uint8_t p2 = 0) {
        if (!_isrFlag) {
            _isrFlag = 1;
            
            if (_EB_TYPE <= 3) {                                                // РЕАЛЬНОЕ УСТРОЙСТВО
                if (_EB_TYPE >= 2) poolEnc(fastRead(0) | (fastRead(1) << 1));   // энк или энк с кнопкой
                if (_EB_TYPE <= 2) {                                            // кнопка или энк с кнопкой
                    if (_EB_TYPE == EB_BTN) _btnState = fastRead(0);            // кнопка
                    else _btnState = fastRead(2);                               // энк с кнопкой
                    _btnState ^= _EB_readFlag(11);                              // инверсия кнопки
                    poolBtn(); 
                }
            } else {                                                            // ВИРТУАЛЬНОЕ УСТРОЙСТВО
                if (_EB_TYPE >= 5) poolEnc(p0 | (p1 << 1));                     // энк или энк с кнопкой
                if (_EB_TYPE <= 5) {                                            // кнопка или энк с кнопкой
                    if (_EB_TYPE == VIRT_BTN) _btnState = p0;                   // кнопка
                    else _btnState = p2;                                        // энк с кнопкой
                    _btnState ^= _EB_readFlag(11);                              // инверсия кнопки
                    poolBtn(); 
                }
            }
        }
        _isrFlag = 0;
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
    uint8_t getState() { return EBState; }      // получить статус
    void resetState() { EBState = 0; }          // сбросить статус
    
    // ======================================= ENC =======================================
    bool left() { return checkState(1); }       // поворот влево
    bool right() { return checkState(2); }      // поворот вправо
    bool leftH() { return checkState(3); }      // поворот влево нажатый
    bool rightH() { return checkState(4); }     // поворот вправо нажатый
    
    bool fast() { return _EB_readFlag(1); }     // быстрый поворот
    bool turn() { return checkFlag(0); }        // энкодер повёрнут
    bool turnH() { return checkFlag(9); }       // энкодер повёрнут нажато
    int8_t getDir() { return _dir; }            // направление последнего поворота, 1 или -1
    int16_t counter = 0;                        // счётчик энкодера
    
    // ======================================= BTN =======================================
    bool state() { return _btnState; }          // статус кнопки
    bool press() { return checkState(8); }      // кнопка нажата
    bool release() { return checkFlag(10); }    // кнопка отпущена
    bool click() { return checkState(5); }      // клик по кнопке
    
    bool held() { return checkState(6); }       // кнопка удержана
    bool hold() { return _EB_readFlag(4); }     // кнопка удерживается
    bool step() { return checkState(7); }       // режим импульсного удержания
    bool releaseStep() { return checkFlag(12); }// кнопка отпущена после импульсного удержания
    
    bool held(uint8_t clk) { return (clicks == clk) ? checkState(6) : 0; }          // кнопка удержана с предварительным накликиванием
    bool hold(uint8_t clk) { return (clicks == clk) ? _EB_readFlag(4) : 0; }        // кнопка удерживается с предварительным накликиванием
    bool step(uint8_t clk) { return (clicks == clk) ? checkState(7) : 0; }          // режим импульсного удержания с предварительным накликиванием
    bool releaseStep(uint8_t clk = 0) { return (clicks == clk) ? checkFlag(12) : 0; }   // кнопка отпущена после импульсного удержания с предварительным накликиванием
    
    uint8_t clicks = 0;                         // счётчик кликов
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
    
    // ===================================== PRIVATE =====================================
private:  
    // ===================================== POOL ENC =====================================
    void poolEnc(uint8_t state) {
    #ifdef EB_BETTER_ENC
        if (_prev != state) {
            _ecount += _EB_DIR[state | (_prev << 2)];                   // сдвиг внутреннего счётчика
            _prev = state;
            #ifdef EB_HALFSTEP_ENC                                      // полушаговый энкодер
            // спасибо https://github.com/GyverLibs/EncButton/issues/10#issue-1092009489
            if ((state == 0x3 || state == 0x0) && _ecount != 0) {
            #else                                                       // полношаговый
            if (state == 0x3 && _ecount != 0) {                         // защёлкнули позицию
            #endif
                EBState = (_ecount < 0) ? 1 : 2;
                _ecount = 0;
                if (_EB_TYPE == EB_ENCBTN || _EB_TYPE == VIRT_ENCBTN) { // энкодер с кнопкой
                    if (!_EB_readFlag(4) && (_btnState || _EB_readFlag(8))) EBState += 2;   // если кнопка не "удерживается"
                }
                _dir = (EBState & 1) ? -1 : 1;                          // направление
                counter += _dir;                                        // счётчик
                if (EBState <= 2) _EB_setFlag(0);			            // флаг поворота для юзера
                else if (EBState <= 4) _EB_setFlag(9);			        // флаг нажатого поворота для юзера
                if (millis() - _debTimer < EB_FAST) _EB_setFlag(1);     // быстрый поворот
                else _EB_clrFlag(1);						            // обычный поворот
                _debTimer = millis();
            }
        }
    #else
        if (_encRST && state == 0b11) {                                 // ресет и энк защёлкнул позицию
            if (_EB_TYPE == EB_ENCBTN || _EB_TYPE == VIRT_ENCBTN) {     // энкодер с кнопкой
                if ((_prev == 1 || _prev == 2) && !_EB_readFlag(4)) {   // если кнопка не "удерживается" и энкодер в позиции 1 или 2
                    EBState = _prev;
                    if (_btnState || _EB_readFlag(8)) EBState += 2;
                }
            } else {                                                    // просто энкодер
                if (_prev == 1 || _prev == 2) EBState = _prev;
            }
            
            if (EBState > 0) {                                          // был поворот
                _dir = (EBState & 1) ? -1 : 1;                          // направление
                counter += _dir;                                        // счётчик
                if (EBState <= 2) _EB_setFlag(0);			            // флаг поворота для юзера
                else if (EBState <= 4) _EB_setFlag(9);			        // флаг нажатого поворота для юзера
                if (millis() - _debTimer < EB_FAST) _EB_setFlag(1);     // быстрый поворот
                else _EB_clrFlag(1);						            // обычный поворот
            }

            _encRST = 0;
            _debTimer = millis();
        }
        if (state == 0b00) _encRST = 1;
        _prev = state;
    #endif
    }
    
    // ===================================== POOL BTN =====================================
    void poolBtn() {
        uint32_t thisMls = millis();
        uint32_t debounce = thisMls - _debTimer;
        if (_btnState) {                                                	// кнопка нажата
            if (!_EB_readFlag(3)) {                                         // и не была нажата ранее
                if (_EB_readFlag(14)) {                                     // ждём дебаунс
                    if (debounce > EB_DEB) {                                // прошел дебаунс
                        _EB_setFlag(3);                                     // флаг кнопка была нажата
                        EBState = 8;                                        // кнопка нажата
                        _debTimer = thisMls;                                // сброс таймаутов
                    }
                } else {                                                    // первое нажатие
                    EBState = 0;
                    _EB_setFlag(14);                                        // запомнили что хотим нажать                    
                    if (debounce > EB_CLICK || _EB_readFlag(5)) {           // кнопка нажата после EB_CLICK
                        clicks = 0;											// сбросить счётчик и флаг кликов
                        flags &= ~0b0011000011100000;                       // clear 5 6 7 12 13 (клики)
                    }
                    _debTimer = thisMls;
                }
            } else {                                                      	// кнопка уже была нажата
                if (!_EB_readFlag(4)) {                                     // и удержание ещё не зафиксировано
                    if (debounce < (uint32_t)(_holdT << 7)) {                         // прошло меньше удержания
                        if (EBState != 0 && EBState != 8) _EB_setFlag(2);   // но энкодер повёрнут! Запомнили
                    } else {                                                // прошло больше времени удержания
                        if (!_EB_readFlag(2)) {                             // и энкодер не повёрнут
                            EBState = 6;                                   	// значит это удержание (сигнал)
                            flags |= 0b00110000;                            // set 4 5 запомнили что удерживается и отключаем сигнал о кликах
                            _debTimer = thisMls;                            // сброс таймаута
                        }
                    }
                } else {                                                    // удержание зафиксировано
                    if (debounce > EB_STEP) {                              	// таймер степа
                        EBState = 7;                                       	// сигналим
                        _EB_setFlag(13);                                    // зафиксирован режим step
                        _debTimer = thisMls;                                // сброс таймаута
                    }
                }
            }
        } else {                                                        	// кнопка не нажата
            if (_EB_readFlag(3)) {                                          // но была нажата
                if (debounce > EB_DEB) {
                    if (!_EB_readFlag(4) && !_EB_readFlag(2)) {	            // энкодер не трогали и не удерживали - это клик
                        EBState = 5;                                        // click
                        clicks++;
                    }
                    flags &= ~0b00011100;                                   // clear 2 3 4                    
                    _debTimer = thisMls;                                    // сброс таймаута
                    _EB_setFlag(10);                                        // кнопка отпущена
                    if (checkFlag(13)) _EB_setFlag(12);                     // кнопка отпущена после step
                }
            } else if (clicks > 0 && debounce > EB_CLICK && !_EB_readFlag(5)) flags |= 0b11100000;	 // set 5 6 7 (клики)
            checkFlag(14);                                                  // сброс ожидания нажатия
        }
    }
    
    // ===================================== MISC =====================================
    bool fastRead(uint8_t pin) {
        return digitalRead(_pins[pin]);
    }
    
    bool checkState(uint8_t val) {
        if (EBState == val) {
            EBState = 0;
            return 1;
        } return 0;
    }
    bool checkFlag(uint8_t val) {
        if (_EB_readFlag(val)) {
            _EB_clrFlag(val);
            return 1;
        } return 0;
    }
    void exec(uint8_t num) {
        if (*_callback[num]) _callback[num]();
    }
    
    uint8_t _prev : 2;
    uint8_t EBState : 4;
    bool _btnState : 1;
    bool _encRST : 1;
    bool _isrFlag : 1;
    uint16_t flags = 0;
    
    #ifdef EB_BETTER_ENC
    int8_t _ecount = 0;
    #endif
    
    uint32_t _debTimer = 0;
    uint8_t _holdT = (EB_HOLD >> 7);
    int8_t _dir = 0;
    void (*_callback[_EB_MODE ? 14 : 0])() = {};
    uint8_t _amount = 0;
    
    uint8_t _pins[EB_PIN_AM];

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
