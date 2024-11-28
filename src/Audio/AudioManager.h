#pragma once
#include "pAudioSource.h"
#include "Output/I2SOutput/I2SOutput.h"
#include "Source/Bluetooth/BluetoothAudioSource.h"
#include "Source/SD/SDSource.h"
#include "Source/SPIFFSSource/SPIFFSSource.h"
#include "Source/WebRadioSource/WebRadioSource.h"

#ifndef dAudioManager
#define dAudioManager

typedef enum {
	AUDIO_LOOP_MODE_NONE = 0,   // Play the playlist and stop at the end of it
	AUDIO_LOOP_MODE_PLAYLIST,   // Play the playlist and restart at the end of it
	AUDIO_LOOP_MODE_TRACK,      // Play the current track and restart it when finished.
} AUDIO_LOOP_MODE;

typedef enum {
	MP3 = 0,
} AUDIO_CODEC;

class AudioManager
{
private:

    MP3DecoderHelix _decoderMP3;

    LinearVolumeControl _volumeControl;

    pAudioSource *_pendingSource = nullptr;
    pAudioOutput *_pendingOutput = nullptr;

    pAudioSource *_currentSource = nullptr;
    pAudioOutput *_currentOutput = nullptr;

    const int _mutePin = 33;

    AUDIO_LOOP_MODE _audioLoopMode = AUDIO_LOOP_MODE_NONE;

    bool _useAudioPlayer = false;

    bool _isMuted = false;
    bool _isPaused = false;

    TaskHandle_t audioPlayerLoopTask;

public:
    AudioManager(/* args */);
    ~AudioManager();

    pAudioSource *sourceList[4] { &bluetoothAudioSource, &sdSource, &webRadioSource, &spiffsSource };
    pAudioOutput *outputList[1] { &i2sOutput };

    AudioPlayer audioPlayer;

    bool useEquilizer = false;

    pAudioSource *getAudioSource(const char* s);
    pAudioOutput *getAudioOutput(const char* s);

    pAudioSource *getCurrentSource() { return _currentSource; };
    pAudioOutput *getCurrentOutput() { return _currentOutput; };

    void setAudioSource(pAudioSource *audioSource, bool update = false);
    void setAudioOutput(pAudioOutput *audioOutput, bool update = false);
    void setSourceAndOutput(pAudioSource *audioSource, pAudioOutput *audioOutput, bool update = false);
    void Update();
    void Loop();
    void End();
    void UpdateVolume();
    void Play()     { if(_currentSource == nullptr) return; _currentSource->Play(); _isPaused = false; };
    void Pause()    { if(_currentSource == nullptr) return; _currentSource->Pause(); _isPaused = true; };
    void Next()     { if(_currentSource == nullptr) return; _currentSource->Next(); };
    void Previous() { if(_currentSource == nullptr) return; _currentSource->Previous(); };
    void Mute();
    void UnMute();
    bool isMuted() {return _isMuted; };

    void setLoopMode(AUDIO_LOOP_MODE mode);
    
    void CreateAudioPlayerTask();
    void DeleteAudioPlayerTask();

    AudioDecoder *getDecoder(AUDIO_CODEC codec);

};

void AudioPlayerTLoopTask(void * parameter);

extern AudioManager audioManager;
#endif
