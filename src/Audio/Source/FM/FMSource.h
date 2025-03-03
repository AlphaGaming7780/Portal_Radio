#pragma once
#include "../../pAudioSource.h"

class FMSource : public pAudioSource 
{
private:
    VolumeStream _volumeStream;
    LinearVolumeControl _volumeControl;
public:
    FMSource(/* args */);
    ~FMSource();

    String getID() override;

    void Setup() override;
    void setOutput(audio_tools::AudioOutput &output) override;
    void setOutput(audio_tools::AudioStream &stream) override;
    void Begin() override;
    void End() override;

    void setFreq(uint32_t freq);
    uint32_t getFreq();

    void playPreset(uint8_t presetId);
    void setPreset(uint8_t presetId);
    void Next() override;
    void Previous() override;

    // float volumeInc() override;
    void setVolume(float volume) override;
    float getVolume() override;

};

extern FMSource FM;
