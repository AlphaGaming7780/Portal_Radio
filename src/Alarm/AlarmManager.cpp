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
    // rtc.offset = userDataManager.getTimeZone() * 3600;
    _alarmList = userDataManager.getAlarmList();

    // int year = nextion.getYear();
    // int month = nextion.getMonth();
    // int day = nextion.getDay();
    // int hour = nextion.getHour();
    // int minute = nextion.getMinute();
    // int second = nextion.getSecond();

    tm timeInfo = _SyncTime();
    _oldHour = timeInfo.tm_hour;
    _oldMinute = timeInfo.tm_min;
}

void AlarmManager::Loop()
{
    tm timeInfo = rtc.getTimeStruct();

    if(_ShouldSyncTime(timeInfo)) {
        tm newTimeInfo = _SyncTime();

        _CheckMissedAlarm(timeInfo, newTimeInfo);

        timeInfo = newTimeInfo;

    }

    if(_ShouldCheckAlarm(timeInfo)) _CheckAlarm(timeInfo);
    if(_ShouldUpdateNextionDate(timeInfo)) _UpdateNextionDate(timeInfo);

    _oldHour = timeInfo.tm_hour;
    _oldMinute = timeInfo.tm_min;

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
    // nextion.SendAlarmList(_alarmList);
    // nextion.SelectAlarm(index);
}

void AlarmManager::setAlarmMinute(int index, int minute)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.minute = minute;
    _alarmList[index] = alarm;
    // nextion.SendAlarmList(_alarmList);
    // nextion.SelectAlarm(index);
}

void AlarmManager::setAlarmEnabled(int index, bool enabled)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.enable = enabled;
    _alarmList[index] = alarm;
    // nextion.SendAlarmList(_alarmList);
    // nextion.SelectAlarm(index);
}

void AlarmManager::setAlarmDay(int index, DayOfWeek dayOfWeek)
{
    if(index < 0 || index >= _alarmList.size() ) return;
    Alarm alarm = _alarmList[index];
    alarm.dayOfWeek = alarm.dayOfWeek ^ dayOfWeek;
    _alarmList[index] = alarm;
    // nextion.SendAlarmList(_alarmList);
    // nextion.SelectAlarm(index);
}

void AlarmManager::NewAlarm()
{
    _alarmList.push_back(Alarm());
    SaveAlarm(_alarmList.size()-1);
}

void AlarmManager::RemoveAlarm(int index)
{
    if (index >= _alarmList.size()) index = _alarmList.size() - 1;
    _alarmList.erase(index);
    SaveAlarm(index - 1);
}

void AlarmManager::SaveAlarm(int index)
{
    debug.printlnInfo("Saving alarm.");
    userDataManager.setAlarmList(_alarmList);
    userDataManager.Save();
    nextion.SendAlarmList(_alarmList);
    nextion.SelectAlarm(index);
}

bool AlarmManager::IsAlarmRinging()
{
    return _isAlarmRinging;
}

void AlarmManager::StopCurrentAlarm()
{
    _isAlarmRinging = false;
    audioManager.setAudioSource(nullptr, true);
}

void AlarmManager::SetClockDirty()
{
    _isClockDirty = true;
}

bool AlarmManager::_ShouldSyncTime(tm timeInfo)
{
    bool should = _oldHour != timeInfo.tm_hour || _isClockDirty;
    _isClockDirty = false;
    return should;
}

tm AlarmManager::_SyncTime()
{
    uint8_t year, month, week, day, hour, minute, second;
    t4b.GetClock(&second, &minute, &hour, &day, &week, &month, &year);

    // Serial.printf("year = %i; month = %i; day = %i; hour = %i; minute = %i; second = %i\n", year + 2000, month, day, hour, minute, second);
    rtc.setTime(second, minute, hour, day, month, year + 2000);

    return rtc.getTimeStruct();
}

bool AlarmManager::_ShouldCheckAlarm(tm timeInfo)
{
    bool should = _oldMinute != timeInfo.tm_min || _oldHour != timeInfo.tm_hour;
    return should;
}

bool AlarmManager::_ShouldUpdateNextionDate(tm timeInfo)
{
    bool should = _oldHour != timeInfo.tm_hour;
    return should;
}

void AlarmManager::_UpdateNextionDate(tm timeInfo)
{
    nextion.setYear(timeInfo.tm_year);
    nextion.setMonth(timeInfo.tm_mon);
    nextion.setDay(timeInfo.tm_mday);
    nextion.setHour(timeInfo.tm_hour);
    nextion.setMinute(timeInfo.tm_min);
    nextion.setSecond(timeInfo.tm_sec);
    nextion.setDateString(rtc.getTime("%A %d-%m-%Y"));
}

void AlarmManager::_CheckAlarm(tm timeInfo)
{
    if(_isAlarmRinging) return;
    for (size_t i = 0; i < _alarmList.size(); i++)
    {
        Alarm a = _alarmList[i];
        if(!a.enable) continue;

        if(a.hour != timeInfo.tm_hour || a.minute != timeInfo.tm_min || ( ( a.dayOfWeek & dayOfWeekFromTimeInfoFormat(timeInfo.tm_wday) ) == 0) ) continue;

        debug.printlnInfo("ALARM !!!!!");

        return _RingAlarm(a);

    }
}

void AlarmManager::_CheckMissedAlarm(tm timeInfo, tm newTimeInfo)
{
    if(_isAlarmRinging) return;

    int cTime = timeInfo.tm_hour * 60 + timeInfo.tm_min;
    int nTime = newTimeInfo.tm_hour * 60 + newTimeInfo.tm_min;

    if(timeInfo.tm_mday != newTimeInfo.tm_mday) {
        nTime += 24 * 60;
    }

    int deltaTime = nTime - cTime;

    if(deltaTime <= 0 ) {
        if( (debug.getDebugLevel() & pDebugLevel::Debug_ERROR) > 0) {
            debug.printError("[AlarmManager] deltaTime <= 0, ");
            Serial.printf("nTime: %i, cTime: %i.\n", nTime, cTime);
        }
        return;
    }

    for (size_t i = 0; i < _alarmList.size(); i++)
    {
        Alarm a = _alarmList[i];
        if(!a.enable) continue;

        int alarmTime = timeInfo.tm_hour * 60 + timeInfo.tm_min;

        // If alarm is lower than our current time, add 24 hours to the delat to take in account the next day.
        if( alarmTime < cTime ) {
            alarmTime += 24 * 60;
        }

        int alarmDeltaTime = alarmTime - cTime;

        if( alarmDeltaTime < 0 || alarmDeltaTime > deltaTime || ( ( a.dayOfWeek & dayOfWeekFromTimeInfoFormat(timeInfo.tm_wday) ) == 0) ) continue;
        
        debug.printlnInfo("ALARM !!!!!");
        
        return _RingAlarm(a);
    }
}

void AlarmManager::_RingAlarm(Alarm alarm)
{
    _isAlarmRinging = true;

    audioManager.setAudioSource(&sdSource, true);
    if(SD.exists(alarmFileLocation)) sdSource.setPathAndPlay(alarmFileLocation);
}
