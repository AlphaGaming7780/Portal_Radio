#include "main.h"

#include "T4B/ExtendedT4B.h"

#define START_WDT_TIMEOUT 10
#define LOOP_WDT_TIMEOUT 3

void setup() {

    esp_task_wdt_init(START_WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch

    // LOGLEVEL_HELIX = LogLevelHelix::Info;
    debug.setDebugLevel(Debug_INFO | Debug_WARN | Debug_ERROR);
    // AudioLogger::instance().begin(Serial, AudioLogger::Info);
    debug.begin(115200);
    debug.printlnInfo("Starting...");
    // debug.printInfo("Executing on core : ");
    // Serial.println(xPortGetCoreID());
    // Serial.printf("CPU Frequency: %i MHz\n", getCpuFrequencyMhz());

    debug.printlnInfo("Starting Nextion...");
    nextion.Begin(115200);

    debug.printlnInfo("Starting EC11...");
    ec11.Begin();

    debug.printlnInfo("Starting T4B...");
    t4b.Setup();

    debug.printlnInfo("Starting SD...");
    while (!SD.begin()) {
        debug.printlnError("SD.begin failed");
        delay(1000);
    }

    if(!SD.exists("SystemData"))
    {
        SD.mkdir("SystemData");
    }

    debug.printlnInfo("Loading user data...");
    if(!userDataManager.Load()) debug.printlnError("Failed to load user data!");

    ec11.setInvertDirection(userDataManager.getEC11InvertDirection());

    alarmManager.Begin();

    pAudioOutput *audioOutput = audioManager.getAudioOutput(userDataManager.getLastSelectedOutput());
    pAudioSource *audioSource = audioManager.getAudioSource(userDataManager.getLastSelectedSource());

    // Force I2S Output since we don't have any other output type.
    if(audioOutput == nullptr) audioOutput = new I2SOutput();

    audioManager.setSourceAndOutput(audioSource, audioOutput, true);

    // audioManager.setLoopMode(AUDIO_LOOP_MODE_PLAYLIST);

    // t4b.DabSearch();

    nextion.StartupFinished();

    if(!userDataManager.getMute()) audioManager.UnMute(false);

    esp_task_wdt_init(LOOP_WDT_TIMEOUT, true);

    debug.printlnInfo("Portal Radio started!");
}

#ifdef dDebug
uint32_t oldFreeHeap = 0;
#endif

void loop() {

    esp_task_wdt_reset();
    #ifdef dDebug
    uint32_t freeHeap = esp_get_free_heap_size();
    if(freeHeap != oldFreeHeap) {
        debug.printInfo("ESP32 free heap : "); Serial.print(freeHeap); Serial.println("B.");
        oldFreeHeap = freeHeap;
    }
    #endif

    // ec11.Loop();
    t4b.Loop();
    audioManager.Loop();
    nextion.Loop();
    alarmManager.Loop();
}
