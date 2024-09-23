#include "AudioTools.h"
#include "../../pAudioOutput.h"

#ifndef dAnalogueOutput
#define dAnalogueOutput
class AnalogueOutput : public pAudioOutput
{
private:
    AnalogAudioStream _stream;
public:

    AnalogueOutput(/* args */);
    ~AnalogueOutput();

    AudioOutputType GetOutputType();

    audio_tools::AudioOutput &GetAudioOutput();
    audio_tools::AudioStream &GetAudioStream();

    void begin();

};

extern AnalogueOutput analogueOutput;

#endif