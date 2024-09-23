#include "AudioTools.h"

#ifndef dAudioOutput
#define dAudioOutput

enum AudioOutputType {
    NONE = -1,
    OutputType = 0,
    StreamType,

};

class pAudioOutput
{
private:
    /* data */
public:
    // pAudioOutput(/* args */);
    // ~pAudioOutput();

    virtual AudioOutputType GetOutputType() = 0;

    virtual audio_tools::AudioOutput& GetAudioOutput() = 0;
    virtual audio_tools::AudioStream& GetAudioStream() = 0;

    virtual void begin() = 0;
};

#endif