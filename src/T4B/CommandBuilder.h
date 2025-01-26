#ifndef CommandBuilder_h
#define CommandBuilder_h

#include "Command.h"
// #include <fmt/format.h>
#include <WString.h>


enum class CommandType : uint8_t
{
    System = 0x00,
    Stream = 0x01,
    Rtc = 0x02,
    MOT = 0x03,
    Notification = 0x07,
    GPIO = 0x08,
};

enum class CmdSystemId : uint8_t
{
    Ready = 0x00,
    Reset = 0x01,
    GetMCUVersion = 0x02,
    GetBootVersion = 0x03,
    GetASPVersion = 0x04,
    GetAllVersion = 0x05,
    GetModuleVersion = 0x06
};

enum class CmdStreamId : uint8_t
{
    Play = 0x00,
    Stop = 0x02,
    SearchFm = 0x03,
    SearchDab = 0x04,
    SearchStop = 0x05,
    SetLRMode = 0x06,
    GetPlayStatus = 0x10,
    GetPlayMode = 0x11,
    GetPlayIndex = 0x12,
    GetTotalProgram = 0x13,
    GetSearchProgram = 0x14,
    GetSignalStrength = 0x15,
    GetStereo = 0x16,
    
    SetStereoMode = 0x20,
    GetStereoMode = 0x21,
    SetVolume = 0x22,
    GetVolume = 0x23,
    SetPreset = 0x24,
    GetPreset = 0x25,
    // StationType = 0x0E,
    // DabStationName = 0x0F,
    // DabStationText = 0x10,
    // SampleRate = 0x11,
    // DabDataRate = 0x12,
    // DabSignalQuality = 0x13,
    // DabFrequency = 0x14,
    // DabEnsembleName = 0x15,
    // DabStationCount = 0x16,
    // DabStationOnAir = 0x17,
    // DabStationServiceName = 0x1A,
    // DabFoundStationsCount = 0x1B,
    // DabStationType = 0x1E,
    // MemorySet = 0x21,
    // MemoryGet = 0x22,
    // DabStationInfo = 0x23,
    // DabSortGet = 0x24,
    // DabSortSet = 0x25,
    // DabDrcGet = 0x26,
    // DabDrcSet = 0x27,
    SetHeadroom = 0x28,
    GetHeadroom = 0x29,
    SetSorter = 0x2A,
    GetSorter = 0x2B,
    GetProgrameType = 0x2C,
    GetProgrameName = 0x2D,
    GetProgrameText = 0x2E,

    // FmStereoThresholdLevelSet = 0x30,
    // FmStereoThresholdLevelGet = 0x31,
    // FmRdsData = 0x32,
    // FmSeekThresholdSet = 0x35,
    // FmSeekThresholdGet = 0x36,
    // FmStereoThresholdSet = 0x37,
    // FmStereoThresholdGet = 0x38,
    // FmExactStation = 0x39
    GetEnsembleName = 0x41,
    GetServiceName = 0x42,

    IsActive = 0x44,

    GetFrequency = 0x46
};

enum class CmdRtcId : uint8_t
{
    Set = 0x00,
    Get = 0x01,
    Sync = 0x02,
    SyncStatus = 0x03,
    StatusClock = 0x04
};

enum class CmdNotificationId : uint8_t
{
    SetNotification = 0x00,
    GetNotification = 0x01
};

enum class CmdGpioId : uint8_t
{
    SetFunction = 0x00,
    SetLevel = 0x01,
    GetLevel = 0x02,
};

enum class CmdErrorCode : uint8_t
{
    NONE = 0x00,
    TimeOut = 0x01,
    Unknown = 0xFF,
    IndexOutOfRange = 0xFE,
    NoSupportForThisMode = 0xFD,
    NoSupportForThisPlayStatus = 0xFC,
    NoSupportForThisFW = 0xFB,
    NoData = 0xFA,
    DataNotUpdate = 0xF9,
    ParameterIncorrect = 0xF8,
    StationFull = 0xF7
};

inline const char* ToString(CmdErrorCode v)
{
    String s = "No CmdErrorCode matching : " + String(static_cast<uint8_t>(v)) + ".";
    switch (v)
    {
        case CmdErrorCode::NONE:                        return "NONE";
        case CmdErrorCode::TimeOut:                     return "TimeOut : Didn't got any response back.";
        case CmdErrorCode::Unknown:                     return "Unknown : This command not recognized.";
        case CmdErrorCode::IndexOutOfRange:             return "IndexOutOfRange : The index may be out of range.";
        case CmdErrorCode::NoSupportForThisMode:        return "NoSupportForThisMode : This command can't use in current play mode.";
        case CmdErrorCode::NoSupportForThisPlayStatus:  return "NoSupportForThisPlayStatus : This command can't use in current play status.";
        case CmdErrorCode::NoSupportForThisFW:          return "NoSupportForThisFW : This command can't use in current firmware.";
        case CmdErrorCode::NoData:                      return "NoData : The data is not ready for use.";
        case CmdErrorCode::DataNotUpdate:               return "DataNotUpdate : The data not update.";
        case CmdErrorCode::ParameterIncorrect:          return "ParameterIncorrect : The input parameter incorrect.";
        case CmdErrorCode::StationFull:                 return "StationFull : The total number of programs reaches the maximum.";
        default:                                        return s.c_str();
    }
}

class CommandBuilder
{
  public:
    CommandBuilder& createSystem(CmdSystemId const id);
    CommandBuilder& createStream(CmdStreamId const id);
    CommandBuilder& createRtc(CmdRtcId const id);
    CommandBuilder& createNotification(CmdNotificationId const id);
    CommandBuilder& createGPIO(CmdGpioId const id);

    CommandBuilder& append(uint8_t const value);
    CommandBuilder& append(uint16_t const value);
    CommandBuilder& append(uint32_t const value);

    CommandBuilder& append(bool const value);

    Command& build();

  private:
    CommandBuilder& create(CommandType const type, uint8_t const id);
    Command command;
};

#endif // CommandBuilder_h
