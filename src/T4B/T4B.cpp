
#include "T4B.h"
#include "Nextion/Nextion.h"
#include "DataManager/UserDataManager.h"

T4B t4b(&Serial1, 12);

static const uint8_t HeaderSizeIndex = 5U;
static const uint8_t ResponseAck = 0x00;
static const uint8_t ResponseAckNack = 0x02;
static const uint8_t UnusedPin = 255;
static const uint32_t FmFrequencyMin = 87500U;
static const uint32_t FmFrequencyMax = 108000U;

T4B::T4B(HardwareSerial *serial, uint8_t resetPin)
{
    _serial = serial;
    _pinReset = resetPin;
}

T4B::~T4B()
{
}

void T4B::Init(ulong baud)
{
    _serial->begin(baud, 134217756U, 26, 27);
    _serial->setTimeout(50U);

    pinMode(_pinReset, OUTPUT);
    digitalWrite(_pinReset, LOW);
    delay(100U);
    digitalWrite(_pinReset, HIGH);
    delay(1500);

    while (!Ready())
    {
        delay(500U);
        Serial.println("T4B isn't ready");
    }
    Serial.println("T4B is ready.");
}

void T4B::Loop()
{
    _NotificationReceive();

    StreamStatus streamStatus;
    EventType eventType;
    if(!getPlayStatus(&streamStatus, &eventType)) return;

    if(eventType == EventType::NONE) return;

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

    }

    if ( ( eventType & EventType::SortChange ) > 0 ) {

    }

    if ( ( eventType & EventType::FrequencyChange ) > 0 ) {
        uint8_t freqIndex;
        getFrequency(currentFreqOrProgIndex, &freqIndex);
        Serial.printf("Frequency Changed : %u.\n", freqIndex);
    }

    if ( ( eventType & EventType::TimeChange ) > 0 ) {

    }

}

CmdErrorCode T4B::getError()
{
    return _cmdError;
}

// *************************
// ***** SYSTEM ************
// *************************

/*
 *   Test for DAB module is ready for communication
 */
bool T4B::Ready()
{
    Command command = _commandBuilder.createSystem(CmdSystemId::Ready).build();
    return _commandSend(command);
}

/*
 *   Reset module.
 *   FullReset => Reset module database & module.
 */
bool T4B::Reset(bool const fullReset)
{
    Command command =
        _commandBuilder.createSystem(CmdSystemId::Reset).append(static_cast<uint8_t>(fullReset)).build();

    if (_commandSend(command))
    {
        init();
        return true;
    }
    return false;
}

/*
 *   Get module version.
 */
bool T4B::getModuleVersion(uint8_t *const version)
{
    Command command = _commandBuilder.createSystem(CmdSystemId::GetModuleVersion).build();
    return _commandSend(command) && _responseUint8(0U, version);
}

// *************************
// ***** STREAM ************
// *************************

/*
 *   Play DAB program
 *   programIndex = 1..999999 (see programs index)
 */
bool T4B::PlayDab(uint32_t const stationId)
{
    Command command = _commandBuilder.createStream(CmdStreamId::Play)
                          .append(static_cast<uint8_t>(StreamMode::Dab))
                          .append(stationId)
                          .build();

    return _commandSend(command);
}

/*
 *   Play FM program
 *   frequency = 87500..108000 (MHz)
 */
bool T4B::PlayFm(uint32_t const frequency)
{
    if(frequency < FmFrequencyMin || frequency > FmFrequencyMax) return false;
    Command command = _commandBuilder.createStream(CmdStreamId::Play)
                          .append(static_cast<uint8_t>(StreamMode::Fm))
                          .append(frequency)
                          .build();

    return _commandSend(command);
}

/*
 *   Play single tone
 *   tone = 1 ... 20 (kHz)
 */
