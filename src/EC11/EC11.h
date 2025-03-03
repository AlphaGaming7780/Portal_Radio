#pragma once
#include "Arduino.h"

class EC11
{
private:
    int _pinA, _pinB, _pinSW = 32;
    bool _oldPinA, _oldPinB = false;
    
    bool _reseted = true;
    int _direction = 0;
    bool _oldSwState, _swRisingEdge = false;

    bool _invertDirection = false;

    static void DoPinAInterrupt();
    static void DoPinBInterrupt();
    static void DoButtonInterrupt();

public:
    EC11(int pinA, int pinB, int pinSW);
    ~EC11();

    void Begin();

    int getDirection();
    bool isSwitchPressed();
    bool isSwitchRisingEdge();

    void setInvertDirection(bool value);
};

extern EC11 ec11;