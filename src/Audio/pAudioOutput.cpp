#include "pAudioOutput.h"

audio_tools::ModifyingStream &pAudioOutput::GetEquilizerStream()
{
    Serial.println("Use EQ.");
    if(GetOutputType() == OutputType) {
        eq = new Equilizer3Bands(GetAudioOutput());
    } else {
        eq = new Equilizer3Bands(GetAudioStream());

    }
    ConfigEquilizer3Bands &cfg_eq = eq->defaultConfig();
    cfg_eq.setAudioInfo(GetAudioInfo());
    cfg_eq.gain_low = 1.0; 
    cfg_eq.gain_medium = 0.5;
    cfg_eq.gain_high = 1.0;
    eq->begin(cfg_eq);
    _useEq = true;
    return *eq;
}

void pAudioOutput::end()
{
    switch (GetOutputType())
    {
    case OutputType:
        GetAudioOutput().end();
        break;
    case StreamType:
        GetAudioStream().end();
    default:
        Serial.printf("Unknown audio type : %i.\n", GetOutputType());
        break;
    }

    if(_useEq) eq->end();
}
