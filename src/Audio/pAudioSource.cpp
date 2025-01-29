#include "pAudioSource.h"
#include "AudioManager.h"
#include "Nextion/Nextion.h"

pAudioSource::pAudioSource(/* args */)
{
}

pAudioSource::~pAudioSource()
{
}

void pAudioSource::Begin(pAudioOutput *pAudioOutput)
{
    debug.printlnInfo("Begin audio source.");
    pAudioOutput->Begin();
    Setup();

    if(audioManager.useEquilizer) {
        setOutput(pAudioOutput->getEquilizerStream());
    } else {
        switch (pAudioOutput->getOutputType())
        {
        case OutputType:
            setOutput(pAudioOutput->getAudioOutput());
            break;
        case StreamType:
            setOutput(pAudioOutput->getAudioStream());
            break;
        default:
            Serial.printf("pAudioSource::begin : Unknown audio type : %i.\n", pAudioOutput->getOutputType());
            break;
        }
    }
    
    Begin();
}

void pAudioSource::Setup()
{
    _decoder = audioManager.getDecoder(MP3);

    _decoder->addNotifyAudioChange(audioManager.audioPlayer);

    audioManager.audioPlayer.setAudioSource(getAudioSource());
    audioManager.audioPlayer.setDecoder(*_decoder);
    
    // TO DO : Fix Metadata not working with SD card or find an alternative.
    audioManager.audioPlayer.setMetadataCallback(UpdateMetaData);
}

void pAudioSource::setOutput(audio_tools::AudioOutput &output)
{
    audioManager.audioPlayer.setOutput(output);
}

void pAudioSource::setOutput(audio_tools::AudioStream &stream)
{
    audioManager.audioPlayer.setOutput(stream);
}

void pAudioSource::Begin()
{
    audioManager.audioPlayer.begin();
    audioManager.CreateAudioPlayerTask();

}

void pAudioSource::End()
{
    audioManager.DeleteAudioPlayerTask();
    audioManager.audioPlayer.end();
    _decoder->removeNotifyAudioChange(audioManager.audioPlayer);
}

float pAudioSource::volumeInc()
{
    return 0.025f;
}

void pAudioSource::setVolume(float volume)
{
    audioManager.audioPlayer.setVolume(volume);
    nextion.setVolume(volume * 100, audioManager.isMuted());
}
void pAudioSource::Play() { audioManager.audioPlayer.play(); }
void pAudioSource::Pause() { audioManager.audioPlayer.stop(); }
void pAudioSource::Next() { audioManager.audioPlayer.next(); }
void pAudioSource::Previous() { audioManager.audioPlayer.previous(); }
float pAudioSource::getVolume() { return audioManager.audioPlayer.volume(); }

bool pAudioSource::isCurrentSource()
{
    return audioManager.getCurrentSource()->getID() == getID();
}

void PrintMetaData(MetaDataType type, const char *str, int len)
{
    debug.print("==> ");
    debug.print(toStr(type));
    debug.print(": ");
    debug.println(str);
}

void UpdateMetaData(MetaDataType type, const char *str, int len)
{
    PrintMetaData(type, str, len);
    switch (type)
    {
    case Title:
        nextion.setTitle(str);
        break;
    case Artist:
        nextion.setArtist(str);
        break;
    case Name:
        nextion.setTitle(str);
        break;
    case Description:
        nextion.setArtist(str);
        break;
    case Album:
        nextion.setAlbum(str);
        break;
    case Genre:
        nextion.setGenre(str);
        break;

    default:
        break;
    }
}
