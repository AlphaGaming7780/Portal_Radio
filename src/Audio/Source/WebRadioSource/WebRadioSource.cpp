#include "WebRadioSource.h"
#include "../../AudioManager.h"
// #include "main.h"
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

    _decoder = audioManager.getDecoder(MP3);

    _decoder->addNotifyAudioChange(audioManager.audioPlayer);

    audioManager.audioPlayer.setAudioSource(source);
    audioManager.audioPlayer.setDecoder(*_decoder);
    audioManager.audioPlayer.setMetadataCallback(updateMetaData);
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
    _decoder->removeNotifyAudioChange(audioManager.audioPlayer);
    audioManager.audioPlayer.end();
    WiFi.disconnect(true, true);
}
