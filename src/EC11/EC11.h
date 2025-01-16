#pragma once
#include "Arduino.h"

class EC11
{
private:
    int _pinA, _pinB, _pinSW = -1;
    bool _oldPinA, _oldPinB = false;
    
    bool _reseted = true;
    int _direction = 0;
    bool _oldSwState, _swRisingEdge = false;

    bool _invertDirection = false;

public:
    EC11(int pinA, int pinB, int pinSW);
    ~EC11();

    void Begin();
    void Loop();

    int getDirection();
    bool isSwitchPressed();
    bool isSwitchRisingEdge();

    void setInvertDirection(bool value);
};

extern EC11 ec11;