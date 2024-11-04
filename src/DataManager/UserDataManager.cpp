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
    File userDataFile = SD.open("/SystemData/userdata.json", "r", true);
    deserializeJson(_userData, userDataFile.readString());
    userDataFile.close();

    return true;
}

bool UserDataManager::Save()
{
    debug.printlnInfo("Saving user data.");
    String s;
    serializeJson(_userData, s);

    SD.remove("/SystemData/userdata.json");
    File userDataFile = SD.open("/SystemData/userdata.json", "w", true);
    userDataFile.print(s);
    userDataFile.close();

    return true;
}
