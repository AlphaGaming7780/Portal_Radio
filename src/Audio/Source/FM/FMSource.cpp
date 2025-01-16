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
    I2SManager::BeginIn();
}

void FMSource::setOutput(audio_tools::AudioOutput &output)
{
    
}

void FMSource::setOutput(audio_tools::AudioStream &stream)
{
    _volumeStream.setStream(stream);
    _volumeStream.setVolumeControl(_volumeControl);
    auto vcfg = _volumeStream.defaultConfig();
    vcfg.copyFrom(stream.audioInfo());
    _volumeStream.begin(vcfg);
    audioManager.streamCopie.begin(_volumeStream, I2SManager::_streamIn );
}

void FMSource::Begin()
{
    uint32_t lastFmFreq = userDataManager.getLastFmFreq();
    if(lastFmFreq < 87500) lastFmFreq = 87500;
    else if(lastFmFreq > 108000) lastFmFreq = 108000;
    t4b.PlayFm(lastFmFreq);
    audioManager.CreateStreamCopierTask();
    nextion.setTitle("");
    nextion.setArtist("");
}

void FMSource::End()
{
    audioManager.DeleteStreamCopierTask();
    audioManager.streamCopie.end();
    t4b.Stop();
    I2SManager::_streamIn.end();
}

void FMSource::setFreq(uint32_t freq)
{
    if(!t4b.PlayFm(freq)) return;
    userDataManager.setLastFmFreq(freq);
    userDataManager.Save();
}

void FMSource::Next()
{
    t4b.FmSearch(true);
    nextion.setTitle("");
    nextion.setArtist("");
}

void FMSource::Previous()
{
    t4b.FmSearch(false);
    nextion.setTitle("");
    nextion.setArtist("");
}

// float FMSource::volumeInc()
// {
//     return 0.0625f;
// }

void FMSource::setVolume(float volume)
{
    _volumeStream.setVolume(volume);
    nextion.setVolume(volume * 100, audioManager.isMuted());
}

float FMSource::getVolume()
{   
    return _volumeStream.volume();
}
