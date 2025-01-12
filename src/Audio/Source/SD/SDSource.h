#include "Arduino.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioSourceSD.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "../../pAudioSource.h"

#ifndef dSDSource
#define dSDSource
class SDSource : public pAudioSource 
{
private:
    AudioDecoder *_decoder;

public:
    // AudioPlayer audioPlayer;
    AudioSourceSD source;

    String getID() override;
    AudioSource &getAudioSource() override;

    SDSource(/* args */);
    ~SDSource();
    // void Setup() override;
    // void setOutput(audio_tools::AudioOutput &output) override;
    // void setOutput(audio_tools::AudioStream &stream) override;
    // void Begin() override;
    // void Loop() override;
    // void End() override;

    void setPathAndPlay(char *path);

};

extern SDSource sdSource;
#endif