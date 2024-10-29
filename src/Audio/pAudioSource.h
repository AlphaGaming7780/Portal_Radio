#include "AudioTools.h"
#include "pAudioOutput.h"

#ifndef dAudioSource
#define dAudioSource

class pAudioSource
{
private:

public:
    pAudioSource(/* args */);
    ~pAudioSource();

    virtual String getID() = 0;

    void begin(pAudioOutput *pAudioOutput);
    virtual void preBegin() = 0;
    virtual void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) = 0;
    virtual void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput) = 0;
    virtual void postBegin() = 0;
    virtual void loop() = 0;
    virtual void end() = 0;

    virtual void updateVolume(float volume);
    virtual void play();
    virtual void pause();
    virtual void next();
    virtual void previous();

    virtual float getVolume();

};

void printMetaData(MetaDataType type, const char* str, int len);
void updateMetaData(MetaDataType type, const char* str, int len);

#endif
