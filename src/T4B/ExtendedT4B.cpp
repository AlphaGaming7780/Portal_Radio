#include "ExtendedT4B.h"
#include "Nextion/Nextion.h"

ExtendedT4B t4b(bt4b);


ExtendedT4B::ExtendedT4B(HardwareSerial *serial, uint8_t resetPin) : T4B(serial, resetPin)
{
}

ExtendedT4B::ExtendedT4B(T4B &t4b) : T4B(t4b)
{
}

ExtendedT4B::~ExtendedT4B()
{
}

void ExtendedT4B::OnT4BEvent(EventType eventType)
{
    Serial.printf("Processing Event : %u.\n", eventType);

    uint32_t currentFreqOrProgIndex;
    if(!getPlayIndex(&currentFreqOrProgIndex)) return;
    
    char buffer[T4BMaxTextSize];

    if( ( eventType & EventType::ProgramNameChange ) > 0) {
        StreamMode streamMode;
        if(getPlayMode(&streamMode)) {
            if(streamMode == StreamMode::Dab) {
                getProgrameNameDAB(buffer, sizeof(buffer), currentFreqOrProgIndex);
            } else if(streamMode == StreamMode::Fm) {
                getProgrameNameFM(buffer, sizeof(buffer));
            }
        };
        Serial.printf("Program Name Changed : %s.\n", buffer);
        nextion.setTitle(buffer);    
    }
    
    if( ( eventType & EventType::ProgramTextChange ) > 0 ) {
        getProgrameText(buffer, sizeof(buffer));
        Serial.printf("Program Text Changed : %s.\n", buffer);
        nextion.setArtist(buffer);
    }
    
    if ( ( eventType & EventType::DLSCommandChange ) > 0 ) {
        Serial.printf("DLS Command CHanged.\n");
    }
    
    if ( ( eventType & EventType::StereoModeChange ) > 0 ) {
        StereoMode stereoMode;
        if(getStereo(&stereoMode)) {
            Serial.printf("Stereo Mode Changed : %u.\n", stereoMode);
        }
    }

    if ( ( eventType & EventType::ServiceUpdate ) > 0 ) {
        getPlayIndex(&_CurrentPlayIndex, true);
        UpdateDabCache();
        nextion.SendDabStationList(_CurrentPlayIndex);
        DAB.UpdateProgramData(_CurrentPlayIndex);
    }

    if ( ( eventType & EventType::SortChange ) > 0 ) {
        getPlayIndex(&_CurrentPlayIndex, true);
        UpdateDabCache();
        nextion.SendDabStationList(_CurrentPlayIndex);
        DAB.UpdateProgramData(_CurrentPlayIndex);
    }

    if ( ( eventType & EventType::FrequencyChange ) > 0 ) {
        uint8_t freqIndex;
        getFrequency(currentFreqOrProgIndex, &freqIndex);
        Serial.printf("Frequency Changed : %u.\n", freqIndex);
    }

    if ( ( eventType & EventType::TimeChange ) > 0 ) {
        debug.printlnInfo("T4B Event : TimeChange");
        alarmManager.SetClockDirty();
    }
}

