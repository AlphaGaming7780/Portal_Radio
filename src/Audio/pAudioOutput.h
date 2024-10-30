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
    virtual AudioOutputType getOutputType() = 0;

    virtual audio_tools::AudioOutput& getAudioOutput() = 0;
    virtual audio_tools::AudioStream& getAudioStream() = 0;
    audio_tools::ModifyingStream& getEquilizerStream();
    virtual AudioInfo getAudioInfo() = 0;

    virtual void Begin() = 0;
    virtual void End();
};

#endif