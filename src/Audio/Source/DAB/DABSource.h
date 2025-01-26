#pragma once
#include "../../pAudioSource.h"
#include "T4B/ExtendedT4B.h"

class DABSource : public pAudioSource 
{
private:
    VolumeStream _volumeStream;
    LinearVolumeControl _volumeControl;

    Vector<DabProgramInfo> _CurrentDabProgramList;
    String _EnsembleNameFilter = emptyString;

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

    void setEnsembleIdFilter(String ensembleId, bool update = true);
    String getEnsembleIdFilter();
    Vector<DabProgramInfo> getCurrentProgramList();

    uint32_t getIndexInListOfProgram(uint32_t programIndex);
    uint32_t getIndexInListOfProgram(String serviceName);

    // float volumeInc() override;
    void setVolume(float volume) override;
    float getVolume() override;

};

extern DABSource DAB;