bool T4B::PlayeSingleTone(uint32_t const tone)
{
    Command command = _commandBuilder.createStream(CmdStreamId::Play)
                          .append(static_cast<uint8_t>(StreamMode::SingleTone))
                          .append(tone)
                          .build();

    return _commandSend(command);
}

/*
 *   Stop.
 */
bool T4B::Stop()
{
    Command command = _commandBuilder.createStream(CmdStreamId::Stop).build();
    return _commandSend(command);
}

/*
 * Seek FM program.
 */
bool T4B::FmSearch(bool const searchForward)
{
    Command command = _commandBuilder.createStream(CmdStreamId::SearchFm)
                          .append(static_cast<uint8_t>(searchForward))
                          .build();

    return _commandSend(command);
}

/*
 * Search DAB bands for programs.
 */
bool T4B::DabSearch(DabBand const band)
{
    _commandBuilder.createStream(CmdStreamId::SearchDab);

    switch (band)
    {
        case DabBand::BandIII:
            _commandBuilder.append(static_cast<uint8_t>(0U));
            _commandBuilder.append(static_cast<uint8_t>(40U));
            break;
        case DabBand::ChinaBand:
            _commandBuilder.append(static_cast<uint8_t>(41U));
            _commandBuilder.append(static_cast<uint8_t>(71U));
            break;
        case DabBand::LBand:
            _commandBuilder.append(static_cast<uint8_t>(72U));
            _commandBuilder.append(static_cast<uint8_t>(94U));
            break;
    }

    Command command = _commandBuilder.build();
    return _commandSend(command);
}

bool T4B::setLRMode(LRMode const lrMode)
{
    Command command = _commandBuilder.createStream(CmdStreamId::SetLRMode).append(static_cast<uint8_t>(lrMode)).build();
    return _commandSend(command);
}

bool T4B::getPlayStatus(StreamStatus *const streamStatus, EventType *const eventType)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetPlayStatus).build();
    return (_commandSend(command) && _responseUint8(0U, reinterpret_cast<uint8_t*>(streamStatus)) && _responseUint16(1U, reinterpret_cast<uint16_t*>(eventType)));
}

bool T4B::getPlayMode(StreamMode* const streamMode)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetPlayMode).build();
    return (_commandSend(command) && _responseUint8(0U, reinterpret_cast<uint8_t*>(streamMode)));
}

bool T4B::getPlayIndex(uint32_t *const playIndex)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetPlayIndex).build();
    return (_commandSend(command) && _responseUint32(0U, playIndex));
}

bool T4B::getTotalProgram(uint32_t *const totalProgram)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetTotalProgram).build();
    return (_commandSend(command) && _responseUint32(0U, totalProgram));
}

bool T4B::getSearchProgram(uint8_t *const nbrDabFoundProgram)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetSearchProgram).build();
    return (_commandSend(command) && _responseUint8(0U, nbrDabFoundProgram));
}

bool T4B::getSignalStrength(uint8_t *const signalStrength, float *const error)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetSignalStrength).build();

    bool outValue = _commandSend(command);

    uint16_t errorRate = 0;
    bool errorBool = _responseUint16(1U, &errorRate);

    if(errorRate == 0xFFFF || errorRate == 0x00FF || errorRate == 0x0000) *error = -1;
    else if(errorRate == 0xFF00) *error = 0;
    else {
        float firstValue = powf(10, ( errorRate & 0xFF ) / 100.0);
        float secondValue = 1/pow(10, ( ( errorRate >> 8 ) & 0xFF ) );
        *error = firstValue * secondValue;
    }

    return (outValue && errorBool && _responseUint8(0U, signalStrength));
}

bool T4B::getStereo(StereoMode *const stereoMode)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetStereo).build();
    return (_commandSend(command) && _responseUint8(0U, reinterpret_cast<uint8_t*>(stereoMode)));
}

bool T4B::setStereoMode(bool const stereoMode)
{
    Command command = _commandBuilder.createStream(CmdStreamId::SetStereoMode).append(stereoMode).build();
    return _commandSend(command);
}

