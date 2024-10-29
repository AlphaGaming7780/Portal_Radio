#include "SDSource.h"
#include "../../AudioManager.h"

SDSource sdSource;

String SDSource::getID()
{
    return "SD card";
}

SDSource::SDSource()
{
}

SDSource::~SDSource()
{
}

void SDSource::preBegin()
{
    _decoder = audioManager.getDecoder(MP3);

    _decoder->addNotifyAudioChange(audioManager.audioPlayer);

    audioManager.audioPlayer.setAudioSource(source);
    audioManager.audioPlayer.setDecoder(*_decoder);
    audioManager.audioPlayer.setMetadataCallback(updateMetaData);
}

void SDSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(output);
}

void SDSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(stream);
}

void SDSource::postBegin()
{
    audioManager.audioPlayer.begin();
}

void SDSource::loop()
{
    audioManager.audioPlayer.copy();
}

void SDSource::end()
{
    _decoder->removeNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.end();
}

void SDSource::setPathAndBegin(char *path)
{
    source.setPath(path);
    source.begin();
}
