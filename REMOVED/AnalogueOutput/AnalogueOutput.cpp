#include "AnalogueOutput.h"

AnalogueOutput analogueOutput;

AnalogueOutput::AnalogueOutput(/* args */) {}
AnalogueOutput::~AnalogueOutput() {}

AudioOutputType AnalogueOutput::GetOutputType() 
{
    return StreamType;
}
audio_tools::AudioOutput &AnalogueOutput::GetAudioOutput() {}
audio_tools::AudioStream &AnalogueOutput::GetAudioStream() 
{
    return _stream;
}

void AnalogueOutput::begin()
{
    // auto config = _stream.defaultConfig();
    // _stream.begin(config);
};