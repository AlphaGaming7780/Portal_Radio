#include "Arduino.h"
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioSourceSPIFFS.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "../../pAudioSource.h"

#ifndef dSPIFFSSource
#define dSPIFFSSource
class SPIFFSSource : public pAudioSource 
{
private:
    /* data */
    
public:
    AudioSourceSPIFFS source;
    MP3DecoderHelix decoder;

    SPIFFSSource(/* args */);
    ~SPIFFSSource();

    String getID() override;
    AudioSource &getAudioSource() override;

    // void Setup() override;
    // void setOutput(audio_tools::AudioOutput &output) override;
    // void setOutput(audio_tools::AudioStream &stream) override;
    // void Begin() override;
    // void Loop() override;
    // void End() override;

    // void updateVolume(float volume) override;

};

extern SPIFFSSource spiffsSource;;
#endif