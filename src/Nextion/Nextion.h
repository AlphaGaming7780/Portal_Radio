#pragma once
#include "Arduino.h"
#include "Audio/AudioManager.h"
#include "Alarm/AlarmManager.h"
#include "Alarm/Alarm.h"
#include "pDebug/pDebug.h"
#include "Nextion/NextionPendingData.h"

class Nextion
{
public:
    Nextion() {};
    Nextion(HardwareSerial *serial);
    ~Nextion();

    bool Init(HardwareSerial *serial);
    bool Begin(ulong baudrate = 9600);

    void StartupFinished();

    void Loop();

    void setAudioSource(String source);
    void setVolume(int volume, bool isMuted);
    void setTitle(String title);
    void setArtist(String artist);
    void setPlayStatus(bool playStatus);

    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMinute();
    int getSecond();
    int getDayOfWeek();

    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setHour(int hour);
    void setMinute(int minute);
    void setSecond(int second);
    void setDatOfWeek(int dayOfWeek);

    void SelectAlarm(int index);
    void SelectLastAlarm();
    void SendAlarmList(Vector<Alarm> alarmList);
    void SendAlarmData(Alarm alarm);

    void Sleep(bool sleep);
    void Reset();

private:
    HardwareSerial *_serial;
    ulong _baudrate;

    String _endChar = String(char(0xFF))+String(char(0xFF))+String(char(0xFF));
    String _selectedPage = "";

    bool _isSleeping = false;
    bool _isReady = false;

    int _selectedAlarmIndex = 0;

    NextionPendingData _pendingData;

    void _UpdatePendingAudioSource(String source);
    String _formatHexCodeToString(uint8_t value);

    void _print(String c);

    int _getInt(String command, int size);

    void _UpdatePendingData() {
        
        if(_pendingData.audioSource != emptyString)   { setAudioSource(_pendingData.audioSource); _pendingData.audioSource = emptyString; }

        if(_pendingData.volume != -1 && _pendingData.isMuted != BOOL3_NULL) {
            setVolume(_pendingData.volume, _pendingData.isMuted);
            _pendingData.volume = -1;
            _pendingData.isMuted = BOOL3_NULL;
        }

        if(_pendingData.title != emptyString)   { setTitle(_pendingData.title); _pendingData.title = emptyString; }
        if(_pendingData.artist != emptyString)  { setArtist(_pendingData.artist); _pendingData.artist = emptyString; }
        if(_pendingData.playStatus != BOOL3_NULL)     { setPlayStatus(_pendingData.playStatus); _pendingData.playStatus = BOOL3_NULL; } 

    }

};

extern Nextion nextion;
