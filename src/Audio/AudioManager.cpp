#include "AudioManager.h"

AudioManager audioManager;

AudioManager::AudioManager(/* args */)
{
    audioPlayer.setVolumeControl(_volumeControl);
}

AudioManager::~AudioManager()
{
}

void AudioManager::SetAudioSource(pAudioSource *audioSource, bool update)
{
    _pendingSource = audioSource;
    if(update) Update();   
}

void AudioManager::SetAudioOutput(pAudioOutput *audioOutput, bool update)
{
    _pendingOutput = audioOutput;
    if(update) Update();
}

void AudioManager::SetSourceAndOutput(pAudioSource *audioSource, pAudioOutput *audioOutput, bool update)
{
    _pendingSource = audioSource;
    _pendingOutput = audioOutput;
    if(update) Update();
}

void AudioManager::Update()
{
    if( _pendingSource == nullptr && _pendingOutput == nullptr) return;

    if(_currentSource != nullptr) _currentSource->end();
    if(_currentOutput != nullptr) _currentOutput->end();
    if(_pendingSource != nullptr) _currentSource = _pendingSource;
    if(_pendingOutput != nullptr) _currentOutput = _pendingOutput;
    _currentSource->begin(_currentOutput);
    updateVolume();
}

void AudioManager::loop()
{   
    if(_currentSource != nullptr) {
        updateVolume();
        _currentSource->loop();
    }
}

void AudioManager::end()
{
    if(_currentSource != nullptr) _currentSource->end();
    if(_currentOutput != nullptr) _currentOutput->end();
    _currentOutput = nullptr;
    _currentSource = nullptr;
}

void AudioManager::updateVolume() {
    // Reading potentiometer value (range is 0 - 4095)
    float vol = static_cast<float>(analogRead(_volumePin));
    _currentSource->updateVolume(vol/4065.0);
}