void ExtendedT4B::OnT4BNotification(NotificationType notifType)
{
    Serial.printf("Process Notification : %u.\n", notifType);
    char buffer[T4BMaxTextSize];

    switch (notifType)
    {
        case NotificationType::ScanFinished:
            if(!T4B::getPlayIndex(&_CurrentPlayIndex)) break;

            // Removed because it cause a bug where it get DAB mode instead of FM when scanning for FM freq.
            // if(!T4B::getPlayMode(&_CurrentStreamMode)) break;

            if(_CurrentStreamMode == StreamMode::Dab) {
                Serial.printf("Playing program index : %u.\n", _CurrentPlayIndex);
                if(_CurrentPlayIndex < DabIndexMin || _CurrentPlayIndex > DabIndexMax) break;
                userDataManager.setLastDabProgramIndex(_CurrentPlayIndex);
                userDataManager.Save();
                DAB.UpdateProgramData(_CurrentPlayIndex);

            } else if(_CurrentStreamMode == StreamMode::Fm) {
                Serial.printf("Playing Frequence : %u.\n", _CurrentPlayIndex);
                if(_CurrentPlayIndex < FmFrequencyMin || _CurrentPlayIndex > FmFrequencyMax) break;
                userDataManager.setLastFmFreq(_CurrentPlayIndex);
                userDataManager.Save();
                nextion.setTitle("");
                nextion.setArtist("");
                nextion.setFmFreq(_CurrentPlayIndex);
            }

            break;
        
        case NotificationType::NewFmProgrameText:
            getProgrameText(buffer, sizeof(buffer));
            nextion.setArtist(String(buffer));
            Serial.printf("ProgrameText : %s.\n", buffer);
            break;
            
        case NotificationType::DABReconfiguration:
            break;

        case NotificationType::DABChannelListOrderChange:
            break;

        case NotificationType::RDSGroup:
            break;

        case NotificationType::NewDABRadioText:
            getProgrameText(buffer, sizeof(buffer));
            nextion.setArtist(String(buffer));
            Serial.printf("Value : %s", buffer);
            break; 

        case NotificationType::ScanFrequency:
            break;

        default:
            Serial.printf("Unknown Notification type : %i.\n", notifType);
            break;
    }
}

void ExtendedT4B::Setup(bool cacheDabInfo)
{
    Init();
    AddOnEventAndNotification(*this);
    EnableSyncClock(true);
    if(!EnableI2S()) debug.printlnError("T4B : Failed to enable I2S.");
    if(!setVolume(16)) debug.printlnWarn("T4B : Failed to set volume.");
    // if(!setLRMode()) debug.printlnWarn("T4B : Failed to setLRMode.");
    if(!setStereoMode()) debug.printlnWarn("T4B : Failed to setStereoMode.");
    if(!setSorter(DabSorter::EnsembleId)) debug.printlnWarn("T4B : Failed to setSorter.");
    if(!setNotification(NotificationType::ScanFinished | NotificationType::ScanFrequency)) Serial.println("Failed to set notif.");
    if(cacheDabInfo) UpdateDabCache();
}

void ExtendedT4B::UpdateDabCache()
{
    if(!T4B::getTotalProgram(&_DabTotalprogram)) {
        debug.printlnError("Failed to get the total program!");
        return;
    }

    _DabProgramsList.clear();
    _EnsembleIdsList.clear();

    char ensembleId[T4BMaxTextSize];
    char serviceName[T4BMaxTextSize];

    for (int i = 0; i < _DabTotalprogram; i++)
    {
        T4B::getEnsembleName(ensembleId, sizeof(ensembleId), i);
        T4B::getServiceName(serviceName, sizeof(serviceName), i);
        // DabProgramInfo* prog = new DabProgramInfo(serviceName, ensembleId, i);
        DabProgramInfo prog(serviceName, ensembleId, i);
        // debug.printlnInfo("Dab program " + String(prog.ServiceName) + " as been added to the list.");
        _DabProgramsList.push_back(prog);
        if(!_EnsembleIdsList.contains(ensembleId)) _EnsembleIdsList.push_back(ensembleId);
    }

    debug.printlnInfo("UpdateDabCache : _DabProgramsList size : " + String(_DabProgramsList.size()));
    debug.printlnInfo("UpdateDabCache : _EnsembleIdsList size : " + String(_EnsembleIdsList.size()));
}

Vector<DabProgramInfo> ExtendedT4B::getDabProgramsList()
{
    return _DabProgramsList;
}

void ExtendedT4B::getFilteredByEnsembleIdDabProgramList(Vector<DabProgramInfo> &data, const char *ensembleId)
{
    data.clear();

    debug.printlnInfo("Filtering dab programs with " + String(ensembleId) + " ensemble id.");

    for(auto obj : _DabProgramsList) {
        if(obj.EnsembleId == ensembleId) 
        { 
            data.push_back(obj);
            // debug.printlnInfo(String(obj.ServiceName) + " as the right ensemble id.");
            continue;
        }
        // debug.printlnInfo(String(obj.ServiceName) + " as not the same ensemble id, " + String(obj.EnsembleId) + " != " + String(ensembleId) + ".");
    }
}

