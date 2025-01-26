#include "DABSource.h"
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
    _EnsembleNameFilter = userDataManager.getLastDabEnsembleIdFilter();
    _CurrentDabProgramList = t4b.getFilteredByEnsembleIdDabProgramList(_EnsembleNameFilter.c_str());
    uint32_t lastDabProgIndex = userDataManager.getLastDabProgramIndex();
    uint32_t totalProg;
    
    if(lastDabProgIndex < 0) lastDabProgIndex = 0;
    else if(!t4b.getTotalProgram(&totalProg)) lastDabProgIndex = 0;
    else if(lastDabProgIndex >= totalProg) lastDabProgIndex = totalProg - 1;

    t4b.PlayDab(lastDabProgIndex);
    audioManager.CreateStreamCopierTask();

    DabSorter dabSorter;
    t4b.getSorter(&dabSorter);
    nextion.setDabSorter(dabSorter);

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
    if(!t4b.getPlayIndex(&currentProg)) return;

    uint32_t totalProg = _CurrentDabProgramList.size();
    uint32_t listNextIndex = getIndexInListOfProgram(currentProg) + 1;

    if(listNextIndex >= totalProg) listNextIndex = 0;
    Serial.printf("Total prog : %u, current Prog : %u.\n", totalProg, listNextIndex);

    setProgIndex(_CurrentDabProgramList[listNextIndex].ProgramIndex);
}

void DABSource::Previous()
{
    uint32_t currentProg;
    if(!t4b.getPlayIndex(&currentProg)) return;

    uint32_t totalProg = _CurrentDabProgramList.size();
    uint32_t listIndex = getIndexInListOfProgram(currentProg);

    if(listIndex == 0 ) listIndex = (totalProg - 1);
    else listIndex--;
    Serial.printf("Total prog : %u, current Prog : %u.\n", totalProg, listIndex);

    setProgIndex(_CurrentDabProgramList[listIndex].ProgramIndex);
}

void DABSource::UpdateProgramData(uint32_t programIndex)
{
    nextion.setDabProgramIndex(programIndex);
    char buffer[T4BMaxTextSize];
    if(t4b.getProgrameNameDAB(buffer, sizeof(buffer), programIndex)) nextion.setTitle(buffer);
    else nextion.setTitle("");
    nextion.setArtist("");
    nextion.setEnsembleName("");
    nextion.setServiceName("");

    t4b.getEnsembleName(buffer, sizeof(buffer), programIndex);
    Serial.printf("Ensemble Name : %s.\n", buffer);
    nextion.setEnsembleName(buffer);

    t4b.getServiceName(buffer, sizeof(buffer), programIndex);
    Serial.printf("Service Name : %s.\n", buffer);
    nextion.setServiceName(buffer);
}

void DABSource::setEnsembleIdFilter(String ensembleId, bool update)
{
    _EnsembleNameFilter = ensembleId;
    _CurrentDabProgramList = t4b.getFilteredByEnsembleIdDabProgramList(ensembleId.c_str());
    userDataManager.setLastDabEnsembleIdFilter(_EnsembleNameFilter);
    if(update) nextion.SendDabStationList();
}

String DABSource::getEnsembleIdFilter()
{
    return _EnsembleNameFilter;
}

Vector<DabProgramInfo> DABSource::getCurrentProgramList()
{
    return _CurrentDabProgramList;
}

uint32_t DABSource::getIndexInListOfProgram(uint32_t programIndex)
{
    int i = 0;
    for( auto obj : _CurrentDabProgramList) {
        if(obj.ProgramIndex == programIndex) return i;
        i++;
    }
    return 0;
}

uint32_t DABSource::getIndexInListOfProgram(String serviceName)
{
    int i = 0;
    for (auto obj : _CurrentDabProgramList) {
        if(obj.ServiceName == serviceName) return i;
    }
    debug.printlnError("Failed to get the index in list for " + String(serviceName) + " service name!");
    return 0;
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
