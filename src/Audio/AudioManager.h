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

    const int _volumePin = A6;
    const int _mutePin = 32;

    AUDIO_LOOP_MODE _audioLoopMode = AUDIO_LOOP_MODE_NONE;

    bool _isMuted = false;

public:
    AudioManager(/* args */);
    ~AudioManager();

    pAudioSource *sourceList[3] {&webRadioSource, &sdSource, &bluetoothAudioSource};
    pAudioOutput *outputList[1] {&i2sOutput};

    AudioPlayer audioPlayer;

    bool useEquilizer = false;

    pAudioSource *getCurrentSource() { return _currentSource; };
    pAudioOutput *getCurrentOutput() { return _currentOutput; };

    void SetAudioSource(pAudioSource *audioSource, bool update = false);
    void SetAudioOutput(pAudioOutput *audioOutput, bool update = false);
    void SetSourceAndOutput(pAudioSource *audioSource, pAudioOutput *audioOutput, bool update = false);
    void Update();
    void loop();
    void end();
    void updateVolume();
    void play() {_currentSource->play(); };
    void pause() {_currentSource->pause(); };
    void next() {_currentSource->next(); };
    void previous() {_currentSource->previous(); };
    void mute();
    void unmute();
    bool isMuted() {return _isMuted; };

    void setLoopMode(AUDIO_LOOP_MODE mode);

    AudioDecoder *getDecoder(AUDIO_CODEC codec);

};

extern AudioManager audioManager;
#endif
