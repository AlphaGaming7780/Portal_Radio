#include "SPIFFSSource.h"
#include "../../AudioManager.h"

SPIFFSSource spiffsSource;

void SPIFFSSource::_begin()
{
    
    // audioManager.audioPlayer.setAudioSource(stream);
    // audioManager.audioPlayer.begin();
}

SPIFFSSource::SPIFFSSource()
{
}

SPIFFSSource::~SPIFFSSource()
{
}

void SPIFFSSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput)
{
    // audioManager.audioPlayer.setOutput(output);
    volume.setStream(_stream);
    volume.setOutput(output);
    _begin();
}

void SPIFFSSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    // audioManager.audioPlayer.setOutput(stream);
    volume.setStream(_stream);
    volume.setOutput(stream);
    _begin();
}

void SPIFFSSource::end()
{
    // audioPlayer.end();
    audioManager.audioPlayer.end();
}