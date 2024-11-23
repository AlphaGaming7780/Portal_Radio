#pragma once
#include "ESP32Time.h"
#include "AudioTools.h"
#include "Nextion/Nextion.h"
#include "Alarm/Alarm.h"

class AlarmManager
{

public:
    AlarmManager(/* args */);
    ~AlarmManager();

    void Begin();

    void Loop();

    Vector<Alarm> getAlarmVector() { return _alarmList; };
    Alarm getAlarm(String alarm);
    Alarm getAlarm(int index);

    void setAlarmHour(int index, int hour);
    void setAlarmMinute(int index, int minute);
    void setAlarmEnabled( int index, bool enabled);
    void setAlarmDay(int index, DayOfWeek dayOfWeek);

    void NewAlarm();
    void RemoveAlarm(int index);
    void SaveAlarm();

private:
    ESP32Time rtc;
    int _oldMinute = -1;

    Vector<Alarm> _alarmList;

};

extern AlarmManager alarmManager;