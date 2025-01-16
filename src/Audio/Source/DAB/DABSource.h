#pragma once
#include "../../pAudioSource.h"

class DABSource : public pAudioSource 
{
private:
    VolumeStream _volumeStream;
    LinearVolumeControl _volumeControl;
public:
    DABSource(/* args */);
    ~DABSource();

    String getID() override;

    void Setup() override;
    void setOutput(audio_tools::AudioOutput &output) override;
    void setOutput(audio_tools::AudioStream &stream) override;
    void Begin() override;
    void End() override;

    void setProgIndex(uint32_t freq);
    void Next() override;
    void Previous() override;

    void UpdateProgramData(uint32_t programIndex);

    // float volumeInc() override;
    void setVolume(float volume) override;
    float getVolume() override;

};

extern DABSource DAB;
