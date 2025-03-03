#include "UserDataManager.h"

UserDataManager userDataManager;

UserDataManager::UserDataManager(/* args */)
{
}

UserDataManager::~UserDataManager()
{
}

bool UserDataManager::Load()
{
    debug.printlnInfo("Loading user data.");
    File userDataFile = SD.open(userDataFileLocation, "r", true);
    deserializeJson(_userData, userDataFile.readString());
    userDataFile.close();

    return true;
}

bool UserDataManager::Save()
{
    debug.printlnInfo("Saving user data.");
    String s;
    serializeJson(_userData, s);

    debug.printlnInfo("Removing the userdata.json.");
    if(SD.exists(userDataFileLocation)) SD.remove(userDataFileLocation);
    debug.printlnInfo("Creating the userdata.json.");
    File userDataFile = SD.open(userDataFileLocation, FILE_WRITE, true);
    userDataFile.print(s);
    userDataFile.close();

    return true;
}

Vector<Alarm> UserDataManager::getAlarmList()
{ 
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

void UserDataManager::setAlarmList(Vector<Alarm> value)
{
    
    JsonArray arr = _userData["AlarmList"].to<JsonArray>();
    arr.clear();
    
    debug.printlnInfo("Saving " + String(value.size()) + " alarm.");

    for (size_t i = 0; i < value.size(); i++)
    {
        Alarm alarm = value[i];
        JsonVariant AlarmData = arr.add<JsonVariant>();
        AlarmData["enable"] = alarm.enable;
        AlarmData["hour"] = alarm.hour;
        AlarmData["minute"] = alarm.minute;
        AlarmData["dayOfWeek"] = alarm.dayOfWeek;
        // arr.add(AlarmData);
    }
    
    // _userData["AlarmList"] = arr; 
        
};
