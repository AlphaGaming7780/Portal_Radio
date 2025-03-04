
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
	attachInterrupt(digitalPinToInterrupt(_pinA), DoPinAInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(_pinB), DoPinBInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(_pinSW), DoButtonInterrupt, CHANGE);
}

void EC11::Loop()
{

}

void EC11::DoPinAInterrupt() {
	if (digitalRead(ec11._pinA) == HIGH) {
		if (digitalRead(ec11._pinB) == LOW) {
			ec11._direction = ec11._invertDirection ? 1 : -1;
		}
		else {
			ec11._direction = ec11._invertDirection ? -1 : 1;
		}
	}
	else
	{
		if (digitalRead(ec11._pinB) == HIGH) {
			ec11._direction = ec11._invertDirection ? 1 : -1;
		}
		else {
			ec11._direction = ec11._invertDirection ? -1 : 1;
		}
	}
}

void EC11::DoPinBInterrupt() {
	if (digitalRead(ec11._pinB) == HIGH) {
		if (digitalRead(ec11._pinA) == LOW) {
			ec11._direction = ec11._invertDirection ? -1 : 1;
		}
		else {
			ec11._direction = ec11._invertDirection ? 1 : -1;
		}
	}
	else
	{
		if (digitalRead(ec11._pinA) == HIGH) {
			ec11._direction = ec11._invertDirection ? -1 : 1;
		}
		else {
			ec11._direction = ec11._invertDirection ? 1 : -1;
		}
	}	
}

void EC11::DoButtonInterrupt() {
	bool sw = digitalRead(ec11._pinSW);
	if(ec11._oldSwState && !sw) 
	{
		//Falling edge

	} 
	else if (!ec11._oldSwState && sw) 
	{
		//Rising edge
		if(alarmManager.IsAlarmRinging()) alarmManager.StopCurrentAlarm();
		else if(nextion.IsSleeping()) nextion.Sleep(false);

	}
	debug.printlnInfo("SW");
	ec11._oldSwState = sw;
}

int EC11::getDirection()
{
	int dir = _direction;
	_direction = 0;
	return dir;
}

// bool EC11::isSwitchPressed()
// {
// 	return _oldSwState;
// }

// bool EC11::isSwitchRisingEdge()
// {
// 	return _swRisingEdge;
// }

void EC11::setInvertDirection(bool value)
{
	userDataManager.setEC11InvertDirection(value);
	_invertDirection = value;
}
