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

    SDSource(/* args */);
    ~SDSource();
    void preBegin() override;
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) override;
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput) override;
    void postBegin() override;
    void loop() override;
    void end() override;

    // void updateVolume(float volume) override;

    void setPathAndBegin(char *path);

};

extern SDSource sdSource;
#endif