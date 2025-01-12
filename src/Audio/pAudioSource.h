#include "AudioTools.h"
#include "pAudioOutput.h"

#ifndef dAudioSource
#define dAudioSource

class pAudioSource
{
private:

    AudioDecoder *_decoder;

public:
    pAudioSource(/* args */);
    ~pAudioSource();

    virtual String getID() = 0;
    virtual AudioSource &getAudioSource() {};

    void Begin(pAudioOutput *pAudioOutput);
    virtual void Setup();
    virtual void setOutput(audio_tools::AudioOutput &output);
    virtual void setOutput(audio_tools::AudioStream &StreamBufferDef_t);
    virtual void Begin();
    virtual void End();

    virtual void Play();
    virtual void Pause();
    virtual void Next();
    virtual void Previous();

    virtual float volumeInc();
    virtual void setVolume(float volume);
    virtual float getVolume();

};

void PrintMetaData(MetaDataType type, const char* str, int len);
void UpdateMetaData(MetaDataType type, const char* str, int len);

#endif
