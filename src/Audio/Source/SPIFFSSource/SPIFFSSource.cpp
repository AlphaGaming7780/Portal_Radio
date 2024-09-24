#include "SPIFFSSource.h"
#include "../../AudioManager.h"

SPIFFSSource spiffsSource;

SPIFFSSource::SPIFFSSource()
{

}

SPIFFSSource::~SPIFFSSource()
{
}

void SPIFFSSource::preBegin()
{
    decoder.addNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.setAudioSource(source);
    audioManager.audioPlayer.setDecoder(decoder);
    audioManager.audioPlayer.setMetadataCallback(printMetaData);
}

void SPIFFSSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(output);
}

void SPIFFSSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(stream);
}

void SPIFFSSource::postBegin()
{
    audioManager.audioPlayer.begin();
    audioManager.audioPlayer.setVolume(0.1f);
}

void SPIFFSSource::loop()
{
    audioManager.audioPlayer.copy();
}

void SPIFFSSource::end()
{   
    decoder.removeNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.end();
}