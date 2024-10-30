#include "AudioManager.h"
#include "Nextion/Nextion.h"
#include "DataManager/UserDataManager.h"

AudioManager audioManager;

AudioManager::AudioManager(/* args */)
{
    audioPlayer.setVolumeControl(_volumeControl);
    pinMode(_mutePin, OUTPUT);
}

AudioManager::~AudioManager()
{
}

pAudioSource *AudioManager::getAudioSource(const char *s)
{
    for (auto &&i : sourceList)
    {
        if(i->getID() == s) return i;
    }
    return nullptr;
}

pAudioOutput *AudioManager::getAudioOutput(const char *s)
{
    for (auto &&i : outputList)
    {
        if(i->getID() == s) return i;
    }
    return nullptr;
}

void AudioManager::setAudioSource(pAudioSource *audioSource, bool update)
{
    _pendingSource = audioSource;
    if(update) Update();   
}

void AudioManager::setAudioOutput(pAudioOutput *audioOutput, bool update)
{
    _pendingOutput = audioOutput;
    if(update) Update();
}

void AudioManager::setSourceAndOutput(pAudioSource *audioSource, pAudioOutput *audioOutput, bool update)
{
    _pendingSource = audioSource;
    _pendingOutput = audioOutput;
    if(update) Update();
}

void AudioManager::Update()
{
    if( _pendingSource == nullptr && _pendingOutput == nullptr) return;

    if(_currentSource != nullptr) _currentSource->End();
    if(_currentOutput != nullptr) _currentOutput->End();
    debug.printlnInfo("AudioManager::Update : ending source / output finished.");
    if(_pendingSource != nullptr) _currentSource = _pendingSource;
    if(_pendingOutput != nullptr) _currentOutput = _pendingOutput;
    _currentSource->Begin(_currentOutput);
    debug.printlnInfo("Updating the volume.");
    UpdateVolume();
    nextion.setAudioSource(_currentSource->getID());
    userDataManager.setLastSelectedOutput(_currentOutput->getID());
    userDataManager.setLastSelectedSource(_currentSource->getID());
    userDataManager.Save();
}

void AudioManager::Loop()
{   
    if( !_isMuted &&_currentSource != nullptr && audioPlayer.isActive()) {
        UpdateVolume();
        _currentSource->Loop();
    }

    if(!audioPlayer.isActive()) {
        if(_audioLoopMode == AUDIO_LOOP_MODE_PLAYLIST) {
            audioPlayer.setIndex(0);
        } else if( _audioLoopMode == AUDIO_LOOP_MODE_TRACK) {
            audioPlayer.previous();
        }
    }
}

void AudioManager::End()
{
    if(_currentSource != nullptr) _currentSource->End();
    if(_currentOutput != nullptr) _currentOutput->End();
    _currentOutput = nullptr;
    _currentSource = nullptr;
}

void AudioManager::UpdateVolume() {
    // Reading potentiometer value (range is 0 - 4095)
    float vol = static_cast<float>(analogRead(_volumePin));
    vol = roundf( roundf(vol/4095.0 * 100) / 5 ) * 5 / 100.0;

    if(_currentSource->getVolume() != vol) {
        Serial.printf("New Volume : %f.\n", vol);
        _currentSource->UpdateVolume(vol);
        // nextion.setVolume(vol * 100);
    }
}

void AudioManager::Mute()
{
    digitalWrite(_mutePin, HIGH);
    _isMuted = true;
}

void AudioManager::UnMute()
{
    digitalWrite(_mutePin, LOW);
    _isMuted = false;
}

void AudioManager::setLoopMode(AUDIO_LOOP_MODE mode)
{
    _audioLoopMode = mode;
    if(_audioLoopMode == AUDIO_LOOP_MODE_TRACK) {
        audioPlayer.setAutoNext(false);
    } else {
        audioPlayer.setAutoNext(true);
    }
}

AudioDecoder *AudioManager::getDecoder(AUDIO_CODEC codec)
{
    switch (codec)
    {
    case MP3:
        return &_decoderMP3;
        break;
    
    default:
        return nullptr;
        break;
    }
    return nullptr;
}
