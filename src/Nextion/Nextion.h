#pragma once

#include "Arduino.h"
#include "Audio/AudioManager.h"
#include "pDebug/pDebug.h"
#include "Nextion/NextionPendingData.h"

class Nextion
{
public:
    Nextion() {};
    Nextion(HardwareSerial *serial);
    ~Nextion();

    bool init(HardwareSerial *serial);
    bool begin(ulong baudrate = 9600);

    void StartupFinished();

    void Loop();

    void setAudioSource(String source);
    void setVolume(int volume, bool isMuted);
    void setTitle(String title);
    void setArtist(String artist);
    void setPlayStatus(bool playStatus);

    void Sleep(bool sleep);
    void Reset();

private:
    HardwareSerial *_serial;
    ulong _baudrate;

    String _endChar = String(char(0xFF))+String(char(0xFF))+String(char(0xFF));

    bool _isSleeping = false;
    bool _isReady = false;

    NextionPendingData _pendingData;

    void UpdatePendingAudioSource(String source);
    String formatHexCodeToString(uint8_t value);

    void UpdatePendingData() {
        
        if(_pendingData.audioSource != emptyString)   { setAudioSource(_pendingData.audioSource); _pendingData.audioSource = emptyString; }

        if(_pendingData.volume != -1 && _pendingData.isMuted != BOOL3_NULL) {
            setVolume(_pendingData.volume, _pendingData.isMuted);
            _pendingData.volume = -1;
            _pendingData.isMuted = BOOL3_NULL;
        }

        if(_pendingData.title != emptyString)   { setTitle(_pendingData.title); _pendingData.title = emptyString; }
        if(_pendingData.artist != emptyString)  { setArtist(_pendingData.artist); _pendingData.artist = emptyString; }
        if(_pendingData.playStatus != BOOL3_NULL)     { setPlayStatus(_pendingData.playStatus); _pendingData.playStatus = BOOL3_NULL; } 

    }

};

extern Nextion nextion;
