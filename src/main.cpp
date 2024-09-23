#include "Arduino.h"
#include "main.h"
#include "Audio/AudioManager.h"
#include "AudioConfig.h"

// #include "ESP8266.h"

// ESP8266 Esp8266(Serial2);

// const char* SSID = "eduroam";
// const char* ID = "loic.trioen@std.heh.be";
// const char* PWD = "Loicx22heh22loicx";

// const char* SSID = "S24+ Triton";
// const char* PWD = "axyktb87k2rxbp2";

const char* SSID = "C chez toi ici";
const char* PWD = "DecoP9DecoP9";

void setup() {

    Serial.begin(115200);

    Serial.print("\n");
    Serial.println("Starting...");

//--------------------------------------------------------- SPIFFS

    if(!SPIFFS.begin(true)) 
    {
        Serial.println("ERREUR: SPIFFS Failed to begin.");
        return;
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file)
    {   
        Serial.printf("File: %s.\n", file.name());
        file.close();
        file = root.openNextFile();
    }  

//--------------------------------------------------------- BLUETOOTH

    audioManager.SetSourceAndOutput(&bluetoothAudioSource, &i2SOutput, true);
    // audioManager.SetSourceAndOutput(&sdSource, &i2SOutput, true);

//--------------------------------------------------------- WIFI

    
    // Serial.print("Tentative de connection.");

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

    // char	*ipAddress, ap[31];
    // Esp8266.begin();
    // WIFI_ERR err = Esp8266.setup( WIFI_CONN_SERVER, WIFI_PRO_TCP, WIFI_MUX_MULTI);
    // if(err != WIFI_ERR_OK  ) 
    // {
    //     Serial.printf("\nWiFi setup fail error code : %i.\n", (int)err);
    //     return;
    // }

    // if (Esp8266.join(SSID, PWD) == WIFI_ERR_OK) {
    //     ipAddress = Esp8266.ip(ESP2866_WIFI_MODE_STA);
	// 	Serial.print(F("\n\rIP address: "));
	// 	Serial.print(ipAddress);
	// 	Serial.print(" : ");  
	// 	if (Esp8266.isConnect(ap))
	// 		Serial.println(ap);
	// 	else
	// 		Serial.println(" not found");
	// } else {
	// 	Serial.println(F("connect fail"));
	// 	while (1);
	// }

    // Serial.println("\nConnection établie!");
    // Serial.printf("Adresse IP: %s.\n", ipAddress);
    

//--------------------------------------------------------- SERVER

    // Setup_Server();

    Serial.println("Portal Radio started!");
}

void loop() {

}