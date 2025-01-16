#include "DABSource.h"
#include "T4B/T4B.h"
#include "../../I2SManager.h"
#include "Audio/AudioManager.h"
#include "Nextion/Nextion.h"
#include <DataManager/UserDataManager.h>

DABSource DAB;

DABSource::DABSource()
{
}

DABSource::~DABSource()
{
}

String DABSource::getID()
{
    return "DAB";
}

void DABSource::Setup()
{
    I2SManager::BeginIn();
}

void DABSource::setOutput(audio_tools::AudioOutput &output)
{
    
}

void DABSource::setOutput(audio_tools::AudioStream &stream)
{
    _volumeStream.setStream(stream);
    _volumeStream.setVolumeControl(_volumeControl);
    auto vcfg = _volumeStream.defaultConfig();
    vcfg.copyFrom(stream.audioInfo());
    _volumeStream.begin(vcfg);
    audioManager.streamCopie.begin(_volumeStream, I2SManager::_streamIn );
}

void DABSource::Begin()
{
    uint32_t lastDabProgIndex = userDataManager.getLastDabProgramIndex();
    uint32_t totalProg;
    
    if(lastDabProgIndex < 0) lastDabProgIndex = 0;
    else if(!t4b.getTotalProgram(&totalProg)) lastDabProgIndex = 0;
    else if(lastDabProgIndex >= totalProg) lastDabProgIndex = totalProg - 1;

    t4b.PlayDab(lastDabProgIndex);
    audioManager.CreateStreamCopierTask();
    UpdateProgramData(lastDabProgIndex);
}

void DABSource::End()
{
    audioManager.DeleteStreamCopierTask();
    audioManager.streamCopie.end();
    t4b.Stop();
    I2SManager::_streamIn.end();
}

void DABSource::setProgIndex(uint32_t progIndex)
{
    if(!t4b.PlayDab(progIndex)) return;
    userDataManager.setLastDabProgramIndex(progIndex);
    userDataManager.Save();

    UpdateProgramData(progIndex);
}

void DABSource::Next()
{
    uint32_t currentProg;
    uint32_t totalProg;
    if(!t4b.getTotalProgram(&totalProg) || !t4b.getPlayIndex(&currentProg)) return;

    Serial.printf("Total prog : %u, current Prog : %u.\n", totalProg, currentProg);
    if(currentProg++ >= totalProg) currentProg = 0;

    if(!t4b.PlayDab(currentProg)) return;
    userDataManager.setLastDabProgramIndex(currentProg);
    userDataManager.Save();

    UpdateProgramData(currentProg);

}

void DABSource::Previous()
{
    uint32_t currentProg;
    uint32_t totalProg;
    if(!t4b.getTotalProgram(&totalProg) || !t4b.getPlayIndex(&currentProg)) return;

    Serial.printf("Total prog : %u, current Prog : %u.\n", totalProg, currentProg);
    if(currentProg-- < 0 ) currentProg = totalProg - 1;

    if(!t4b.PlayDab(currentProg)) return;
    userDataManager.setLastDabProgramIndex(currentProg);
    userDataManager.Save();

    UpdateProgramData(currentProg);

}

void DABSource::UpdateProgramData(uint32_t programIndex)
{
    char buffer[T4BMaxTextSize];
    if(t4b.getProgrameNameDAB(buffer, sizeof(buffer), programIndex)) nextion.setTitle(buffer);
    else nextion.setTitle("");
    nextion.setArtist("");

    t4b.getEnsembleName(buffer, sizeof(buffer), programIndex);
    Serial.printf("Ensemble Name : %s.\n", buffer);

    t4b.getServiceName(buffer, sizeof(buffer), programIndex);
    Serial.printf("Service Name : %s.\n", buffer);
}

// float DABSource::volumeInc()
// {
//     return 0.0625f;
// }

void DABSource::setVolume(float volume)
{
    _volumeStream.setVolume(volume);
    nextion.setVolume(volume * 100, audioManager.isMuted());
}

float DABSource::getVolume()
{   
    return _volumeStream.volume();
}
