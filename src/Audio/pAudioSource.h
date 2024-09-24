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
    virtual void preBegin() = 0;
    virtual void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) = 0;
    virtual void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput) = 0;
    virtual void postBegin() = 0;
    virtual void loop() = 0;
    virtual void end() = 0;

};

void printMetaData(MetaDataType type, const char* str, int len);

#endif
