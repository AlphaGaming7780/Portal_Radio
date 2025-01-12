#pragma once
#include <HardwareSerial.h>
#include "SoftwareSerial.h"
#include "CommandBuilder.h"
#include "pDebug/pDebug.h"

static uint16_t const T4BMaxTextSize = 128U;
static uint16_t const T4BMaxDataSize = 2 * T4BMaxTextSize;
static uint8_t const T4BHeaderSize = 6U;

enum class DabBand : uint8_t
{
    BandIII = 0x01,
    ChinaBand = 0x02,
    LBand = 0x03
};

enum class StreamState : uint8_t
{
    Playing,
    Searching,
    Tuning,
    Stopped,
    Sorting,
    Reconfiguring
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

class T4B
{

public:
    T4B(HardwareSerial *serial, uint8_t resetPin);
    ~T4B();

    void Init(ulong baud = 115200);

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


    bool setVolume(uint8_t const volume);
    bool getVolume(uint8_t* const volume);

    bool setHeadroom(uint8_t const headroomLevel);
    bool getHeadroom(uint8_t* const headroomLevel);

private:

    HardwareSerial *_serial;
    uint8_t _pinReset;

    CommandBuilder _commandBuilder;
    uint8_t _response[T4BMaxDataSize];
    uint8_t _responseHeader[T4BHeaderSize];
    uint16_t _responseSize = 0U;
    CmdErrorCode _cmdError = CmdErrorCode::NONE;

    bool _commandSend(Command const& command);
    // bool _commandSend(Command const &command, CmdErrorCode *const error);
    bool _responseReceive();
    bool _responseText(char* const buffer, uint16_t const size);
    bool _responseUint8(uint8_t const index, uint8_t* const resp);
    bool _responseUint16(uint8_t const index, uint16_t* const resp);
    bool _responseUint32(uint8_t const index, uint32_t* const resp);
    char _uint16ToChar(uint8_t const byte0, uint8_t const byte1);
    void _discardTrailingSpaces(char* const text);
};

extern T4B t4b;