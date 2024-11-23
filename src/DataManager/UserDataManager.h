#pragma once
#include <Arduino.h>
#include "SD.h"
#include "ArduinoJson.h"
#include "pdebug/pdebug.h"
#include "Alarm/AlarmManager.h"
#include "AudioTools.h"

class UserDataManager
{
private:
    JsonDocument _userData;
public:
    UserDataManager();
    ~UserDataManager();

    bool Load();
    bool Save();

    const char* getLastSelectedSource() { return _userData["LastSelectedSource"]; };
    void setLastSelectedSource(String value) { _userData["LastSelectedSource"] = value; };

    const char* getLastSelectedOutput() { return _userData["LastSelectedOutput"]; };
    void setLastSelectedOutput(String value) { _userData["LastSelectedOutput"] = value; };

    int getTimeZone() { return _userData["TimeZone"]; };
    void setTimeZone(int value) { _userData["TimeZone"] = value; };

    Vector<Alarm> getAlarmList() { 
        JsonArray arr = _userData["AlarmList"]; 

        debug.printlnInfo("Loading " + String(arr.size()) + " alarm.");

        Vector<Alarm> vec;

        for (size_t i = 0; i < arr.size(); i++)
        {
            Alarm alarm;
            JsonVariant AlarmData = arr[i];
            alarm.enable = AlarmData["enable"];
            alarm.hour = AlarmData["hour"];
            alarm.minute = AlarmData["minute"];
            alarm.dayOfWeek = AlarmData["dayOfWeek"];
            vec.push_back(alarm);
        }
        
        return vec;
    }

    void setAlarmList(Vector<Alarm> value) {
        
        JsonArray arr = _userData["AlarmList"].to<JsonArray>();
        arr.clear();
        
        debug.printlnInfo("Saving " + String(value.size()) + " alarm.");

        for (size_t i = 0; i < value.size(); i++)
        {
            Alarm alarm = value[i];
            JsonVariant AlarmData = arr.add();
            AlarmData["enable"] = alarm.enable;
            AlarmData["hour"] = alarm.hour;
            AlarmData["minute"] = alarm.minute;
            AlarmData["dayOfWeek"] = alarm.dayOfWeek;
            // arr.add(AlarmData);
        }
        
        // _userData["AlarmList"] = arr; 
         
    };

};

extern UserDataManager userDataManager;