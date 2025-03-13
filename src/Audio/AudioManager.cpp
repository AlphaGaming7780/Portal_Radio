#include "AudioManager.h"
#include "Nextion/Nextion.h"
#include "DataManager/UserDataManager.h"
#include "EC11/EC11.h"

AudioManager audioManager;

AudioManager::AudioManager(/* args */)
{
    audioPlayer.setVolumeControl(_volumeControl);
    pinMode(_mutePin, OUTPUT);
    Mute(false);
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

    // if(s == "Bluetooth") {
    //     return new BluetoothAudioSource();
    // }

    // if( s == "DAB" ) {
    //     return new DABSource();
    // }

    // if( s == "FM" ) {
    //     return new FMSource();
    // }

    // if( s == "SD card" ) {
    //     return new SDSource();
    // }
        
    // return nullptr;
}

pAudioOutput *AudioManager::getAudioOutput(const char *s)
{
    // for (auto &&i : outputList)
    // {
    //     if(i->getID() == s) return i;
    // }
    // return nullptr;

    if(s == "Speaker") {
        return new I2SOutput();
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

    bool oldIsMuted = _isMuted;

    // Reset source in case we get stuck during the process of disabling on of them (SD) so it actually unstuck the user
    userDataManager.setLastSelectedSource("NULL");
    userDataManager.Save();

    nextion.setTitle("");
    nextion.setArtist("");
    nextion.setGenre("");
    nextion.setAlbum("");

    if(!oldIsMuted) Mute(false);

    if(_currentSource != nullptr) 
    {
        debug.printlnInfo("Ending audio source.");
        _currentSource->End();
        // if(_currentSource != _pendingSource) delete _currentSource;
    }

    if(_currentOutput != nullptr) 
    {
        debug.printlnInfo("Ending audio output.");
        _currentOutput->End();
        if(_currentOutput != _pendingOutput) delete _currentOutput;
    }

    _currentSource = _pendingSource;
    _currentOutput = _pendingOutput;

    if(_currentSource != nullptr && _currentOutput != nullptr) {
        debug.printlnInfo("Begin source.");
        _currentSource->Begin(_currentOutput);
        debug.printlnInfo("Updating the volume.");
        _currentSource->setVolume(userDataManager.getVolume());

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

    if(!oldIsMuted) UnMute(false);
}

void AudioManager::Loop()
{   

    if( _useAudioPlayer || _useStreamCopier) {
        
        // if(!_isMuted) 
        // note needed since we use interrupt now on the EC11, wer still need, because saving user data cause crash, maybe as to change the way
        UpdateVolume();

        if(!_isPaused && !audioPlayer.isActive() && _useAudioPlayer) {
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
    if(_useAudioPlayer) DeleteAudioPlayerTask();
    if(_useStreamCopier) DeleteStreamCopierTask();

    _currentOutput = nullptr;
    _currentSource = nullptr;
}

void AudioManager::UpdateVolume() {
    
    if(_currentSource == nullptr) return;

    float oldVol = _currentSource->getVolume();
    float savedVol = userDataManager.getVolume();
    if(oldVol != savedVol) oldVol = savedVol;
    float newVol = oldVol + ec11.getDirection() * _currentSource->volumeInc();

    if(newVol < 0) newVol = 0;
    else if( newVol > 1 ) newVol = 1;

    if( oldVol != newVol ) {
        Serial.printf("Old volume : %f, Saved Volume : %f, New Volume : %f.\n", oldVol, savedVol, newVol);
        userDataManager.setVolume(newVol);
        userDataManager.Save();
        _currentSource->setVolume(newVol);
    }
}

void AudioManager::Mute(bool save)
{
    digitalWrite(_mutePin, HIGH);
    _isMuted = true;
    if(save) {
        userDataManager.setMute(true);
        userDataManager.Save();
    }
    nextion.setMute(true);
}

void AudioManager::UnMute(bool save)
{
    digitalWrite(_mutePin, LOW);
    _isMuted = false;
    if(save) {
        userDataManager.setMute(false);
        userDataManager.Save();
    }
    nextion.setMute(false);
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
    if(audioPlayerLoopTask != NULL) DeleteAudioPlayerTask();

    _useAudioPlayer = true;

    xTaskCreatePinnedToCore(
        AudioPlayerLoopTask,     /* Function to implement the task */
        "AudioPlayerLoopTask",    /* Name of the task */
        10000,                    /* Stack size in words */
        (void*)&_useAudioPlayer,                     /* Task input parameter */
        255,                        /* Priority of the task */
        &audioPlayerLoopTask,     /* Task handle. */
        0                         // Core ID
    );

}

void AudioManager::DeleteAudioPlayerTask()
{
    _useAudioPlayer = false;

    if(audioPlayerLoopTask == NULL) {
        return;
    } 

    while (!_useAudioPlayer)
    {
        esp_task_wdt_reset();
    }
    
    _useAudioPlayer = false;

    vTaskDelete(audioPlayerLoopTask);

    audioPlayerLoopTask = NULL;
    
}

void AudioManager::CreateStreamCopierTask()
{
    if(streamCopierLoopTask != NULL) DeleteStreamCopierTask();

    _useStreamCopier = true;

    xTaskCreatePinnedToCore(
        StreamCopierLoopTask,     /* Function to implement the task */
        "StreamCopierLoopTask",    /* Name of the task */
        10000,                    /* Stack size in words */
        (void*)&_useStreamCopier,                     /* Task input parameter */
        255,                        /* Priority of the task */
        &streamCopierLoopTask,     /* Task handle. */
        0                         // Core ID
    );

}

void AudioManager::DeleteStreamCopierTask()
{
    _useStreamCopier = false;
    if(streamCopierLoopTask == NULL) {
        debug.printlnWarn("DeleteStreamCopierTask streamCopierLoopTask was null");
        return;
    }

    while (!_useStreamCopier)
    {
        esp_task_wdt_reset();
    }

    _useStreamCopier = false;

    vTaskDelete(streamCopierLoopTask);

    streamCopierLoopTask = NULL;
}

void AudioPlayerLoopTask(void *parameter)
{

    bool *value = ((bool*)parameter);

    while (*value)
    {
        audioManager.audioPlayer.copy();
        esp_task_wdt_reset();
    }

    (*value) = true;

    while (true)
    {
        // Wait until this task get deleted by the main thread.
        esp_task_wdt_reset();
    }

}

void StreamCopierLoopTask(void *parameter)
{
    bool *value = ((bool*)parameter);

    while (*value)
    {
        audioManager.streamCopie.copy();
        esp_task_wdt_reset();
    }

    (*value) = true;

    while (true)
    {
        // Wait until this task get deleted by the main thread.
        esp_task_wdt_reset();
    }
}
