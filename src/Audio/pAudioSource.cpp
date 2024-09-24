#include "pAudioSource.h"

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
    switch (pAudioOutput->GetOutputType())
    {
    case OutputType:
        begin(pAudioOutput->GetAudioOutput(), pAudioOutput);
        break;
    case StreamType:
        begin(pAudioOutput->GetAudioStream(), pAudioOutput);
        break;
    default:
        Serial.printf("Unknown audio type : %i.\n", pAudioOutput->GetOutputType());
        break;
    }
    postBegin();
}

void printMetaData(MetaDataType type, const char *str, int len)
{
    Serial.print("==> ");
    Serial.print(toStr(type));
    Serial.print(": ");
    Serial.println(str);
}
