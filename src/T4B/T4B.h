#pragma once
#include <HardwareSerial.h>
#include "SoftwareSerial.h"
#include "CommandBuilder.h"
#include "pDebug/pDebug.h"
#include "Preprocessor Macros/ENUM_CLASS_FLAGS.h"

static uint16_t const T4BMaxTextSize = 128U;
static uint16_t const T4BMaxDataSize = 2 * T4BMaxTextSize;
static uint16_t const T4BMaxEventSize = 22;
static uint8_t const T4BHeaderSize = 6U;

enum class DabBand : uint8_t
{
    BandIII = 0x01,
    ChinaBand = 0x02,
    LBand = 0x03
};

enum class StreamStatus : uint8_t
{
    Playing,
    Searching,
    Tuning,
    Stopped,
    // Sorting,
    // Reconfiguring
};

enum class StreamMode : uint8_t
{
    Dab = 0x00,
    Fm = 0x01,
    I2Sin = 0x02,
    Beep = 0x03,
    SingleTone = 0x04,
    Noise = 0x05,
    LineIn1 = 0x06,
    LineIn2 = 0x07,
    None = 0xFF
};

enum class LRMode : uint8_t
{
    LR = 0,
    L_Only = 1,
    R_Only = 2,
};

enum class StereoMode : uint8_t
{
    Stereo = 0,
    JointStereo = 1,
    DualChannel = 2,
    SingleChannel = 3
};

enum class EventType : uint16_t
{
    NONE = 0,
    ProgramNameChange = 1,
    ProgramTextChange = 2,
    DLSCommandChange = 4,
    StereoModeChange = 8,
    ServiceUpdate = 16,
    SortChange = 32,
    FrequencyChange = 64,
    TimeChange = 128
};
ENUM_CLASS_FLAGS(EventType);

/* Notification bitmask
BIT0: Scan finished notification
BIT1: Got new FM program text notification 
BIT2: DAB reconfiguration notification
BIT3: DAB channel list order change notification
BIT4: RDS group notification
BIT5: Got new DAB radio text (DL+ type) notification
BIT6: Scan frequency notification
*/
enum class NotificationType : uint16_t
{
    ScanFinished = 1,
    NewFmProgrameText = 2,
    DABReconfiguration = 4,
    DABChannelListOrderChange = 8,
    RDSGroup = 16,
    NewDABRadioText = 32,
    ScanFrequency = 64
};
ENUM_CLASS_FLAGS(NotificationType);

enum class GpioFunction : uint8_t
{   
    DATA_IN = 0,
    DATA_OUT = 1,
    MUTE = 2,
    I2S_DATA_IN = 3,
    I2S_LRCLK_IN = 4,
    I2S_SSCLK_IN = 5,
    I2S_BCLK_IN = 6,
    I2S_DATA_OUT = 7,
    I2S_LRCLK_OUT = 8,
    I2S_SSCLK_OUT = 9,
    I2S_BCLK_OUT = 10,
    SPDIF_OUT = 11,
    SPI_DO = 12,
    SPI_CLK = 13,
    SPI_CS = 14,
    SPI_DI = 15
};

enum class GpioDrivingStrength : uint8_t
{
    DRIVE_2MA = 0,
    DRIVE_4MA = 1,
    DRIVE_6MA = 2,
    DRIVE_8MA = 3
};

class T4B
{

public:
    T4B(HardwareSerial *serial, uint8_t resetPin);
    ~T4B();

    void Init(ulong baud = 115200);
    void Loop();

    CmdErrorCode getError();

    // *************************
    // ***** SYSTEM ************
    // *************************

    bool Ready();
    bool Reset(bool const fullReset = false);
    bool getModuleVersion(uint8_t *const version);


    // *************************
    // ***** STREAM ************
    // *************************

    // bool PlayDab(uint32_t const stationId, CmdErrorCode *const error);
    bool PlayDab(uint32_t const stationId);
    // bool PlayFm(uint32_t const frequency, CmdErrorCode *const error);
    bool PlayFm(uint32_t const frequency);
    // bool PlayI2Sin();
    bool PlayeSingleTone(uint32_t const tone);
    // bool PlayNoise();
    // bool PlayBeep(uint8_t const beeperNum, uint16_t const freq1, uint16_t const freq2, uint16_t const beeperTime, uint16_t const slientTime);
    bool Stop();
    bool FmSearch(bool const searchForward = true);
    bool DabSearch(DabBand const band = DabBand::BandIII);

