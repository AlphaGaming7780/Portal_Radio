#pragma once
#include "T4B/T4B.h"
#include "AudioTools.h"

class DabProgramInfo
{
public:

    DabProgramInfo() {}

    DabProgramInfo(String serviceName, String ensembleId, uint32_t programIndex) {
        ServiceName = serviceName;
        EnsembleId = ensembleId;
        ProgramIndex = programIndex;
    }

    ~DabProgramInfo() {}

    String ServiceName;
    String EnsembleId;
    uint32_t ProgramIndex;
};

class ExtendedT4B : public T4B, public T4BEventAndNotificationSupport
{
private:

    Vector<DabProgramInfo> _DabProgramsList;
    Vector<String> _EnsembleIdsList;
    uint32_t _DabTotalprogram;
    StreamMode _CurrentStreamMode;
    uint32_t _CurrentPlayIndex = -1;
    
    //                          1,  2,  3,  4,  5,  6,  7,  8,  9,  10
    uint32_t _FmPresets[10] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

public:
    ExtendedT4B(HardwareSerial *serial, uint8_t resetPin);
    ExtendedT4B(T4B &t4b);
    ~ExtendedT4B();

    void OnT4BEvent(EventType eventType) override;
    void OnT4BNotification(NotificationType notifType) override;

    void Setup(bool cacheDabInfo = true);

    void UpdateDabCache();
    Vector<DabProgramInfo> getDabProgramsList();
    void getFilteredByEnsembleIdDabProgramList(Vector<DabProgramInfo> &data, const char *ensembleId);

    Vector<String> getEnsembleIdList();

    uint32_t getProgramIndexByServiceName(const char* serviceName);

    bool PlayDab(uint32_t const stationId);
    bool PlayFm(uint32_t const frequency);

    bool getPlayMode(StreamMode* const streamMode, bool const forceUpdate = false);
    bool getPlayIndex(uint32_t* const playIndex, bool const forceUpdate = false);
    bool getTotalProgram(uint32_t* const totalProgram, bool const forceUpdate = false);

    bool getEnsembleName(char* const ensembleName, uint16_t const size, uint32_t const programIndex, bool const forceUpdate = false);
    bool getServiceName(char* const serviceName, uint16_t const size, uint32_t const programIndex, bool const forceUpdate = false);

    bool getPresetFM(uint8_t const presetIndex, uint32_t* const frequency, bool const forceUpdate = false);
    bool setPresetFM(uint8_t const presetIndex, uint32_t const frequency);

};

extern ExtendedT4B t4b;