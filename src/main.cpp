#include "main.h"

#include "T4B/T4B.h"

// https://www.circuitstate.com/pinouts/doit-esp32-devkit-v1-wifi-development-board-pinout-diagram-and-reference/#ADC

void setup() {

    // LOGLEVEL_HELIX = LogLevelHelix::Info;
    debug.setDebugLevel(Debug_INFO | Debug_WARN | Debug_ERROR);
    // AudioLogger::instance().begin(Serial, AudioLogger::Info);
    debug.begin(115200);
    debug.print("\n");
    debug.printlnInfo("Starting...");

    ec11.Begin();

    t4b.Init();

    if(!t4b.setVolume(16)) {Serial.println(ToString(t4b.getError()));}

    // Serial.println("FM!!!!!!!!!");
    
    // if(!t4b.PlayFm(104200)) {Serial.println(ToString(t4b.getError()));}
    // if(!t4b.FmSearch()) {Serial.println(ToString(t4b.getError()));}

    // if(!t4b.PlayDab(1)) {Serial.println(ToString(t4b.getError()));}

    // if(!t4b.PlayeSingleTone(1)) {Serial.println(ToString(t4b.getError()));}
    
    // while(true) {}

    Serial.println("Starting Nextion...");
    nextion.Begin(115200);


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

    Serial.println("Starting SD...");
    while (!SD.begin()) {
        debug.println("SD.begin failed");
        delay(1000);
    }

    if(!SD.exists("SystemData"))
    {
        SD.mkdir("SystemData");
    }

    userDataManager.Load();

    ec11.setInvertDirection(userDataManager.getEC11InvertDirection());
    // ec11.setInvertDirection(true);

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
  
    nextion.StartupFinished();
    
    alarmManager.Begin();

    pAudioOutput *audioOutput = audioManager.getAudioOutput(userDataManager.getLastSelectedOutput());
    pAudioSource *audioSource = audioManager.getAudioSource(userDataManager.getLastSelectedSource());

    if(audioOutput == nullptr) audioOutput = &i2sOutput;
    // if(audioSource == nullptr) audioSource = &bluetoothAudioSource;

    audioManager.setSourceAndOutput(audioSource, audioOutput, true);

    if(!t4b.PlayFm(104200U)) {Serial.println(ToString(t4b.getError()));}
    // if(!t4b.setHeadroom(12U)) {Serial.println(ToString(t4b.getError()));}
    uint8_t headroomLevel;
    if(t4b.getHeadroom(&headroomLevel)) {Serial.printf("Headroom Level : %u\n", headroomLevel);}


    // audioManager.SetSourceAndOutput(&sdSource, &i2sOutput, true);
    // audioManager.SetSourceAndOutput(&webRadioSource, &i2sOutput, true);

    // audioManager.setLoopMode(AUDIO_LOOP_MODE_PLAYLIST);

    debug.printlnInfo("Portal Radio started!");
}


void loop() {
    ec11.Loop();
    audioManager.Loop();
    nextion.Loop();
    alarmManager.Loop();
}