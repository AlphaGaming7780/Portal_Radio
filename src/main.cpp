#include "main.h"

// #define ESP8266_USE_DEBUGSERIAL
// #include "SoftwareSerial.h"
// #include "ESP8266_Wifi_Module/ESP866WebServer.h"

//                                    RX | TX
// EspSoftwareSerial::UART	ESP8266Serial(18, 19);
// ESP866WebServer webServer(80);
// ESP8266Wifi ESPWifi(&ESP8266Serial);

// https://www.circuitstate.com/pinouts/doit-esp32-devkit-v1-wifi-development-board-pinout-diagram-and-reference/#ADC

// const char* SSID = "S24+ Triton";
// const char* PWD = "axyktb87k2rxbp2";

// const char* SSID = "C chez toi ici";
// const char* PWD = "DecoP9DecoP9";

const char* SSID = "Alpha Gaming";
const char* PWD = "loliloli";

void setup() {

    LOGLEVEL_HELIX = LogLevelHelix::Info;
    debug.setDebugLevel(Debug_INFO | Debug_WARN | Debug_ERROR);
    debug.begin(115200);
    debug.print("\n");
    debug.printlnInfo("Starting...");

    // Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
    // Serial.print("\n");
    // Serial.println("Starting...");

    // uint32_t flash_size = ESP.getFlashChipSize();
  
    // Serial.print("Flash size: ");
    // Serial.print(flash_size);
    // Serial.println(" bytes");
    
    nextion.begin(115200);


//--------------------------------------------------------- SPIFFS

    // if(!SPIFFS.begin(true)) 
    // {
    //     Serial.println("ERREUR: SPIFFS Failed to begin.");
    //     return;
    // }

    // File root = SPIFFS.open("/");
    // File file = root.openNextFile();

    // while (file)
    // {   
    //     Serial.printf("File: %s.\n", file.name());
    //     file.close();
    //     file = root.openNextFile();
    // }

//--------------------------------------------------------- SD

    while (!SD.begin()) {
        debug.println("SD.begin failed");
        delay(1000);
    }

    if(!SD.exists("SystemData"))
    {
        SD.mkdir("SystemData");
    }

    userDataManager.Load();


//--------------------------------------------------------- WIFI

    // Serial.print("Tentative de connection WIFI.");

    // WiFi.disconnect(true);      
    // esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)Identity, strlen(Identity));
    // esp_wifi_sta_wpa2_ent_set_username((uint8_t *)Identity, strlen(Identity));
    // esp_wifi_sta_wpa2_ent_set_password((uint8_t *)PASSWORD, strlen(PASSWORD));
    // esp_wifi_sta_wpa2_ent_enable();
    // // WPA2 enterprise magic ends here


    // WiFi.begin(SSID);


//             SSID, WPA2_AUTH_... , IDENTITY, UserName, Password, ca_pem , client _crt, client_key, channel, bssid, connect
    //WiFi.begin(SSID, WPA2_AUTH_PEAP, Identity, Identity, PASSWORD, nullptr, nullptr, nullptr, 0, nullptr);
    // WiFi.begin(SSID, PWD);
    // WiFi.begin(SSID, WPA2_AUTH_TTLS, ID, ID, PWD);

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     if(WiFi.status() == WL_CONNECT_FAILED) {
    //         Serial.printf("Failed to connect to wifi\n");
    //         return;
    //     }
    //     Serial.print(".");
    //     delay(100);
    // }
    // Serial.println("\nConnection établie!");
    // Serial.printf("Adresse IP: %s.\n", WiFi.localIP().toString());
    

//--------------------------------------------------------- SERVER

    // audioManager.SetSourceAndOutput(&webRadioSource, &i2sOutput, true);
    // audioManager.SetSourceAndOutput(&spiffsSource, &i2sOutput, true);

    // Setup_Server();

    // audioManager.setLoopMode(AUDIO_LOOP_MODE_PLAYLIST);

    pAudioOutput *audioOutput = audioManager.getAudioOutput(userDataManager.getLastSelectedOutput());
    pAudioSource *audioSource = audioManager.getAudioSource(userDataManager.getLastSelectedSource());

    if(audioOutput == nullptr) audioOutput = &i2sOutput;
    if(audioSource == nullptr) audioSource = &bluetoothAudioSource;

    audioManager.setSourceAndOutput(audioSource, audioOutput, true);
    // audioManager.SetSourceAndOutput(&sdSource, &i2sOutput, true);
    // audioManager.SetSourceAndOutput(&webRadioSource, &i2sOutput, true);

    debug.printlnInfo("Portal Radio started!");
}

void loop() {
    audioManager.Loop();
    nextion.Loop();
}