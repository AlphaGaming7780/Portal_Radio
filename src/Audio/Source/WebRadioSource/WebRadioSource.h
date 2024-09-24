#include "Arduino.h"
#include "AudioTools.h"
#include "../../pAudioSource.h"

#ifndef dWebRadioSource
#define dWebRadioSource
class WebRadioSource : public pAudioSource
{
private:
//https://streamurl.link/
    const char *urls[1] {
        "https://radiocontact.ice.infomaniak.ch/radiocontact-mp3-192.mp3"
        // "https://stream.srg-ssr.ch/rsc_de/aacp_96.m3u",
        // "http://liveradio.swr.de/sw890cl/swr3/play.m3u",
        // "http://stream.srg-ssr.ch/m/rsj/mp3_128",
        // "http://stream.srg-ssr.ch/m/drs3/mp3_128",
        // "http://stream.srg-ssr.ch/m/rr/mp3_128",
        // "http://sunshineradio.ice.infomaniak.ch/sunshineradio-128.mp3",
        // "http://streaming.swisstxt.ch/m/drsvirus/mp3_128"
    };
    void _begin();
public:
    ICYStream _stream;
    AudioSourceURL source = AudioSourceURL(_stream, urls, ".mp3");

    WebRadioSource(/* args */);
    ~WebRadioSource();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void end();
};

extern WebRadioSource webRadioSource;
#endif