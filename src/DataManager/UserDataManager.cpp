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

    File userDataFile = SD.open("SystemData/userdata.json", "r", true);
    deserializeJson(_userData, userDataFile.readString());
    userDataFile.close();

    return true;
}

bool UserDataManager::Save()
{
    String s;
    serializeJson(_userData, s);

    SD.remove("SystemData/userdata.json");
    File userDataFile = SD.open("SystemData/userdata.json", "w", true);
    userDataFile.print(s);
    userDataFile.close();

    return true;
}
