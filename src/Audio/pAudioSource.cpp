#include "pAudioSource.h"
#include "AudioManager.h"
#include "Nextion/Nextion.h"

pAudioSource::pAudioSource(/* args */)
{
}

pAudioSource::~pAudioSource()
{
}

void pAudioSource::begin(pAudioOutput *pAudioOutput)
{
    pAudioOutput->begin();
    preBegin();

    if(audioManager.useEquilizer) {
        begin(pAudioOutput->GetEquilizerStream(), pAudioOutput);
    } else {
        switch (pAudioOutput->GetOutputType())
        {
        case OutputType:
            begin(pAudioOutput->GetAudioOutput(), pAudioOutput);
            break;
        case StreamType:
            begin(pAudioOutput->GetAudioStream(), pAudioOutput);
            break;
        default:
            Serial.printf("pAudioSource::begin : Unknown audio type : %i.\n", pAudioOutput->GetOutputType());
            break;
        }
    }
    
    postBegin();
}

void pAudioSource::updateVolume(float volume) { audioManager.audioPlayer.setVolume(volume); nextion.setVolume(volume * 100, audioManager.isMuted()); };
void pAudioSource::play() { audioManager.audioPlayer.play(); };
void pAudioSource::pause() { audioManager.audioPlayer.stop(); };
void pAudioSource::next() { audioManager.audioPlayer.next(); };
void pAudioSource::previous() { audioManager.audioPlayer.previous(); }
float pAudioSource::getVolume() { return audioManager.audioPlayer.volume(); }

void printMetaData(MetaDataType type, const char *str, int len)
{
    Serial.print("==> ");
    Serial.print(toStr(type));
    Serial.print(": ");
    Serial.println(str);
}

void updateMetaData(MetaDataType type, const char *str, int len)
{
    printMetaData(type, str, len);
    switch (type)
    {
    case Title:
        nextion.setTitle(str);
        break;
    case Artist:
        nextion.setArtist(str);
        break;
    case Name:
        nextion.setTitle(str);
        break;
    case Description:
        nextion.setArtist(str);
        break;

    default:
        break;
    }
}
