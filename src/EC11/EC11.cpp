
#include "EC11.h"
#include "pdebug/pdebug.h"
#include "DataManager/UserDataManager.h"

EC11 ec11(34, 35, 32);

EC11::EC11(int pinA, int pinB, int pinSW)
{
    _pinA = pinA;
    _pinB = pinB;
    _pinSW = pinSW;
}

EC11::~EC11()
{
}

void EC11::Begin()
{
    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);
    pinMode(_pinSW, INPUT);

    _oldPinA = digitalRead(_pinA);
}

void EC11::Loop()
{

    bool sw = digitalRead(_pinSW);

    if(!_oldSwState && sw) _swRisingEdge = true;
    else _swRisingEdge = false;

    _oldSwState = sw;

    bool pinA = digitalRead(_pinA);
    bool pinB = digitalRead(_pinB);

    if ( ( pinA == 1 && pinB == 1 ) || ( _oldPinA == pinA && _oldPinB == _pinB )) {
        _direction = 0;
        return; 
    }

    _oldPinA = pinA;
    _oldPinB = pinB;

    // Serial.printf("EC11 : PinA : %i, PinB %i, _firstDirection %i\n", pinA, pinB, _firstDirection);

    if( 
        ( pinA && !pinB) 
    ) { 

        if(_firstDirection == 0) {
            _firstDirection = true;
            // Serial.println("_firstDirection = 1");
        } else {
            _direction = _invertDirection ? 1 : -1; 
            _firstDirection = false;
            // Serial.printf("_direction = %i.\n", _direction);
        }
        return; 
    }
    else if ( 
        ( !pinA && pinB ) 
    ) { 
        if(_firstDirection == 0) {
            _firstDirection = true;
            // Serial.println("_firstDirection = -1");
        } else {
            _direction = _invertDirection ? -1 : 1;
            _firstDirection = false;
            // Serial.printf("_direction = %i.\n", _direction);
        }
        return; 
    }
}

int EC11::getDirection()
{
    return _direction;
}

bool EC11::isSwitchPressed()
{
    return _oldSwState;
}

bool EC11::isSwitchRisingEdge()
{
    return _swRisingEdge;
}

void EC11::setInvertDirection(bool value)
{
    userDataManager.setEC11InvertDirection(value);
    _invertDirection = value;
}
