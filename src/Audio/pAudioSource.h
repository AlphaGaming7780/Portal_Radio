#include "AudioTools.h"
#include "pAudioOutput.h"

#ifndef dAudioSource
#define dAudioSource

class pAudioSource
{
private:
    String _ID;
public:
    pAudioSource(/* args */);
    ~pAudioSource();

    String ID = _ID;

    void begin(pAudioOutput *pAudioOutput);
    virtual void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) = 0;
    virtual void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput) = 0;
    virtual void end() = 0;
};

#endif
