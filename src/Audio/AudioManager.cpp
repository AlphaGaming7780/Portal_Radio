#include "AudioManager.h"
#include "Nextion/Nextion.h"
#include "DataManager/UserDataManager.h"
#include "EC11/EC11.h"

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
    // if( _pendingSource == nullptr && _pendingOutput == nullptr) return;

    if(_currentSource != nullptr) 
    {
        debug.printlnInfo("Ending audio source.");
        _currentSource->End();
    }

    if(_currentOutput != nullptr) 
    {
        debug.printlnInfo("Ending audio output.");
        _currentOutput->End();
    }


    _currentSource = _pendingSource;
    _currentOutput = _pendingOutput;

    debug.printlnInfo("Begin source.");
    if(_currentSource != nullptr && _currentOutput != nullptr) {
        _currentSource->Begin(_currentOutput);
        debug.printlnInfo("Updating the volume.");
        UpdateVolume();

        nextion.setAudioSource(_currentSource->getID());
        nextion.setPlayStatus(true);
        _isPaused = false;
    } else {
        nextion.setAudioSource("");
        nextion.setPlayStatus(false);
        _isPaused = true;
    }

    if (_currentOutput != nullptr) userDataManager.setLastSelectedOutput(_currentOutput->getID());
    else userDataManager.setLastSelectedOutput("NULL");
    if (_currentSource != nullptr) userDataManager.setLastSelectedSource(_currentSource->getID());
    else userDataManager.setLastSelectedSource("NULL");
    userDataManager.Save();
}

void AudioManager::Loop()
{   

    if( _useAudioPlayer) {
        
        if(!_isMuted) UpdateVolume();

        if(!_isPaused && !audioPlayer.isActive()) {
            if(_audioLoopMode == AUDIO_LOOP_MODE_PLAYLIST) {
                audioPlayer.setIndex(0);
            } else if( _audioLoopMode == AUDIO_LOOP_MODE_TRACK) {
                audioPlayer.previous();
            }
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

    float vol = userDataManager.getVolume(); //_currentSource->getVolume();
    vol += ec11.getDirection() * 5/100.0;

    if(vol < 0) vol = 0;
    else if( vol > 1 ) vol = 1;

    if( _currentSource != nullptr && _currentSource->getVolume() != vol) {
        Serial.printf("New Volume : %f.\n", vol);
        userDataManager.setVolume(vol);
        _currentSource->UpdateVolume(vol);
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

void AudioManager::CreateAudioPlayerTask()
{
    if(audioPlayerLoopTask != NULL) return;

    xTaskCreatePinnedToCore(
        AudioPlayerTLoopTask,     /* Function to implement the task */
        "AudioPlayerLoopTask",    /* Name of the task */
        10000,                    /* Stack size in words */
        NULL,                     /* Task input parameter */
        255,                        /* Priority of the task */
        &audioPlayerLoopTask,     /* Task handle. */
        0                         // Core ID
    );

    _useAudioPlayer = true;

}

void AudioManager::DeleteAudioPlayerTask()
{
    if(audioPlayerLoopTask == NULL) return;

    vTaskDelete(audioPlayerLoopTask);

    audioPlayerLoopTask = NULL;
    _useAudioPlayer = false;
}

void AudioPlayerTLoopTask(void *parameter)
{
    while (true)
    {
        // debug.printlnInfo("AudioPlayerTask is running.");
        audioManager.audioPlayer.copy();
    }
}
