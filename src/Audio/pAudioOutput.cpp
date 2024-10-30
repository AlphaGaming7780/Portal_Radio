#include "pAudioOutput.h"

audio_tools::ModifyingStream &pAudioOutput::getEquilizerStream()
{
    Serial.println("Use EQ.");
    if(getOutputType() == OutputType) {
        eq = new Equilizer3Bands(getAudioOutput());
    } else {
        eq = new Equilizer3Bands(getAudioStream());

    }
    ConfigEquilizer3Bands &cfg_eq = eq->defaultConfig();
    cfg_eq.setAudioInfo(getAudioInfo());
    cfg_eq.gain_low = 1.0; 
    cfg_eq.gain_medium = 0.5;
    cfg_eq.gain_high = 1.0;
    eq->begin(cfg_eq);
    _useEq = true;
    return *eq;
}

void pAudioOutput::End()
{
    switch (getOutputType())
    {
    case OutputType:
        getAudioOutput().end();
        break;
    case StreamType:
        getAudioStream().end();
        break;
    default:
        Serial.printf("pAudioOutput::end() : Unknown audio type : %i.\n", getOutputType());
        break;
    }

    if(_useEq) eq->end();
}
