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
    if(lastFmFreq < FmFrequencyMin) lastFmFreq = FmFrequencyMin;
    else if(lastFmFreq > FmFrequencyMax) lastFmFreq = FmFrequencyMax;
    t4b.PlayFm(lastFmFreq);
    audioManager.CreateStreamCopierTask();
    nextion.setTitle("");
    nextion.setArtist("");
    nextion.setFmFreq(lastFmFreq);
}

void FMSource::End()
{
    audioManager.DeleteStreamCopierTask();
    audioManager.streamCopie.end();
    t4b.Stop();
    I2SManager::_streamIn.end();
}

// void FMSource::StaticSetFreq(uint32_t freq)
// {
//     FMSource *Fm;
//     if(!audioManager.TryGetCurrentSourceAs<FMSource>(Fm)) return;
//     Fm->setFreq(freq);
// }

void FMSource::setFreq(uint32_t freq)
{
    if(!t4b.PlayFm(freq)) {
        t4b.getPlayIndex(&freq);
    } else {
        userDataManager.setLastFmFreq(freq);
        userDataManager.Save();
    }
    nextion.setFmFreq(freq);
}

uint32_t FMSource::getFreq()
{
    uint32_t freq = 0;
    if(!t4b.getPlayIndex(&freq)) return 0;
    return freq;
}

// void FMSource::StaticPlayPreset(uint8_t presetId)
// {
//     FMSource *Fm;
//     if(!audioManager.TryGetCurrentSourceAs<FMSource>(Fm)) return;
//     Fm->playPreset(presetId);
// }

void FMSource::playPreset(uint8_t presetId)
{
    uint32_t freq;
    if(!t4b.getPresetFM(presetId, &freq)) return;
    setFreq(freq);
}

// void FMSource::StaticSetPreset(uint8_t presetId)
// {
//     FMSource *Fm;
//     if(!audioManager.TryGetCurrentSourceAs<FMSource>(Fm)) return;
//     Fm->setPreset(presetId);
// }

void FMSource::setPreset(uint8_t presetId)
{
    uint32_t freq;
    if(!isCurrentSource()) return;
    if(!t4b.getPlayIndex(&freq)) return;
    if(!t4b.setPresetFM(presetId, freq)) return;
    nextion.SendFmPresets();
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
