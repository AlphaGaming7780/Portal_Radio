#include "pAudioSource.h"

pAudioSource::pAudioSource(/* args */)
{
}

pAudioSource::~pAudioSource()
{
}

void pAudioSource::begin(pAudioOutput *pAudioOutput)
{
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
}
