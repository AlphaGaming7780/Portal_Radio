#include "Arduino.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioSourceSD.h"
#include "../../pAudioSource.h"

#ifndef dSDSource
#define dSDSource
class SDSource : public pAudioSource 
{
private:
    /* data */
    void _begin();

public:
    // AudioPlayer audioPlayer;
    AudioSourceSD sd;

    SDSource(/* args */);
    ~SDSource();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void end();

};

extern SDSource sdSource;
#endif