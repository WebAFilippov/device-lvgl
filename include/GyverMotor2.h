#pragma once
#include <Arduino.h>

// ================= НАСТРОЙКИ =================
#ifndef GMOTOR2_DT
#define GMOTOR2_DT 50 // период ускорения, мс
#endif

#ifndef GMOTOR2_PWM_FREQ
#define GMOTOR2_PWM_FREQ 20000 // 20 кГц для BTS7960
#endif

// ============================================

enum GM_driver
{
    DRIVER3WIRE, // BTS7960: EN + EN + PWM
};

// ===== PWM обёртка =====
#ifdef ESP32
inline void pwmWrite(uint8_t ch, uint32_t duty)
{
    ledcWrite(ch, duty);
}
#else
inline void pwmWrite(uint8_t pin, uint32_t duty)
{
    analogWrite(pin, duty);
}
#endif

// ===== КЛАСС МОТОРА =====
template <GM_driver GM_TYPE, uint8_t GM_RES = 10>
class GMotor2
{
public:
    // pinA = R_EN, pinB = L_EN, pinC = PWM, pwmCh = PWM_CHANNEL
    GMotor2(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pwmCh = 0)
        : _pinA(pinA), _pinB(pinB), _pinC(pinC), _pwmCh(pwmCh)
    {

        pinMode(_pinA, OUTPUT);
        pinMode(_pinB, OUTPUT);

#ifdef ESP32
        ledcSetup(_pwmCh, GMOTOR2_PWM_FREQ, GM_RES);
        ledcAttachPin(_pinC, _pwmCh);
#else
        pinMode(_pinC, OUTPUT);
#endif
        _setAll(0);
    }

    // ---------- НАСТРОЙКИ ----------
    void setMinDuty(uint16_t mduty)
    {
        _mduty = mduty;
        _run(_speed);
    }

    void setMinDutyPerc(uint8_t p)
    {
        if (p > 100)
            p = 100;
        setMinDuty(maxDuty * p / 100);
    }

    void reverse(bool rev)
    {
        _rev = rev;
        _run(_speed);
    }

    void setDeadtime(uint16_t us)
    {
        _deadtime = us;
    }

    // ---------- УПРАВЛЕНИЕ ----------
    void stop()
    {
        setSpeed(0);
    }

    void brake()
    {
        _setAll(1);
        _speed = 0;
#ifndef GMOTOR2_NO_ACCEL
        _target = 0;
#endif
    }

    void setSpeed(int16_t speed)
    {
        speed = constrain(speed, -maxDuty, maxDuty);
#ifndef GMOTOR2_NO_ACCEL
        _target = speed;
        if (!_ds)
            _run(speed);
#else
        _run(speed);
#endif
    }

    void setSpeedPerc(int16_t spd)
    {
        spd = constrain(spd, -100, 100);
        setSpeed(maxDuty * spd / 100);
    }

    void tick()
    {
#ifndef GMOTOR2_NO_ACCEL
        if (_ds && (_speed != _target) &&
            (uint8_t(uint8_t(millis()) - _tmr) >= GMOTOR2_DT))
        {

            _tmr = millis();
            if (abs(_speed - _target) > _ds)
                _run(_speed + (_speed < _target ? _ds : -_ds));
            else
                _run(_target);
        }
#endif
    }

    void setAccel(uint32_t s)
    {
#ifndef GMOTOR2_NO_ACCEL
        _ds = s * GMOTOR2_DT / 1000;
#endif
    }

    void setAccelPerc(uint8_t p)
    {
        if (p > 100)
            p = 100;
        setAccel(maxDuty * p / 100);
    }

    // ---------- СТАТУС ----------
    int8_t getState()
    {
        return _speed ? (_speed > 0 ? 1 : -1) : 0;
    }

    int16_t getSpeed()
    {
        return _speed;
    }

    static constexpr int32_t maxDuty = (1 << GM_RES) - 1;

private:
    void _run(int16_t spd)
    {
        if (!spd)
        {
            _setAll(0);
            _speed = 0;
            return;
        }

        if (_deadtime && _speed && (_speed > 0) != (spd > 0))
        {
            _setAll(0);
            delayMicroseconds(_deadtime);
        }

        _speed = spd;

        bool dir = (spd > 0) ^ _rev;
        if (spd < 0)
            spd = -spd;

        if (_mduty)
            spd = ((spd * (maxDuty - _mduty)) >> GM_RES) + _mduty;

        // ===== BTS7960: DRIVER3WIRE =====
        digitalWrite(_pinA, !dir); // R_EN
        digitalWrite(_pinB, dir);  // L_EN
        pwmWrite(_pwmCh, spd);     // PWM
    }

    void _setAll(bool val)
    {
        pwmWrite(_pwmCh, val ? maxDuty : 0);
        digitalWrite(_pinA, val);
        digitalWrite(_pinB, val);
    }

    int16_t _mduty = 0, _speed = 0;
#ifndef GMOTOR2_NO_ACCEL
    int16_t _target = 0, _ds = 0;
    uint8_t _tmr = 0;
#endif

    const uint8_t _pinA, _pinB, _pinC;
    uint8_t _pwmCh;
    uint8_t _deadtime = 0;
    bool _rev = 0;
};