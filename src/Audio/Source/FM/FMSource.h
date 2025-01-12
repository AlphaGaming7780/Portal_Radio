#pragma once
#include "../../pAudioSource.h"

class FMSource : public pAudioSource 
{
private:
public:
    FMSource(/* args */);
    ~FMSource();

    String getID() override;

    void Setup() override;
    void setOutput(audio_tools::AudioOutput &output) override;
    void setOutput(audio_tools::AudioStream &StreamBufferDef_t) override;
    void Begin() override;
    void End() override;

    void Play();
    void Pause();
    void Next();
    void Previous();

    float volumeInc() override;
    void setVolume(float volume) override;
    float getVolume() override;

};

extern FMSource FM;
