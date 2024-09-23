#include "Arduino.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioSourceSD.h"
#include "../../pAudioSource.h"
#include "SPIFFSStream.h"

#ifndef dSPIFFSSource
#define dSPIFFSSource
class SPIFFSSource : public pAudioSource 
{
private:
    /* data */
    void _begin();
    VolumeStream volume;
    SPIFFSStream _stream;
    SPIFFSAudioSource source;
public:

    SPIFFSSource(/* args */);
    ~SPIFFSSource();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void end();

};

extern SPIFFSSource spiffsSource;;
#endif