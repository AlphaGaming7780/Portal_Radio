#include "Nextion.h"

Nextion nextion(&Serial2);

Nextion::Nextion(HardwareSerial *serial)
{
    _serial = serial;
}

Nextion::~Nextion()
{
}

bool Nextion::Init(HardwareSerial *serial)
{
    _serial = serial;
    return true;
}

bool Nextion::Begin(ulong baudrate)
{
    if(_serial == nullptr) return false;
    _serial->begin(baudrate);
    _baudrate = baudrate;

    Sleep(false);
    Reset();

    return true;
}

void Nextion::StartupFinished()
{
    while (!_isReady)
    {
        Loop();
    }
    
    _print("thup=1");
    _print("thsp=60");
    setYear(2024);
    setMonth(11);
    setDay(18);
    setHour(8);
    setMinute(30);
    setSecond(30);
    _UpdatePendingData();
    _print("page 1");
    delay(1000);
    Loop();
}

void Nextion::Loop()
{
    while(_serial->available() != 0) {
        String s = _serial->readStringUntil(char(0xFF));

        if(s.length() == 0) continue;

        int x = s.indexOf(':');

        int si = s.toInt();

        if      ( s == _formatHexCodeToString(0x00) ) debug.printlnError("Invalid Commande");
        else if ( s == _formatHexCodeToString(0x01) ) debug.printlnInfo("Instruction Successful");
        else if ( s == _formatHexCodeToString(0x02) ) debug.printlnError("Invalid Component ID");
        else if ( s == _formatHexCodeToString(0x1A) ) debug.printlnError("Invalid Variable name or attribute");
        else if ( s == _formatHexCodeToString(0x1B) ) debug.printlnError("Invalid Variable Operation");
        else if ( s == _formatHexCodeToString(0x1C) ) debug.printlnError("Assignement operation failed");
        else if ( s == _formatHexCodeToString(0x1E) ) debug.printlnError("Invalid Quantity of Parameters");
        else if ( s == _formatHexCodeToString(0x20) ) debug.printlnError("Escape Character Invalid");
        else if ( s == _formatHexCodeToString(0x23) ) debug.printlnError("Variable name too long");
        else if ( s == _formatHexCodeToString(0x24) ) debug.printlnWarn("Serial Buffer Overflow");
        else if ( s == _formatHexCodeToString(0x86) ) { debug.printlnInfo("Auto sleep"); _isSleeping = true; }
        else if ( s == _formatHexCodeToString(0x87) ) { debug.printlnInfo("Auto wakeup"); _isSleeping = false; _UpdatePendingData(); }
        // else if ( s == _formatHexCodeToString(0x00) + _formatHexCodeToString(0x00) + _formatHexCodeToString(0x00) ) { debug.printlnInfo("Nextion Startup"); }
        else if ( s == _formatHexCodeToString(0x88) ) { debug.printlnInfo("Nextion Ready"); _isReady = true; }
        else if ( s == "play" ) audioManager.Play();
        else if ( s == "pause") audioManager.Pause();
        else if ( s == "next" ) audioManager.Next();
        else if ( s == "prev" ) audioManager.Previous();
        else if ( s == "mute" ) audioManager.Mute();
        else if ( s == "unmute") audioManager.UnMute();
        else if ( s == "getAlarmList") { SendAlarmList(alarmManager.getAlarmVector()); SelectAlarm(_selectedAlarmIndex); }
        else if ( s == "newAlarm" ) alarmManager.NewAlarm();
        else if ( s == "removeAlarm" ) alarmManager.RemoveAlarm(_selectedAlarmIndex);
        else if ( s == "saveAlarm" ) alarmManager.SaveAlarm(_selectedAlarmIndex);
        else if ( s == "getFmFreq" ) setFmFreq(FM.getFreq());
        else if ( s == "getFmPresets" ) SendFmPresets();
        else if ( s == "getDabPresets" ) SendDabPresets();
        else if ( s == "getDabStationsList" ) { uint32_t playIndex; t4b.getPlayIndex(&playIndex); SendDabStationList(playIndex); }
        else if ( x != -1 ) {
            String c = s.substring(0, x);
            String v = s.substring(x+1);
            debug.printlnInfo("Command : " + c);
            debug.printlnInfo("value : " + v);

            if ( c == "page" ) _selectedPage = v;
            else if ( c == "audioSource") _UpdatePendingAudioSource(v);
            else if ( c == "getAlarmData" ) { _selectedAlarmIndex = v.toInt(); SendAlarmData(alarmManager.getAlarm(_selectedAlarmIndex)); }
            else if ( c == "setAlarmHour" ) alarmManager.setAlarmHour(_selectedAlarmIndex, v.toInt());
            else if ( c == "setAlarmMin" ) alarmManager.setAlarmMinute(_selectedAlarmIndex, v.toInt());
            else if ( c == "setAlarmEnable" ) alarmManager.setAlarmEnabled( _selectedAlarmIndex, bool(v.toInt()));
            else if ( c == "setAlarmDay" ) alarmManager.setAlarmDay( _selectedAlarmIndex, dayOfWeekFromTimeInfoFormat(v.toInt()) );
            else if ( c == "setFmFreq") FM.setFreq(v.toInt());
            else if ( c == "playFmPreset") FM.playPreset(v.toInt());
            else if ( c == "setFmPreset" ) FM.setPreset(v.toInt());
            else if ( c == "setDabProgIndex" ) DAB.setProgIndex(t4b.getProgramIndexByServiceName(v.c_str()));
            else if ( c == "setDabSorter" ) { if(t4b.setSorter(static_cast<DabSorter>(v.toInt()))) SendDabStationList(); }
            else if ( c == "setDabFilter" ) DAB.setEnsembleIdFilter(v);

            else Serial.printf("Unknown command : %s, with value : %s.\n", c, v);
        }
        else Serial.printf("Unknown data : %s.\n", s);
    }
}