bool T4B::getStereoMode(bool *const stereoMode)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetStereoMode).build();
    return (_commandSend(command) && _responseBool(0U, stereoMode));
}

bool T4B::setVolume(uint8_t const volume)
{
    uint8_t volumeValue = (volume > 16U) ? 16U : volume;
    Command command = _commandBuilder.createStream(CmdStreamId::SetVolume).append(volumeValue).build();
    return _commandSend(command);
}

bool T4B::getVolume(uint8_t *const volume)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetVolume).build();
    return (_commandSend(command) && _responseUint8(0U, volume));
}

bool T4B::setPreset(StreamMode const streamMode, uint8_t const presetIndex, uint32_t const data)
{
    if(streamMode == StreamMode::Dab) {

    } else if (streamMode == StreamMode::Fm) {
        if(data < FmFrequencyMin || data > FmFrequencyMax) return false;
    } else return false;

    if(presetIndex > 9) return false;

    Command command = _commandBuilder.createStream(CmdStreamId::SetPreset)
                        .append(static_cast<uint8_t>(streamMode))
                        .append(presetIndex)
                        .append(data)
                        .build();

    return _commandSend(command);
}

bool T4B::setPresetDAB(uint8_t const presetIndex, uint32_t const programIndex)
{
    return setPreset(StreamMode::Dab, presetIndex, programIndex);
}

bool T4B::setPresetFM(uint8_t const presetIndex, uint32_t const frequency)
{
    return setPreset(StreamMode::Fm, presetIndex, frequency);
}

bool T4B::getPreset(StreamMode const streamMode, uint8_t const presetIndex, uint32_t *const data)
{
    if(streamMode != StreamMode::Dab && streamMode != StreamMode::Fm) return false;
    if(presetIndex > 9) return false;

    Command command = _commandBuilder.createStream(CmdStreamId::GetPreset)
                        .append(static_cast<uint8_t>(streamMode))
                        .append(presetIndex)
                        .build();

    return (_commandSend(command) && _responseUint32(0U, data));
}

bool T4B::getPresetDAB(uint8_t const presetIndex, uint32_t *const programIndex)
{
    return getPreset(StreamMode::Dab, presetIndex, programIndex);
}

bool T4B::getPresetFM(uint8_t const presetIndex, uint32_t *const frequency)
{
    return getPreset(StreamMode::Fm, presetIndex, frequency);
}

bool T4B::setHeadroom(uint8_t const headroomLevel)
{
    uint8_t headroomLevelValue = (headroomLevel > 12U) ? 12U : headroomLevel;
    Command command = _commandBuilder.createStream(CmdStreamId::SetHeadroom).append(headroomLevelValue).build();

    return _commandSend(command);
}

bool T4B::getHeadroom(uint8_t *const headroomLevel)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetHeadroom).build();
    return (_commandSend(command) && _responseUint8(0U, headroomLevel));
}

bool T4B::getProgrameNameFM(char *const programeName, uint16_t const size)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetProgrameName).append(0xFFFFFFFF).append(false).build();
    return _commandSend(command) && _responseText(programeName, size);
}

bool T4B::getProgrameNameDAB(char* const programeName, uint16_t const size, uint32_t programIndex, bool AbbreviatedName)
{
    uint32_t totalProg;

    if(!getTotalProgram(&totalProg)) return false;
    if(programIndex >= totalProg) return false;

    Command command = _commandBuilder.createStream(CmdStreamId::GetProgrameName).append(programIndex).append(!AbbreviatedName).build();
    return _commandSend(command) && _responseText(programeName, size);
}

bool T4B::getProgrameText(char *const programeText, uint16_t const size)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetProgrameText).build();
    return _commandSend(command) && _responseText(programeText, size);
}

bool T4B::getEnsembleName(char* const ensembleName, uint16_t const size, uint32_t const programIndex, bool const AbbreviatedName)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetEnsembleName).append(programIndex).append(!AbbreviatedName).build();
    return _commandSend(command) && _responseText(ensembleName, size);
}

