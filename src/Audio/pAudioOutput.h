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
    bool _useEq = false;
public:
    Equilizer3Bands *eq;

    virtual String getID() = 0;
    virtual AudioOutputType GetOutputType() = 0;

    virtual audio_tools::AudioOutput& GetAudioOutput() = 0;
    virtual audio_tools::AudioStream& GetAudioStream() = 0;
    audio_tools::ModifyingStream& GetEquilizerStream();
    virtual AudioInfo GetAudioInfo() = 0;

    virtual void begin() = 0;
    virtual void end();
};

#endif