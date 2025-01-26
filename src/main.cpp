#include "main.h"

#include "T4B/ExtendedT4B.h"

// https://www.circuitstate.com/pinouts/doit-esp32-devkit-v1-wifi-development-board-pinout-diagram-and-reference/#ADC

void setup() {

    // LOGLEVEL_HELIX = LogLevelHelix::Info;
    debug.setDebugLevel(Debug_INFO | Debug_WARN | Debug_ERROR);
    // AudioLogger::instance().begin(Serial, AudioLogger::Info);
    debug.begin(115200);
    debug.printlnInfo("Starting...");

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
    // ec11.setInvertDirection(true);

    pAudioOutput *audioOutput = audioManager.getAudioOutput(userDataManager.getLastSelectedOutput());
    pAudioSource *audioSource = audioManager.getAudioSource(userDataManager.getLastSelectedSource());

    if(audioOutput == nullptr) audioOutput = &i2sOutput;
    // if(audioSource == nullptr) audioSource = &bluetoothAudioSource;

    audioManager.setSourceAndOutput(audioSource, audioOutput, true);

    // audioManager.SetSourceAndOutput(&sdSource, &i2sOutput, true);
    // audioManager.SetSourceAndOutput(&webRadioSource, &i2sOutput, true);

    // audioManager.setLoopMode(AUDIO_LOOP_MODE_PLAYLIST);

    
    if(!userDataManager.getMute()) { 
        audioManager.UnMute();
        nextion.setMute(false);
    }

    // t4b.DabSearch();

    nextion.StartupFinished();

    alarmManager.Begin();

    debug.printlnInfo("Portal Radio started!");
}


void loop() {
    ec11.Loop();
    t4b.Loop();
    audioManager.Loop();
    nextion.Loop();
    alarmManager.Loop();
}