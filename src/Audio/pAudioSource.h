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

    void Begin(pAudioOutput *pAudioOutput);
    virtual void Setup() = 0;
    virtual void setOutput(audio_tools::AudioOutput &output);
    virtual void setOutput(audio_tools::AudioStream &StreamBufferDef_t);
    virtual void Begin();
    virtual void Loop();
    virtual void End();

    virtual void UpdateVolume(float volume);
    virtual void Play();
    virtual void Pause();
    virtual void Next();
    virtual void Previous();

    virtual float getVolume();

};

void PrintMetaData(MetaDataType type, const char* str, int len);
void UpdateMetaData(MetaDataType type, const char* str, int len);

#endif