void Nextion::setAudioSource(String source)
{
    if(_isSleeping || !_isReady) {
        _pendingData.audioSource = source;
        return;
    }

    _serial->print("Audio.currentSource.txt=\"" + source + "\"" + _endChar);
    _serial->print("page Audio" + _endChar);
}

void Nextion::setVolume(int volume, bool isMuted)
{
    if(_isSleeping || !_isReady) {
        _pendingData.volume = volume;
        _pendingData.isMuted = isMuted ? BOOL3_TRUE : BOOL3_FLASE;
        return;
    }

    if(volume > 100) volume = 100;
    else if(volume < 0) volume = 0;

    _serial->print("Audio.audioVolume.val=" + String(volume) + _endChar);
    _serial->printf("Audio.audioMute.val=%i%s", isMuted, _endChar);

    if(!_isInAudioPage()) return;

    if(isMuted) {
        _serial->print("pVolume.pic=4" + _endChar);
        _serial->print("jVolume.val=0" + _endChar);
    } else {
        int pic = 0;
        if(volume == 0) pic = 5; 
        else if( volume <= 33) pic = 6;
        else if( volume <= 66) pic = 7;
        else if( volume <= 100) pic = 8;

        _serial->print("pVolume.pic=" + String(pic) + _endChar);
        _serial->print("jVolume.val=" + String(volume) + _endChar);
    }
}

void Nextion::setTitle(String title)
{
    if(_isSleeping || !_isReady) {
        _pendingData.title = title;
        return;
    }

    _serial->print("Audio.audioTitle.txt=\"" + title + "\"" + _endChar);
    if(_isInAudioPage()) _serial->print("gTitle.txt=\"" + title + "\"" + _endChar);
}

void Nextion::setArtist(String artist)
{
    if(_isSleeping || !_isReady) {
        _pendingData.artist = artist;
        return;
    }

    _serial->print("Audio.audioArtist.txt=\"" + artist + "\"" + _endChar);
    if(_isInAudioPage()) _serial->print("gArtist.txt=\"" + artist + "\"" + _endChar);
}

void Nextion::setPlayStatus(bool playStatus)
{
    if(_isSleeping || !_isReady) {
        _pendingData.playStatus = playStatus ? BOOL3_TRUE : BOOL3_FLASE;
        return;
    }

    if(playStatus) {
        _serial->print("Audio.playStatus.val=1" + _endChar);
        if(_isInAudioPage()) _serial->print("pPlay.pic=1" + _endChar);
    } else {
        _serial->print("Audio.playStatus.val=0" + _endChar);
        if(_isInAudioPage()) _serial->print("pPlay.pic=0" + _endChar);
    }
}

void Nextion::setMute(bool mute)
{   
    int vol = roundf(userDataManager.getVolume() * 100);
    setVolume(vol, mute);
}

void Nextion::setFmFreq(uint32_t freq)
{
    if(_selectedPage != "FM") return;
    _serial->printf("sFmNum4.val=%i%s", (freq/100) % 10, _endChar);
    _serial->printf("sFmNum3.val=%i%s", (freq/1000) % 10, _endChar);
    _serial->printf("sFmNum2.val=%i%s", (freq/10000) % 10, _endChar);
    _serial->printf("sFmNum1.val=%i%s", (freq/100000) % 10, _endChar);
}

void Nextion::SendFmPresets()
{
    if(_selectedPage != "FM") return;
    uint32_t freq = 0;
    for (int i = 0; i < 10; i++) {
        if(t4b.getPresetFM(i, &freq)) {
            _serial->printf("bPreset%i.txt=\"%.1f\"%s", i, freq/1000.0, _endChar);
            continue;
        }
        _serial->printf("bPreset%i.txt=\"%s\"%s", i, "None", _endChar);
    }
}

