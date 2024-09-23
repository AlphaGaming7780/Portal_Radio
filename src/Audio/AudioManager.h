#include "pAudioSource.h"
#include "Output/AnalogueOutput/AnalogueOutput.h"
#include "Output/I2SOutput/I2SOutput.h"
#include "Source/Bluetooth/BluetoothAudioSource.h"


class AudioManager
{
private:
    pAudioSource *_pendingSource = nullptr;
    pAudioOutput *_pendingOutput = nullptr;

    pAudioSource *_currentSource = nullptr;
    pAudioOutput *_currentOutput = nullptr;
public:
    AudioManager(/* args */);
    ~AudioManager();
    
    pAudioSource *currentSource = _currentSource;
    pAudioOutput *currentOutput = _currentOutput;

    void SetAudioSource(pAudioSource *audioSource, bool update = false);
    void SetAudioOutput(pAudioOutput *audioOutput, bool update = false);
    void SetSourceAndOutput(pAudioSource *audioSource, pAudioOutput *audioOutput, bool update = false);
    void Update();
};

extern AudioManager audioManager;