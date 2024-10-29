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

    void preBegin() override;
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) override;
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput) override;
    void postBegin() override;
    void loop() override;
    void end() override;

    // void updateVolume(float volume) override;

};

extern SPIFFSSource spiffsSource;;
#endif