bool T4B::getServiceName(char* const serviceName, uint16_t const size, uint32_t const programIndex, bool const AbbreviatedName)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetServiceName).append(programIndex).append(!AbbreviatedName).build();
    return _commandSend(command) && _responseText(serviceName, size);
}

bool T4B::getFrequency(uint32_t const programIndex, uint8_t* const freqIndex)
{
    Command command = _commandBuilder.createStream(CmdStreamId::GetFrequency).append(programIndex).build();
    return (_commandSend(command) && _responseUint8(0U, freqIndex));
}

bool T4B::setNotification(NotificationType const notificationType)
{
    Command command = _commandBuilder.createNotification(CmdNotificationId::SetNotification).append(static_cast<uint16_t>(notificationType)).build();
    return _commandSend(command);
}

bool T4B::getNotification(NotificationType *const notificationType)
{
    Command command = _commandBuilder.createNotification(CmdNotificationId::GetNotification).build();
    return _commandSend(command) && _responseUint16(0U, reinterpret_cast<uint16_t*>(notificationType));
}

bool T4B::setFunction(uint8_t gpioIndex, GpioFunction gpioFunction, GpioDrivingStrength gpioDrivingStrength)
{
    Command command = _commandBuilder.createGPIO(CmdGpioId::SetFunction)
                        .append(gpioIndex)
                        .append(static_cast<uint8_t>(gpioFunction))
                        .append(static_cast<uint8_t>(gpioDrivingStrength))
                        .build();
    return _commandSend(command);
}

bool T4B::EnableI2S(bool enable)
{
    if(!setFunction(43, GpioFunction::I2S_DATA_OUT, GpioDrivingStrength::DRIVE_8MA))
		return false;
	if(!setFunction(55, GpioFunction::I2S_LRCLK_OUT, GpioDrivingStrength::DRIVE_8MA))
		return false;
	if(!setFunction(54, GpioFunction::I2S_SSCLK_OUT, GpioDrivingStrength::DRIVE_8MA))
		return false;
	if(!setFunction(53, GpioFunction::I2S_BCLK_OUT, GpioDrivingStrength::DRIVE_8MA))
		return false;
	return true;
}

