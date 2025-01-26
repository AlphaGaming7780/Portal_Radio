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

    const char* getLastSelectedSource() { return _userData["LastSelectedSource"]; }
    void setLastSelectedSource(String value) { _userData["LastSelectedSource"] = value; }

    const char* getLastSelectedOutput() { return _userData["LastSelectedOutput"]; }
    void setLastSelectedOutput(String value) { _userData["LastSelectedOutput"] = value; }

    float getVolume() { return _userData["Volume"]; }
    void setVolume(float value) { _userData["Volume"] = value; }

    bool getMute() { return _userData["Mute"]; }
    void setMute(bool value) { _userData["Mute"] = value; }

    bool getEC11InvertDirection() { return _userData["EC11InvertDirection"]; }
    void setEC11InvertDirection(bool value) { _userData["EC11InvertDirection"] = value; }

    int getTimeZone() { return _userData["TimeZone"]; }
    void setTimeZone(int value) { _userData["TimeZone"] = value; }

    Vector<Alarm> getAlarmList();
    void setAlarmList(Vector<Alarm> value);

    uint32_t getLastFmFreq() {return _userData["LastFmFreq"];}
    void setLastFmFreq(uint32_t lastFmFreq){ _userData["LastFmFreq"] = lastFmFreq; }

    uint32_t getLastDabProgramIndex() { return _userData["LastDabProgramIndex"]; }
    void setLastDabProgramIndex(uint32_t lastDabProgramIndex) { _userData["LastDabProgramIndex"] = lastDabProgramIndex; }

    String getLastDabEnsembleIdFilter() { return _userData["LastDabEnsembleIdFilter"]; }
    void setLastDabEnsembleIdFilter(String ensembleId) { _userData["LastDabEnsembleIdFilter"] = ensembleId; }

};

extern UserDataManager userDataManager;