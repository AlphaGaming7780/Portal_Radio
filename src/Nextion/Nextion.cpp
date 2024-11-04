#include "Nextion.h"

Nextion nextion(&Serial2);

Nextion::Nextion(HardwareSerial *serial)
{
    _serial = serial;
}

Nextion::~Nextion()
{
}

bool Nextion::init(HardwareSerial *serial)
{
    _serial = serial;
    return true;
}

bool Nextion::begin(ulong baudrate)
{
    if(_serial == nullptr) return false;
    _serial->begin(baudrate);
    _baudrate = baudrate;

    _serial->print("thup=1" + _endChar);
    _serial->print("thsp=60" + _endChar);
    

    return true;
}

void Nextion::Loop()
{

    if(_serial->available() == 0) return;

    String s = _serial->readStringUntil(char(0xFF));

    if(s.length() == 0) return;

    int x = s.indexOf(':');

    int si = s.toInt();

    if      ( s == formatHexCodeToString(0x00) ) debug.printlnError("Invalid Commande");
    else if ( s == formatHexCodeToString(0x1A) ) debug.printlnError("Invalid Variable name or attribute");
    else if ( s == formatHexCodeToString(0x1B) ) debug.printlnError("Invalid Variable Operation");
    else if ( s == formatHexCodeToString(0x1C) ) debug.printlnError("Assignement operation failed");
    else if ( s == formatHexCodeToString(0x1E) ) debug.printlnError("Invalid Quantity of Parameters");
    else if ( s == formatHexCodeToString(0x20) ) debug.printlnError("Escape Character Invalid");
    else if ( s == formatHexCodeToString(0x23) ) debug.printlnError("Variable name too long");
    else if ( s == formatHexCodeToString(0x24) ) debug.printlnWarn("Serial Buffer Overflow");
    else if ( s == formatHexCodeToString(0x86) ) debug.printlnInfo("Auto sleep");
    else if ( s == formatHexCodeToString(0x87) ) debug.printlnInfo("Auto wakeup");
    else if ( s == formatHexCodeToString(0x88) ) debug.printlnInfo("Nextion Ready");
    else if ( s == "play" ) audioManager.Play();
    else if ( s == "pause") audioManager.Pause();
    else if ( s == "next" ) audioManager.Next();
    else if ( s == "prev" ) audioManager.Previous();
    else if ( s == "mute" ) audioManager.Mute();
    else if ( s == "unmute") audioManager.UnMute();
    else if ( x != -1 ) {
        String c = s.substring(0, x);
        String v = s.substring(x+1);
        debug.printlnInfo("Command : " + c);
        debug.printlnInfo("value : " + v);

        if ( c == "audioSource") UpdatePendingAudioSource(v);

    }
    else Serial.printf("Unknown data : %s.\n", s);

}

void Nextion::setAudioSource(String source)
{
    _serial->print("Audio.currentSource.txt=\"" + source + "\"" + _endChar);
    _serial->print("page 0" + _endChar);
}

void Nextion::setVolume(int volume, bool isMuted)
{
    if(volume > 100) volume = 100;
    else if(volume < 0) volume = 0;

    _serial->print("Audio.audioVolume.val=" + String(volume) + _endChar);

    if(!isMuted) {

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
    _serial->print("Audio.audioTitle.txt=\"" + title + "\"" + _endChar);
    _serial->print("gTitle.txt=\"" + title + "\"" + _endChar);
}

void Nextion::setArtist(String artist)
{
    _serial->print("Audio.audioArtist.txt=\"" + artist + "\"" + _endChar);
    _serial->print("gArtist.txt=\"" + artist + "\"" + _endChar);
}

void Nextion::setPlayStatus(bool playStatus)
{

    if(playStatus) {
        _serial->print("Audio.playStatus.val=1" + _endChar);
        _serial->print("pPlay.pic=1" + _endChar);
    } else {
        _serial->print("Audio.playStatus.val=0" + _endChar);
        _serial->print("pPlay.pic=0" + _endChar);
    }
}

void Nextion::UpdatePendingAudioSource(String source)
{
    if      ( source == "Bluetooth" ) audioManager.setAudioSource(&bluetoothAudioSource, true);
    else if ( source == "WebRadio"  ) audioManager.setAudioSource(&webRadioSource, true);
    else if ( source == "SD card"   ) audioManager.setAudioSource(&sdSource, true);
    else if ( source == "FM"        ) debug.printlnError("NO FM AUDIO SOURCE.");
    else if ( source == "DAB"       ) debug.printlnError("NO DAB AUDIO SOURCE.");
}

String Nextion::formatHexCodeToString(uint8_t value)
{
    return String(char(value));
}
