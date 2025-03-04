#pragma once
#include "ESP32Time.h"
#include "AudioTools.h"
#include "Nextion/Nextion.h"
#include "Alarm/Alarm.h"

#define alarmFileLocation "/SystemData/alarm.mp3"

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
    void SaveAlarm(int index);

    bool IsAlarmRinging();
    void StopCurrentAlarm();

    void SetClockDirty();

private:
    ESP32Time rtc;
    int _oldMinute = -1;
    int _oldHour = -1;

    bool _isAlarmRinging = false;

    bool _isClockDirty = false;

    Vector<Alarm> _alarmList;

    bool _ShouldSyncTime(tm timeInfo);
    tm _SyncTime();
    bool _ShouldCheckAlarm(tm timeInfo);
    bool _ShouldUpdateNextionDate(tm timeInfo);
    void _UpdateNextionDate(tm timeInfo);
    void _CheckAlarm(tm timeInfo);
    void _CheckMissedAlarm(tm timeInfo, tm newTimeInfo);

    void _RingAlarm(Alarm alarm);


};

extern AlarmManager alarmManager;