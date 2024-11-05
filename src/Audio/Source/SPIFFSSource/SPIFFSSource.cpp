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

AudioSource &SPIFFSSource::getAudioSource()
{
    return source;
}

// void SPIFFSSource::Setup()
// {
//     decoder.addNotifyAudioChange(audioManager.audioPlayer);
//     audioManager.audioPlayer.setAudioSource(source);
//     audioManager.audioPlayer.setDecoder(decoder);
//     audioManager.audioPlayer.setMetadataCallback(UpdateMetaData);
// }

// void SPIFFSSource::setOutput(audio_tools::AudioOutput &output)
// {
//     audioManager.audioPlayer.setOutput(output);
// }

// void SPIFFSSource::setOutput(audio_tools::AudioStream &stream)
// {
//     audioManager.audioPlayer.setOutput(stream);
// }

// void SPIFFSSource::Begin()
// {
//     audioManager.audioPlayer.begin();
// }

// void SPIFFSSource::Loop()
// {
//     audioManager.audioPlayer.copy();
// }

// void SPIFFSSource::End()
// {   
//     decoder.removeNotifyAudioChange(audioManager.audioPlayer);
//     audioManager.audioPlayer.end();
// }

// void SPIFFSSource::updateVolume(float volume)
// {
//     audioManager.audioPlayer.setVolume(volume);
// }