#include <Arduino.h>
#include "SD.h"
#include "ArduinoJson.h"

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

};

extern UserDataManager userDataManager;