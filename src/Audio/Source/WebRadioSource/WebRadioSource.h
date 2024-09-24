#include "Arduino.h"
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "../../pAudioSource.h"

#ifndef dWebRadioSource
#define dWebRadioSource
class WebRadioSource : public pAudioSource
{
private:
//https://streamurl.link/
//https://fluxradios.blogspot.com/2014/06/flux-url-rtbf-webradios.html
    const char *urls[1] {
        "http://pureplay.cdnstream1.com/6005_128.mp3",
        // "https://fallout.fm:8444/falloutfm6.ogg",
        // "https://cast6.asurahosting.com/proxy/noisedep/stream",
        // "https://hardstyle.stream.laut.fm/hardstyle",
        // "https://kathy.torontocast.com:2930/stream",
        // "https://kathy.torontocast.com:2930/stream",
        // "https://scdn.nrjaudio.fm/adwz1/fr/30615/mp3_128.mp3?origine=fluxradios&aw_0_1st.station=Nostalgie-Best-of-60",
        // "https://rfm.lmn.fm/rfm.mp3", // LINE CUT OFF
        // "http://radios.rtbf.be/wr-c21-60-128.mp3",
        // "http://radios.rtbf.be/classic21-128.mp3",
        // "https://radiocontact.ice.infomaniak.ch/radiocontact-mp3-192.mp3"
    };
    void _begin();
public:
    ICYStream _stream;
    MP3DecoderHelix decoder;
    AudioSourceURL source = AudioSourceURL(_stream, urls, "audio/mp3");
    LinearVolumeControl volume;

    WebRadioSource(/* args */);
    ~WebRadioSource();
    void preBegin();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void postBegin();
    void loop();
    void end();
};

extern WebRadioSource webRadioSource;
#endif