    bool setLRMode(LRMode const lrMode = LRMode::LR);
    bool getPlayStatus(StreamStatus* const streamStatus, EventType* const eventType);
    bool getPlayMode(StreamMode* const streamMode);

    /// @brief  Get current DAB program index.
    ///         If current stream mode is FM,  return RF frequency.
    /// @param playIndex :  If the stream mode is 0 (DAB), this field contains the DAB program index.
    ///                     If the stream mode is 1 (FM), this field contains the RF frequency in kHz (87500 ~108000).		
    /// @return true is succes, false if error.
    bool getPlayIndex(uint32_t* const playIndex);
    bool getTotalProgram(uint32_t* const totalProgram);
    bool getSearchProgram(uint8_t* const nbrDabFoundProgram);
    bool getSignalStrength(uint8_t* const signalStrength, float* const error);
    bool getStereo(StereoMode* const stereoMode );

    bool setStereoMode(bool const stereoMode = true);
    bool getStereoMode(bool* const stereoMode);
    bool setVolume(uint8_t const volume);
    bool getVolume(uint8_t* const volume);
    bool setPresetDAB(uint8_t const presetIndex, uint32_t const programIndex);
    bool setPresetFM(uint8_t const presetIndex, uint32_t const frequency);
    bool getPresetDAB(uint8_t const presetIndex, uint32_t* const programIndex);
    bool getPresetFM(uint8_t const presetIndex, uint32_t* const frequency);

    bool setHeadroom(uint8_t const headroomLevel);
    bool getHeadroom(uint8_t* const headroomLevel);

    bool getProgrameNameFM(char* const programeName, uint16_t const size);
    bool getProgrameNameDAB(char* const programeName, uint16_t const size, uint32_t programIndex, bool const AbbreviatedName = false);
    bool getProgrameText(char* const programeText, uint16_t const size);

    bool getEnsembleName(char* const ensembleName, uint16_t const size, uint32_t const programIndex, bool const AbbreviatedName = false);
    bool getServiceName(char* const serviceName, uint16_t const size, uint32_t const programIndex, bool const AbbreviatedName = false);
    bool getFrequency(uint32_t const programIndex, uint8_t* const freqIndex);

    bool setNotification(NotificationType const notificationType);
    bool getNotification(NotificationType* const notificationType);

    bool setFunction(uint8_t gpioIndex, GpioFunction gpioFunction, GpioDrivingStrength gpioDrivingStrength);

    bool EnableI2S(bool enable = true);


private:

    HardwareSerial *_serial;
    uint8_t _pinReset;

    CommandBuilder _commandBuilder;
    uint8_t _response[T4BMaxDataSize];
    uint8_t _responseHeader[T4BHeaderSize];
    uint16_t _responseSize = 0U;
    CmdErrorCode _cmdError = CmdErrorCode::NONE;

    bool setPreset(StreamMode const streamMode, uint8_t const presetIndex, uint32_t const data );
    bool getPreset( StreamMode const streamMode, uint8_t const presetIndex, uint32_t* const data );

    bool _processNotification();
    bool _commandSend(Command const& command);
    // bool _commandSend(Command const &command, CmdErrorCode *const error);
    bool _responseReceive();
    bool _NotificationReceive();
    bool _processRecievedData(uint16_t &index);
    bool _responseText(char* buffer, uint16_t const size);
    bool _responseUint8(uint8_t const index, uint8_t* const resp);
    bool _responseUint16(uint8_t const index, uint16_t* const resp);
    bool _responseUint32(uint8_t const index, uint32_t* const resp);
    bool _responseBool(uint8_t const index, bool* const resp);
    char _uint16ToChar(uint8_t const byte0, uint8_t const byte1);
    void _discardTrailingSpaces(char* const text);
};

extern T4B t4b;