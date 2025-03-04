#pragma once
#include "Arduino.h"

class EC11
{
private:
    int _pinA, _pinB, _pinSW = 0;
    
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
    void Loop();

    int getDirection();

    void setInvertDirection(bool value);
};

extern EC11 ec11;