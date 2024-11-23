#include "AlarmManager.h"
#include "DataManager/UserDataManager.h"
#include "debug/debug.h"

AlarmManager alarmManager;

AlarmManager::AlarmManager(/* args */)
{
}

AlarmManager::~AlarmManager()
{
}

void AlarmManager::Begin()
{
    rtc.offset = userDataManager.getTimeZone() * 3600;
    _alarmList = userDataManager.getAlarmList();

    int year = nextion.getYear();
    int month = nextion.getMonth();
    int day = nextion.getDay();
    int hour = nextion.getHour();
    int minute = nextion.getMinute();
    int second = nextion.getSecond();
    Serial.printf("year = %i; month = %i; day = %i; hour = %i; minute = %i; second = %i\n", year, month, day, hour, minute, second);
    rtc.setTime(second, minute, hour, day, month, year);
}

void AlarmManager::Loop()
{

    tm timeInfo = rtc.getTimeStruct();
    if(_oldMinute == timeInfo.tm_min) return;
    _oldMinute = timeInfo.tm_min;

    for (size_t i = 0; i < _alarmList.size(); i++)
    {
        Alarm a = _alarmList[i];
        if(!a.enable) continue;

        if(a.hour != timeInfo.tm_hour || a.minute != timeInfo.tm_min || ( ( a.dayOfWeek & dayOfWeekFromTimeInfoFormat(timeInfo.tm_wday) ) == 0) ) continue;

        debug.printlnInfo("ALARM !!!!!");

    }
    
}

Alarm AlarmManager::getAlarm(String alarmName)
{
    for (size_t i = 0; i < _alarmList.size(); i++)
    {
        if(_alarmList[i].getName() == alarmName) return _alarmList[i];
    }
    
    return Alarm();
}

Alarm AlarmManager::getAlarm(int index)
{
    if(index < 0 || index >= _alarmList.size() ) return Alarm();
    return _alarmList[index];
}

void AlarmManager::setAlarmHour(int index, int hour)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.hour = hour;
    _alarmList[index] = alarm;
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(index);
}

void AlarmManager::setAlarmMinute(int index, int minute)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.minute = minute;
    _alarmList[index] = alarm;
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(index);
}

void AlarmManager::setAlarmEnabled(int index, bool enabled)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.enable = enabled;
    _alarmList[index] = alarm;
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(index);
}

void AlarmManager::setAlarmDay(int index, DayOfWeek dayOfWeek)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.dayOfWeek = alarm.dayOfWeek ^ dayOfWeek;
    _alarmList[index] = alarm;
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(index);
}

void AlarmManager::NewAlarm()
{
    _alarmList.push_back(Alarm());
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(_alarmList.size()-1);
}

void AlarmManager::RemoveAlarm(int index)
{
    if (index >= _alarmList.size()) index = _alarmList.size() - 1;
    _alarmList.erase(index);
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(index - 1);
}

void AlarmManager::SaveAlarm()
{
    debug.printlnInfo("Saving alarm.");
    userDataManager.setAlarmList(_alarmList);
    userDataManager.Save();
}
