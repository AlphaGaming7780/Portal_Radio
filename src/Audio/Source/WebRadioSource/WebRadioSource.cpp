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

AudioSource &WebRadioSource::getAudioSource()
{
    return source;
}

// void WebRadioSource::Setup()
// {
//     // Same code for all the source exept the BLE, maybe merge that function into the pAudioSource.
//     _decoder = audioManager.getDecoder(MP3);

//     _decoder->addNotifyAudioChange(audioManager.audioPlayer);

//     audioManager.audioPlayer.setAudioSource(source);
//     audioManager.audioPlayer.setDecoder(*_decoder);
//     audioManager.audioPlayer.setMetadataCallback(UpdateMetaData);
// }

// void WebRadioSource::setOutput(audio_tools::AudioOutput &output) 
// {   
//     audioManager.audioPlayer.setOutput(output);
// }

// void WebRadioSource::setOutput(audio_tools::AudioStream &stream)
// {
//     audioManager.audioPlayer.setOutput(stream);
// }

// void WebRadioSource::Begin()
// {
//     audioManager.audioPlayer.begin();
// }

// void WebRadioSource::Loop()
// {
//     audioManager.audioPlayer.copy();
// }

// void WebRadioSource::End() {
//     _decoder->removeNotifyAudioChange(audioManager.audioPlayer);
//     audioManager.audioPlayer.end();
//     WiFi.disconnect(true, true);
// }
