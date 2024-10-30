#include "Arduino.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "../../pAudioSource.h"

#ifndef dWebRadioSource
#define dWebRadioSource
class WebRadioSource : public pAudioSource
{
private:

    AudioDecoder *_decoder;

//https://streamurl.link/
//https://fluxradios.blogspot.com/2014/06/flux-url-rtbf-webradios.html
    const char *urls[3] {
        // "http://stream.rcs.revma.com/5gd04cwptg0uv",
        // "https://cast1.torontocast.com:2275/stream",
        // "http://pureplay.cdnstream1.com/6005_128.mp3",
        // "https://fallout.fm:8444/falloutfm6.ogg",
        // "https://cast6.asurahosting.com/proxy/noisedep/stream",
        // "https://hardstyle.stream.laut.fm/hardstyle",
        // "https://kathy.torontocast.com:2930/stream",
        // "https://kathy.torontocast.com:2930/stream",
        // "https://scdn.nrjaudio.fm/adwz1/fr/30615/mp3_128.mp3?origine=fluxradios&aw_0_1st.station=Nostalgie-Best-of-60",
        // "https://rfm.lmn.fm/rfm.mp3", // LINE CUT OFF
        "http://radios.rtbf.be/wr-c21-60-128.mp3",
        "http://radios.rtbf.be/classic21-128.mp3",
        "http://radiocontact.ice.infomaniak.ch/radiocontact-mp3-128.mp3",
    };
public:
    ICYStream _stream;
    AudioSourceURL source = AudioSourceURL(_stream, urls, "audio/mp3");
    LinearVolumeControl volume;

    WebRadioSource(/* args */);
    ~WebRadioSource();

    String getID() override;

    void Setup() override;
    // void setOutput(audio_tools::AudioOutput &output) override;
    // void setOutput(audio_tools::AudioStream &stream) override;
    // void Begin() override;
    // void Loop() override;
    void End() override;

    // void updateVolume(float volume) override;
};

extern WebRadioSource webRadioSource;
#endif