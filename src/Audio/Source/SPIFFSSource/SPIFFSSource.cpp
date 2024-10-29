#include "SPIFFSSource.h"
#include "../../AudioManager.h"

SPIFFSSource spiffsSource;

SPIFFSSource::SPIFFSSource()
{

}

SPIFFSSource::~SPIFFSSource()
{
}

String SPIFFSSource::getID()
{
    return "SPIFFS";
}

void SPIFFSSource::preBegin()
{
    decoder.addNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.setAudioSource(source);
    audioManager.audioPlayer.setDecoder(decoder);
    audioManager.audioPlayer.setMetadataCallback(updateMetaData);
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

// void SPIFFSSource::updateVolume(float volume)
// {
//     audioManager.audioPlayer.setVolume(volume);
// }