void Nextion::SendDabPresets()
{
    if(_selectedPage != "DAB") return;
    uint32_t programIndex = 0;
    for (int i = 0; i < 10; i++) {
        if(t4b.getPresetDAB(i, &programIndex)) {
            _serial->printf("bPreset%i.txt=\"%i\"%s", i, programIndex, _endChar);
            continue;
        }
        _serial->printf("bPreset%i.txt=\"%s\"%s", i, "None", _endChar);
    }
}

void Nextion::SendDabStationList(uint32_t programIndex)
{
    if(_selectedPage != "DAB") return;

    String ServiceNameList = "";
    String EnsembleNameList = "";

    Vector<String> ensembleList = t4b.getEnsembleIdList();

    Serial.printf("SendDabStationList : ensembleList lenght : %i.\n", ensembleList.size());

    String dabEnsembleNameFilter = DAB.getEnsembleIdFilter();

    if(dabEnsembleNameFilter == "null" || dabEnsembleNameFilter == emptyString) {
        dabEnsembleNameFilter = ensembleList[0];
        DAB.setEnsembleIdFilter(dabEnsembleNameFilter, false);
    }

    Vector<DabProgramInfo> currentDabProgramList = DAB.getCurrentProgramList();

    Serial.printf("SendDabStationList : _CurrentDabProgramList lenght : %i.\n", currentDabProgramList.size());

    bool notFound = true;
    int i = 0;
    for(auto obj : currentDabProgramList) {
        if ( i > 0 ) ServiceNameList += "\\r";
        ServiceNameList += obj.ServiceName;
        if(obj.ProgramIndex == programIndex) {
            notFound = false;
            _serial->printf("DabStationList.val=%i%s", i, _endChar);
            DAB.setProgIndex(obj.ProgramIndex);
        }
        i++;
    }

    if(notFound) DAB.setProgIndex(currentDabProgramList[0].ProgramIndex);

    i = 0;
    for(auto obj : ensembleList) {
        if ( i > 0 ) EnsembleNameList += "\\r";
        EnsembleNameList += obj;
        if(dabEnsembleNameFilter == obj) _serial->printf("cbDabFilter.val=%i%s", i, _endChar);
        i++;
    }

    _serial->print("DabStationList.path=\"" + ServiceNameList + "\"" + _endChar);
    _serial->print("cbDabFilter.path=\"" + EnsembleNameList + "\"" + _endChar);
}

void Nextion::setDabProgramIndex(uint32_t programIndex)
{
    // if(_selectedPage != "DAB") return;

    int i = 0;
    for(auto obj : DAB.getCurrentProgramList()) {
        if(obj.ProgramIndex == programIndex) {
            _serial->printf("DabStationList.val=%i%s", i, _endChar);
            break;
        }
        i++;
    }
}

void Nextion::setEnsembleName(String ensembleName)
{
    _serial->print("DAB.EnsembleName.txt=\"" + ensembleName + "\"" + _endChar);
    if(_selectedPage == "DAB") _serial->print("tEnsembleName.txt=\"" + ensembleName + "\"" + _endChar);
}

void Nextion::setServiceName(String serviceName)
{
    _serial->print("DAB.ServiceName.txt=\"" + serviceName + "\"" + _endChar);
    if(_selectedPage == "DAB") _serial->print("tServiceName.txt=\"" + serviceName + "\"" + _endChar);
}

void Nextion::setDabSorter(DabSorter dabSorter)
{
    if(_isSleeping || !_isReady) {
        _pendingData.dabSorter = static_cast<int>(dabSorter);
        return;
    }

    _serial->printf("DAB.DabSorter.val=%i%s", dabSorter, _endChar);
    if(_selectedPage == "DAB") _serial->printf("cbDabSorter.val=%i%s", dabSorter, _endChar);
}

int Nextion::getYear()
{
    return _getInt("prints rtc0,2", 2);
}

int Nextion::getMonth()
{
    return _getInt("prints rtc1,1", 1);
}

int Nextion::getDay()
{
    return _getInt("prints rtc2,1", 1);
}

int Nextion::getHour()
{
    return _getInt("prints rtc3,1", 1);
}

int Nextion::getMinute()
{
    return _getInt("prints rtc4,1", 1);
}

int Nextion::getSecond()
{
    return _getInt("prints rtc5,1", 1);
}

int Nextion::getDayOfWeek()
{
    return _getInt("prints rtc6,1", 1);
}

void Nextion::setYear(int year)
{
    _print("rtc0=" + String(year) );
}

