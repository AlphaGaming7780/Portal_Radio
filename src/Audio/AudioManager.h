#include "pAudioSource.h"
#include "Output/I2SOutput/I2SOutput.h"
#include "Source/Bluetooth/BluetoothAudioSource.h"
// #include "Source/SD/SDSource.h"
// #include "Source/SPIFFSSource/SPIFFSSource.h"
#include "Source/WebRadioSource/WebRadioSource.h"

// #ifndef dAudioManager
// #define dAudioManager

class AudioManager
{
private:

    LinearVolumeControl _volumeControl;

    pAudioSource *_pendingSource = nullptr;
    pAudioOutput *_pendingOutput = nullptr;

    pAudioSource *_currentSource = nullptr;
    pAudioOutput *_currentOutput = nullptr;

    const int _volumePin = A6;


    // Dictionary<String, pAudioSource> sourceDict;

public:
    AudioManager(/* args */);
    ~AudioManager();

    pAudioSource *sourceList[2] {&bluetoothAudioSource, &webRadioSource};
    pAudioOutput *outputList[1] {&i2sOutput};

    AudioPlayer audioPlayer;

    bool useEquilizer = false;

    pAudioSource *currentSource = _currentSource;
    pAudioOutput *currentOutput = _currentOutput;

    void SetAudioSource(pAudioSource *audioSource, bool update = false);
    void SetAudioOutput(pAudioOutput *audioOutput, bool update = false);
    void SetSourceAndOutput(pAudioSource *audioSource, pAudioOutput *audioOutput, bool update = false);
    void Update();
    void loop();
    void end();
    void updateVolume();

};

extern AudioManager audioManager;
// #endif