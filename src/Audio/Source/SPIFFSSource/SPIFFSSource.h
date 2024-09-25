#include "Arduino.h"
#include "AudioTools.h"
#include "AudioLibs/AudioSourceSPIFFS.h"
#include "AudioCodecs/CodecMP3Helix.h"
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

    String getID();

    void preBegin();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void postBegin();
    void loop();
    void end();

    void updateVolume(float volume);

};

extern SPIFFSSource spiffsSource;;
#endif