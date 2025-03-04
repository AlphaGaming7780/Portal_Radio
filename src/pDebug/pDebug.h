#include "Arduino.h"

#ifndef dDebug
#define dDebug

enum pDebugLevel
{
    Debug_NONE        = 0,
    Debug_INFO        = 1,
    Debug_WARN        = 2,
    Debug_ERROR       = 4,
};

inline pDebugLevel operator|(pDebugLevel a, pDebugLevel b)
{
    return static_cast<pDebugLevel>(static_cast<int>(a) | static_cast<int>(b));
}

inline pDebugLevel operator&(pDebugLevel a, pDebugLevel b)
{
    return static_cast<pDebugLevel>(static_cast<int>(a) & static_cast<int>(b));
}

class pDebug
{
private:
    HardwareSerial *_serial;
    pDebugLevel _debugLevel;
public:
    pDebug(HardwareSerial *serial, pDebugLevel = Debug_ERROR);
    ~pDebug();

    bool begin(unsigned long baud = 115200);
    void setDebugLevel(pDebugLevel debugLevel);
    pDebugLevel getDebugLevel();

    size_t print(String s);
    size_t printInfo(String s);
    size_t printWarn(String s);
    size_t printError(String s);

    size_t println(String s);
    // size_t printf(const char *format, ...);
    // size_t printfln(const char *format, ...);

    size_t printlnInfo(String s);
    size_t printlnWarn(String s);
    size_t printlnError(String s);

    // size_t printflnInfo(const char *format, ...);
    // size_t printflnWarn(const char *format, ...);
    // size_t printflnError(const char *format, ...);
};

extern pDebug debug;

#endif