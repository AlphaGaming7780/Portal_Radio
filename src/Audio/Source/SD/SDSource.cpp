#include "SDSource.h"
#include "../../AudioManager.h"

SDSource sdSource;

void SDSource::_begin()
{
    audioManager.audioPlayer.setAudioSource(sd);
    audioManager.audioPlayer.begin();
}

SDSource::SDSource()
{
}

SDSource::~SDSource()
{
}

void SDSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(output);
    pAudioOutput->begin();
    _begin();
}

void SDSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(stream);
    pAudioOutput->begin();
    _begin();
}

void SDSource::end()
{
    // audioPlayer.end();
    audioManager.audioPlayer.end();
}