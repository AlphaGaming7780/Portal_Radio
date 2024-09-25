#include "server.h"
#include "main.h"
#include "Audio/AudioManager.h"

AsyncWebServer server(80);

void Setup_Server() {
    Serial.println("Tentaive de démarage du server Web.");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send(SPIFFS, "/index.html", "text/html");
        }
    );

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send(SPIFFS, "/script.js", "text/javascript");
        }
    );

    server.on("/PortalRadio.css", HTTP_GET, [](AsyncWebServerRequest *request) 
        {
            request->send(SPIFFS, "/PortalRadio.css", "text/css");
        }
    );

    server.on("/Data/Monitoring", HTTP_GET, [](AsyncWebServerRequest *request) 
        {
            AsyncJsonResponse * response = new AsyncJsonResponse();
            response->addHeader("Server","Portal Radio Data Monitoring");
            JsonVariant& root = response->getRoot();

            GetSystemData(root);
            GetBluetoothData(root);
            GetWifiData(root);

            response->setLength();
            request->send(response);
        }
    );

    server.on("/Data/AudioSourcesAndOutputs", HTTP_GET, [](AsyncWebServerRequest *request) 
        {
            AsyncJsonResponse * response = new AsyncJsonResponse();
            response->addHeader("Server","Portal Radio Data AudioSourcesAndOutputs");
            JsonVariant& root = response->getRoot();

            int outputsListSize = sizeof(audioManager.outputList) / sizeof(audioManager.outputList[0]);
            int sourcesListSize = sizeof(audioManager.sourceList) / sizeof(audioManager.sourceList[0]);

            String outputsName[outputsListSize];
            String sourcesName[sourcesListSize];

            for(int i = 0; i < outputsListSize; i++) {
                outputsName[i] = audioManager.outputList[i]->getID();
            }

            for(int i = 0; i < sourcesListSize; i++) {
                sourcesName[i] = audioManager.sourceList[i]->getID();
            }

            root["outputsName"] = outputsName;
            root["sourcesName"] = sourcesName;

            response->setLength();
            request->send(response);
        }
    );

    server.begin();
    Serial.println("Server actif!");
}

void GetSystemData(JsonVariant& root) {
    esp_chip_info_t info;
    esp_chip_info(&info);
    JsonObject SystemData = root["System"].to<JsonObject>();
    SystemData["cores"] = info.cores;
    SystemData["features"] = SystemFeaturesToString(info.features);
    SystemData["model"] = SystemModelToString(info.model);
    SystemData["full_revision"] = info.full_revision;
    SystemData["revision"] = info.revision;
}

String SystemFeaturesToString(uint32_t features) {
    String out = "";
    if(features && CHIP_FEATURE_EMB_FLASH) out += "Embedded flash memory,\n";
    if(features && CHIP_FEATURE_WIFI_BGN) out += "2.4GHz WiFi,\n";
    if(features && CHIP_FEATURE_BLE) out += "Bluetooth LE,\n";
    if(features && CHIP_FEATURE_BT) out += "Bluetooth Classic,\n";
    if(features && CHIP_FEATURE_IEEE802154) out += "IEEE 802.15.4,\n";
    if(features && CHIP_FEATURE_EMB_PSRAM) out += "Embedded psram,\n";
    return out;
}

String SystemModelToString(esp_chip_model_t model) {
    switch (model)
    {
    case CHIP_ESP32:
        return "ESP32";
    case CHIP_ESP32S2:
        return "ESP32-S2";
    case CHIP_ESP32S3:
        return "ESP32-S3";
    case CHIP_ESP32C3:
        return "ESP32-C3";
    case CHIP_ESP32H2:
        return "ESP32-H2";
    default:
        return "Unknown";
    }
}

void GetBluetoothData(JsonVariant& root) {
    JsonObject BluetoothData = root["Bluetooth"].to<JsonObject>();
    // BluetoothData["status"] = bluetoothAudioSource.a2dp_sink.get_connection_state();
    // BluetoothData["peerName"] = bluetoothAudioSource.a2dp_sink.get_peer_name();
}

void GetWifiData(JsonVariant& root) {
    JsonObject WifiData = root["Wifi"].to<JsonObject>();
    WifiData["status"] = WiFi.status();
    WifiData["ssid"] = WiFi.SSID();
    WifiData["ip"] = WiFi.localIP();
    WifiData["mac"] = WiFi.macAddress();
    WifiData["subnetMask"] = WiFi.subnetMask();
    WifiData["dns"] = WiFi.dnsIP();
    
    // return WifiData;
}

JsonVariant GetBluetoothData()
{
    
    return JsonVariant();
}
