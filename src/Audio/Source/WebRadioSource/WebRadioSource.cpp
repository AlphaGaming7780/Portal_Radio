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

void WebRadioSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) 
{   
    pAudioOutput->begin();
    _begin();
}

void WebRadioSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    pAudioOutput->begin();
    _begin();
}

void WebRadioSource::_begin()
{
    audioManager.audioPlayer.setAudioSource(source);
    audioManager.audioPlayer.begin();
}

void WebRadioSource::end() {

}