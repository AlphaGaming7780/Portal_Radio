#include "WebRadioSource.h"
#include "../../AudioManager.h"
// AnalogAudioStream out;
// BluetoothA2DPSink a2dp_sink(out);

WebRadioSource webRadioSource;

WebRadioSource::WebRadioSource()
{
}

WebRadioSource::~WebRadioSource()
{
}

String WebRadioSource::getID()
{
    return "Webradio";
}

void WebRadioSource::preBegin()
{
    decoder.addNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.setAudioSource(source);
    audioManager.audioPlayer.setDecoder(decoder);
    audioManager.audioPlayer.setMetadataCallback(printMetaData);
}

void WebRadioSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) 
{   
    audioManager.audioPlayer.setOutput(output);
}

void WebRadioSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    audioManager.audioPlayer.setOutput(stream);
}

void WebRadioSource::postBegin()
{
    audioManager.audioPlayer.begin();
}

void WebRadioSource::loop()
{
    audioManager.audioPlayer.copy();
}

void WebRadioSource::end() {
    decoder.removeNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.end();
}

void WebRadioSource::updateVolume(float volume)
{
    audioManager.audioPlayer.setVolume(volume);
}