void Nextion::setMonth(int month)
{
    if(month > 12) month = 12;
    else if(month < 1) month = 1;

    _print("rtc1=" + String(month));
}

void Nextion::setDay(int day)
{
    if(day > 31) day = 31;
    else if(day < 1) day = 1;

    _print("rtc2=" + String(day));
}

void Nextion::setHour(int hour)
{
    if(hour > 23) hour = 23;
    else if(hour < 0) hour = 0;

    _print("rtc3=" + String(hour));
}

void Nextion::setMinute(int minute)
{
    if(minute > 59) minute = 59;
    else if(minute < 0) minute = 0;

    _print("rtc4=" + String( minute ) );
}

void Nextion::setSecond(int second)
{
    if(second > 59) second = 59;
    else if(second < 0) second = 0;

    _print("rtc5=" + String( second ) );
}

void Nextion::setDatOfWeek(int dayOfWeek)
{
    if(dayOfWeek > 6) dayOfWeek = 6;
    else if(dayOfWeek < 0) dayOfWeek = 0;

    _print("rtc6=" + String( dayOfWeek ) );
}

void Nextion::SelectAlarm(int index)
{
    if(index < 0) index = 0;
    _print("alarmList.val=" + String(index));
    SendAlarmData(alarmManager.getAlarm(index));
    _selectedAlarmIndex = index;
}

void Nextion::SelectLastAlarm()
{
    SelectAlarm(_selectedAlarmIndex);
}

void Nextion::SendAlarmList(Vector<Alarm> alarmList)
{
    String s = "";

    for (size_t i = 0; i < alarmList.size(); i++)
    {
        if(i > 0) s += "\\r";
        s += alarmList[i].getName();
    }

    _print("alarmList.path=\"" + s + "\"");
    
}

void Nextion::SendAlarmData(Alarm alarm)
{
    if(alarm.enable) _print("alarmEn.val=1");
    else _print("alarmEn.val=0");

    _print("alarmHour.val="+String(alarm.hour));
    _print("alarmMin.val="+String(alarm.minute));

    if( alarm.dayOfWeek & DayOfWeek_Monday )    _print("cAlarmMon.val=1");
    else _print("cAlarmMon.val=0");

    if( alarm.dayOfWeek & DayOfWeek_Tuesday )   _print("cAlarmTue.val=1");
    else _print("cAlarmTue.val=0");

    if( alarm.dayOfWeek & DayOfWeek_Wednesday ) _print("cAlarmWed.val=1");
    else _print("cAlarmWed.val=0");

    if( alarm.dayOfWeek & DayOfWeek_Thursday )  _print("cAlarmThu.val=1");
    else _print("cAlarmThu.val=0");

    if( alarm.dayOfWeek & DayOfWeek_Friday )    _print("cAlarmFri.val=1");
    else _print("cAlarmFri.val=0");

    if( alarm.dayOfWeek & DayOfWeek_Saturday )  _print("cAlarmSat.val=1");
    else _print("cAlarmSat.val=0");

    if( alarm.dayOfWeek & DayOfWeek_Sunday )    _print("cAlarmSun.val=1");
    else _print("cAlarmSun.val=0");
}


void Nextion::Sleep(bool sleep)
{
    if(sleep) {
        _serial->print("sleep=1" + _endChar);
        _isSleeping = true;
    } else {
        _serial->print("sleep=0" + _endChar);
        _isSleeping = false;
        _UpdatePendingData();
    }    
}

void Nextion::Reset()
{
    _serial->print("rest" + _endChar);
}

void Nextion::_UpdatePendingAudioSource(String source)
{
    if      ( source == "Bluetooth" ) audioManager.setAudioSource(&bluetoothAudioSource, true);
    else if ( source == "WebRadio"  ) audioManager.setAudioSource(&webRadioSource, true);
    else if ( source == "SD card"   ) audioManager.setAudioSource(&sdSource, true);
    else if ( source == "FM"        ) audioManager.setAudioSource(&FM, true);
    else if ( source == "DAB"       ) audioManager.setAudioSource(&DAB, true);
}

String Nextion::_formatHexCodeToString(uint8_t value)
{
    return String(char(value));
}

void Nextion::_print(String c)
{
    _serial->print(c + _endChar);
}

int Nextion::_getInt(String command, int size)
{
    _print(command);
    while (_serial->available() == 0) {}
    char value[size];
    _serial->readBytes(value, size);

    int x = 0;
    for(int i = 0; i < size; i++) {
        x += int(value[i]) * pow(256, i);
    }

    return x;
}
bool Nextion::_isInAudioPage()
{
    return _selectedPage == "Audio" || _selectedPage == "Bluetooth" || _selectedPage == "DAB" || _selectedPage == "FM" || _selectedPage == "SD card";
}