bool T4B::_processNotification()
{
    if(_responseHeader[0x01] != static_cast<uint8_t>(CommandType::Notification)) return false;

    NotificationType eventType = static_cast<NotificationType>(1 << _responseHeader[2]);

    Serial.printf("Process Notification : %u.\n", eventType);
    uint32_t freq;
    uint8_t dabIndex;
    StreamMode curentStreaMode;
    char buffer[T4BMaxTextSize];
    switch (eventType)
    {
        case NotificationType::ScanFinished:
            if(!getPlayIndex(&freq)) break;

            if(!getPlayMode(&curentStreaMode)) break;

            if(curentStreaMode == StreamMode::Dab) {
                Serial.printf("Playing program index : %u.\n", freq);
                userDataManager.setLastDabProgramIndex(freq);
                userDataManager.Save();
                if(getProgrameNameDAB(buffer, sizeof(buffer), freq)) nextion.setTitle(buffer);
                else nextion.setTitle("");
                nextion.setArtist("");
            } else if(curentStreaMode == StreamMode::Fm) {
                Serial.printf("Playing Frequence : %u.\n", freq);
                userDataManager.setLastFmFreq(freq);
                userDataManager.Save();
                if(getProgrameNameFM(buffer, sizeof(buffer))) nextion.setTitle(buffer);
                else nextion.setTitle("");
                nextion.setArtist("");
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
            if(_responseUint32(0U, &freq)) {
                Serial.printf("Searching Frequence : %u.\n", freq);
            }
            else if(_responseUint8(0U, &dabIndex)) Serial.printf("Searching DAB index : %u.\n", dabIndex);
            break;    

        default:
            break;
    }

    return true;
}

bool T4B::_commandSend(Command const &command)
{

    _cmdError = CmdErrorCode::NONE;

    while (_serial->available())
    {
        _serial->read();
    }
    _serial->write(command.data, command.size);
    _serial->flush();

    bool v = (_responseReceive() &&
            !(_responseHeader[1U] == ResponseAck && _responseHeader[2U] == ResponseAckNack));

    if(v) return true;

    _cmdError = CmdErrorCode::TimeOut;
    _responseUint8(0, reinterpret_cast<uint8_t*>(&_cmdError));

    debug.printlnError("T4B : " + String(ToString(_cmdError)));

    return false;
}



// bool T4B::_commandSend(Command const &command, CmdErrorCode *const error)
// {
//     *error = CmdErrorCode::TimeOut;
//     if(_commandSend(command)) return true;
//     _responseUint8(0, reinterpret_cast<uint8_t*>(error));
//     return false;
// }

bool T4B::_responseReceive()
{
    uint16_t index = 0U;
    uint32_t endMillis = millis() + 250U; // timeout for answer from module = 200ms.
    _responseSize = 0U;

    while (millis() < endMillis && index < T4BMaxDataSize)
    {
        if(!_serial->available()) continue;
        if(_processRecievedData(index)) return true;
    }
    return false;
}

bool T4B::_NotificationReceive()
{
    uint16_t index = 0U;
    unsigned long endMillis = millis() + 1U; // timeout for next byte of the notification from module.
    _responseSize = 0U;

    while (millis() < endMillis && index < T4BMaxEventSize)
    {
        if(!_serial->available()) continue;
        endMillis = millis() + 1U;
        if(_processRecievedData(index)) return true;
    }
    return false;
}

bool T4B::_processRecievedData(uint16_t &index)
{
    uint8_t data = 0U;
    data = _serial->read();
    if (data == CommandStartValue)
    {
        index = 0U;
    }

    if (index < T4BHeaderSize)
    {
        _responseHeader[index] = data;

        if (index == HeaderSizeIndex)
        {
            _responseSize = static_cast<uint16_t>(_responseHeader[5U]);
            _responseSize |= static_cast<uint16_t>(_responseHeader[4U]) << 8U;
        }
    }
    else if ((index - T4BHeaderSize) < _responseSize)
    {
        _response[index - T4BHeaderSize] = data;
    }

    if (data == CommandEndValue)
    {
        if ((index - T4BHeaderSize - _responseSize) == 0U)
        {
            if(_responseHeader[1] != static_cast<uint8_t>(CommandType::Notification)) return true;
                
            if(_responseHeader[2] == 0x01 && _responseSize > 0) return true;

            _processNotification();

            return _responseReceive();

            // return true;
        }
    }
    index++;
    return false;
}

bool T4B::_responseText(char* const buffer, uint16_t const size)
{
    // if(buffer) free(buffer);
    // char iBuffer[size];
    uint16_t j = 0U;
    for (uint16_t i = 0U; i < _responseSize; i = i + 2U)
    {
        if (size <= j) return false;
        buffer[j++] = _uint16ToChar(_response[i], _response[i + 1U]);;
    }
    _discardTrailingSpaces(buffer);
    return true;
    // buffer = iBuffer;
    return true;
}

bool T4B::_responseUint8(uint8_t const index, uint8_t *const resp)
{
    if (_responseSize < index) return false;

    *resp = _response[index];
    return true;

}

bool T4B::_responseUint16(uint8_t const index, uint16_t *const resp)
{
    if (_responseSize < (index + 1U)) return false;
    
    *resp = static_cast<uint16_t>(_response[index + 1U]);
    *resp |= static_cast<uint16_t>(_response[index]) << 8U;
    return true;

}

bool T4B::_responseUint32(uint8_t const index, uint32_t *const resp)
{
    if (_responseSize < (index + 3U)) return false;
    
    *resp = static_cast<uint32_t>(_response[index + 3U]);
    *resp |= static_cast<uint32_t>(_response[index + 2U]) << 8U;
    *resp |= static_cast<uint32_t>(_response[index + 1U]) << 16U;
    *resp |= static_cast<uint32_t>(_response[index + 0U]) << 24U;
    return true;

}

bool T4B::_responseBool(uint8_t const index, bool *const resp)
{
    return _responseUint8(index, reinterpret_cast<uint8_t*>(resp));
}

char T4B::_uint16ToChar(uint8_t const byte1, uint8_t const byte0)
{
    if (byte1 == 0x00)
    {
        if (byte0 < 128U)
        {
            return byte0;
        }

        switch (byte0)
        {
            case 0x8A:
                return 'S';
            case 0x8C:
                return 'S';
            case 0x8D:
                return 'T';
            case 0x8E:
                return 'Z';
            case 0x8F:
                return 'Z';
            case 0x9A:
                return 's';
            case 0x9D:
                return 't';
            case 0x9E:
                return 'z';
            case 0xC0:
                return 'A';
            case 0xC1:
                return 'A';
            case 0xC2:
                return 'A';
            case 0xC3:
                return 'A';
            case 0xC4:
                return 'A';
            case 0xC5:
                return 'A';
            case 0xC7:
                return 'C';
            case 0xC8:
                return 'E';
            case 0xC9:
                return 'E';
            case 0xCA:
                return 'E';
            case 0xCB:
                return 'E';
            case 0xCC:
                return 'I';
            case 0xCD:
                return 'I';
            case 0xCE:
                return 'I';
            case 0xCF:
                return 'I';
            case 0xD0:
                return 'D';
            case 0xD1:
                return 'N';
            case 0xD2:
                return 'O';
            case 0xD3:
                return 'O';
            case 0xD4:
                return 'O';
            case 0xD5:
                return 'O';
            case 0xD6:
                return 'O';
            case 0xD8:
                return 'O';
            case 0xD9:
                return 'U';
            case 0xDA:
                return 'U';
            case 0xDB:
                return 'U';
            case 0xDC:
                return 'U';
            case 0xDD:
                return 'Y';
            case 0xE0:
                return 'a';
            case 0xE1:
                return 'a';
            case 0xE2:
                return 'a';
            case 0xE3:
                return 'a';
            case 0xE4:
                return 'a';
            case 0xE5:
                return 'a';
            case 0xE7:
                return 'c';
            case 0xE8:
                return 'e';
            case 0xE9:
                return 'e';
            case 0xEA:
                return 'e';
            case 0xEB:
                return 'e';
            case 0xEC:
                return 'i';
            case 0xED:
                return 'i';
            case 0xEE:
                return 'i';
            case 0xEF:
                return 'i';
            case 0xF1:
                return 'n';
            case 0xF2:
                return 'o';
            case 0xF3:
                return 'o';
            case 0xF4:
                return 'o';
            case 0xF5:
                return 'o';
            case 0xF6:
                return 'o';
            case 0xF9:
                return 'u';
            case 0xFA:
                return 'u';
            case 0xFB:
                return 'u';
            case 0xFC:
                return 'u';
            case 0xFD:
                return 'y';
            case 0xFF:
                return 'y';
        }
    }
    else if (byte1 == 0x01)
    {
        switch (byte0)
        {
            case 0x1B:
                return 'e'; // ě
            case 0x48:
                return 'n'; // ň
            case 0x59:
                return 'r'; // ř
            case 0x0D:
                return 'c'; // č
            case 0x7E:
                return 'z'; // ž
            case 0x0C:
                return 'C'; // Č
        }
    }

    return 0x00;
}

void T4B::_discardTrailingSpaces(char *const text)
{
    int16_t index = strlen(text) - 1;

    while (index >= 0U && text[index] == ' ')
    {
        index--;
    }

    text[++index] = '\0';
}