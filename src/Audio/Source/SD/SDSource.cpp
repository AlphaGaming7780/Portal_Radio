#include "SDSource.h"
#include "../../AudioManager.h"

SDSource sdSource;

String SDSource::getID()
{
    return "SD card";
}

AudioSource &SDSource::getAudioSource()
{
    return source;
}

SDSource::SDSource()
{
}

SDSource::~SDSource()
{
}

// void SDSource::Setup()
// {
//     _decoder = audioManager.getDecoder(MP3);

//     _decoder->addNotifyAudioChange(audioManager.audioPlayer);

//     audioManager.audioPlayer.setAudioSource(source);
//     audioManager.audioPlayer.setDecoder(*_decoder);
    
//     // TO DO : Fix Metadata not working with SD card or find an alternative.
//     audioManager.audioPlayer.setMetadataCallback(UpdateMetaData);
// }

// void SDSource::setOutput(audio_tools::AudioOutput &output)
// {
//     audioManager.audioPlayer.setOutput(output);
// }

// void SDSource::setOutput(audio_tools::AudioStream &stream)
// {
//     audioManager.audioPlayer.setOutput(stream);
// }

// void SDSource::Begin()
// {
//     audioManager.audioPlayer.begin();
// }

// void SDSource::Loop()
// {
//     audioManager.audioPlayer.copy();
// }

// void SDSource::End()
// {
//     _decoder->removeNotifyAudioChange(audioManager.audioPlayer);
//     audioManager.audioPlayer.end();
// }

void SDSource::setPathAndPlay(char *path)
{
    source.setPath(path);
    source.begin();
}
