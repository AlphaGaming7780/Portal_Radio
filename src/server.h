#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

void Setup_Server(); 

void GetSystemData(JsonVariant& root);
String SystemModelToString(esp_chip_model_t model);
String SystemFeaturesToString(uint32_t features);

void GetBluetoothData(JsonVariant& root);

void GetWifiData(JsonVariant& root);