Vector<String> ExtendedT4B::getEnsembleIdList()
{
    return _EnsembleIdsList;
}

uint32_t ExtendedT4B::getProgramIndexByServiceName(const char *serviceName)
{
    for (auto obj : _DabProgramsList) {
        if(obj.ServiceName == serviceName) return obj.ProgramIndex;
    }
    debug.printlnError("Failed to get the program Index for " + String(serviceName) + " service name!");
    return 0;
}

bool ExtendedT4B::PlayDab(uint32_t const stationId)
{
    if(!T4B::PlayDab(stationId)) return false;
    _CurrentStreamMode = StreamMode::Dab;
    _CurrentPlayIndex = stationId;
    return true;
}

bool ExtendedT4B::PlayFm(uint32_t const frequency)
{
    if(!T4B::PlayFm(frequency)) return false;
    _CurrentStreamMode = StreamMode::Fm;
    _CurrentPlayIndex = frequency;
    return true;
}

bool ExtendedT4B::getPlayMode(StreamMode *const streamMode, bool const forceUpdate)
{
    if(forceUpdate) {
        if (!T4B::getPlayMode(streamMode)) return false;
        _CurrentStreamMode = *streamMode;
        return true;
    }
    *streamMode = _CurrentStreamMode;
    return true;
}

bool ExtendedT4B::getPlayIndex(uint32_t *const playIndex, bool const forceUpdate)
{
    if(forceUpdate) {
        if (!T4B::getPlayIndex(playIndex)) return false;
        _CurrentPlayIndex = *playIndex;
        return true;
    }
    *playIndex = _CurrentPlayIndex;
    return true;
}

bool ExtendedT4B::getTotalProgram(uint32_t *const totalProgram, bool const forceUpdate)
{
    if(forceUpdate) {
        if (!T4B::getTotalProgram(totalProgram)) return false;
        _DabTotalprogram = *totalProgram;
        return true;
    }
    *totalProgram = _DabTotalprogram;
    return true;
}

bool ExtendedT4B::getEnsembleName(char *const ensembleName, uint16_t const size, uint32_t const programIndex, bool const forceUpdate)
{
    DabProgramInfo prog = _DabProgramsList[programIndex];
    if(forceUpdate) {
        if (!T4B::getEnsembleName(ensembleName, size, programIndex)) return false;
        prog.EnsembleId = ensembleName;
        return true;
    }
    prog.EnsembleId.toCharArray(ensembleName, size);
    return true;
}

bool ExtendedT4B::getServiceName(char *const serviceName, uint16_t const size, uint32_t const programIndex, bool const forceUpdate)
{
    DabProgramInfo prog = _DabProgramsList[programIndex];
    if(forceUpdate) {
        if (!T4B::getEnsembleName(serviceName, size, programIndex)) return false;
        prog.ServiceName = serviceName;
        return true;
    }
    prog.ServiceName.toCharArray(serviceName, size);
    return true;
}

bool ExtendedT4B::getPresetFM(uint8_t const presetIndex, uint32_t *const frequency, bool const forceUpdate)
{
    //                  Not a good idea but should be a bit better, have to change to full cache this.
    if(forceUpdate || _FmPresets[presetIndex] == 0) 
    {
        if(!T4B::getPresetFM(presetIndex, frequency)) return false;

        _FmPresets[presetIndex] = *frequency;

        return true;
    }

    *frequency = _FmPresets[presetIndex];

    return *frequency != 0;
}

bool ExtendedT4B::setPresetFM(uint8_t const presetIndex, uint32_t const frequency)
{
    if(T4B::setPresetFM(presetIndex, frequency)) return false;
    _FmPresets[presetIndex] = frequency;
    return true;
}
