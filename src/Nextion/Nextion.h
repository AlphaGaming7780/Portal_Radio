#include "Arduino.h"
#include "Audio/AudioManager.h"
#include "pDebug/pDebug.h"

#ifndef dNextion
#define dNextion
class Nextion
{
public:
    Nextion() {};
    Nextion(HardwareSerial *serial);
    ~Nextion();

    bool init(HardwareSerial *serial);
    bool begin(ulong baudrate = 9600);

    void Loop();

    void setAudioSource(String source);
    void setVolume(int volume, bool isMuted);
    void setTitle(String title);
    void setArtist(String artist);
    void setPlayStatus(bool playStatus);

private:
    HardwareSerial *_serial;
    ulong _baudrate;

    String _endChar = String(char(0xFF))+String(char(0xFF))+String(char(0xFF));

    void updatePendingAudioSource(String source);    
};

extern Nextion nextion;
#endif