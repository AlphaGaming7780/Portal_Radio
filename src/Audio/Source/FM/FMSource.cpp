#include "FMSource.h"
#include "T4B/T4B.h"
#include "../../I2SManager.h"
#include "Audio/AudioManager.h"
#include "Nextion/Nextion.h"
#include <DataManager/UserDataManager.h>

FMSource FM;

FMSource::FMSource()
{
}

FMSource::~FMSource()
{
}

String FMSource::getID()
{
    return "FM";
}

void FMSource::Setup()
{
    
}

void FMSource::setOutput(audio_tools::AudioOutput &output)
{
    
}

void FMSource::setOutput(audio_tools::AudioStream &StreamBufferDef_t)
{
    audioManager.streamCopie.begin(I2SManager::_stream, StreamBufferDef_t);
}

void FMSource::Begin()
{
    !t4b.PlayFm(104200U);
    audioManager.CreateStreamCopierTask();
}

void FMSource::End()
{
    audioManager.DeleteStreamCopierTask();
    t4b.Stop();
}

void FMSource::Play()
{
}

void FMSource::Pause()
{
}

void FMSource::Next()
{
}

void FMSource::Previous()
{
}

float FMSource::volumeInc()
{
    return 0.0625f;
}

void FMSource::setVolume(float volume)
{
    uint8_t volumeEnd = roundf(volume * 16);
    Serial.println(volumeEnd);
    t4b.setVolume(volumeEnd);
    volume = roundf( roundf(volumeEnd/16.0 * 100) / 5 ) * 5 / 100.0;
    userDataManager.setVolume(volume);
    nextion.setVolume(volume * 100, audioManager.isMuted());
}

float FMSource::getVolume()
{
    uint8_t vol = 0;
    t4b.getVolume(&vol);
    float volume = roundf( roundf(vol/16.0 * 100) / 5 ) * 5 / 100.0;
    return volume;
}
