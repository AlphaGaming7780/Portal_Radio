#include "AudioManager.h"
#include "Nextion/Nextion.h"

AudioManager audioManager;

AudioManager::AudioManager(/* args */)
{
    audioPlayer.setVolumeControl(_volumeControl);
    pinMode(_mutePin, OUTPUT);
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
    debug.printlnInfo("AudioManager::Update : ending source / output finished.");
    if(_pendingSource != nullptr) _currentSource = _pendingSource;
    if(_pendingOutput != nullptr) _currentOutput = _pendingOutput;
    _currentSource->begin(_currentOutput);
    debug.printlnInfo("Updating the volume.");
    updateVolume();
    nextion.setAudioSource(_currentSource->getID());
}

void AudioManager::loop()
{   
    if( !_isMuted &&_currentSource != nullptr && audioPlayer.isActive()) {
        updateVolume();
        _currentSource->loop();
    }

    if(!audioPlayer.isActive()) {
        if(_audioLoopMode == AUDIO_LOOP_MODE_PLAYLIST) {
            audioPlayer.setIndex(0);
        } else if( _audioLoopMode == AUDIO_LOOP_MODE_TRACK) {
            audioPlayer.previous();
        }
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
    vol = roundf( roundf(vol/4095.0 * 100) / 5 ) * 5 / 100.0;

    if(_currentSource->getVolume() != vol) {
        Serial.printf("New Volume : %f.\n", vol);
        _currentSource->updateVolume(vol);
        // nextion.setVolume(vol * 100);
    }
}

void AudioManager::mute()
{
    digitalWrite(_mutePin, HIGH);
    _isMuted = true;
}

void AudioManager::unmute()
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
