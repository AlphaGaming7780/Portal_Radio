#include "AudioTools.h"
#include "../../pAudioOutput.h"
#include "../../I2SManager.h"

#ifndef dI2SOutput
#define dI2SOutput
class I2SOutput : public pAudioOutput
{
private:
public:

    I2SOutput(/* args */) {}
    ~I2SOutput() {}

    String getID() override { return "Speaker"; }

    AudioOutputType getOutputType() override
    {
        return StreamType;
    }
    audio_tools::AudioOutput &getAudioOutput() override {}
    audio_tools::AudioStream &getAudioStream() override
    {
        return I2SManager::_streamOut;
    }

    AudioInfo getAudioInfo() override { return I2SManager::_streamOut.audioInfo(); }

    void Begin() override {
        I2SManager::BeginOut();
    }
};

extern I2SOutput i2sOutput;

#endif