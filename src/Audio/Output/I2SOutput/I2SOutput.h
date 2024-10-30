#include "AudioTools.h"
#include "../../pAudioOutput.h"

#ifndef dI2SOutput
#define dI2SOutput
class I2SOutput : public pAudioOutput
{
private:
    I2SStream _stream;
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
        return _stream;
    }

    AudioInfo getAudioInfo() override { return _stream.audioInfo(); }

    void Begin() override {
        
        auto config = _stream.defaultConfig(TX_MODE);
        config.i2s_format = I2S_LSB_FORMAT;
        config.pin_ws = 14;
        config.pin_bck = 15;
        config.pin_data = 22;
        config.sample_rate = 41000;
        config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
        config.buffer_count = 8;
        config.buffer_size = 64;
        config.use_apll = false;
        config.auto_clear = true; // avoiding noise in case of data unavailability
        _stream.begin(config);
    }
};

extern I2SOutput i2sOutput;

#endif