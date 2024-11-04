#include "pDebug.h"
#include <stdarg.h>

pDebug debug(&Serial);

pDebug::pDebug(HardwareSerial *serial, pDebugLevel debugLevel)
{
    _serial = serial;
    _debugLevel = debugLevel;
}

pDebug::~pDebug()
{
}

bool pDebug::begin(unsigned long baud)
{
    _serial->begin(baud);
    return true;
}

void pDebug::setDebugLevel(pDebugLevel debugLevel)
{
    _debugLevel = debugLevel;
}

size_t pDebug::print(String s)
{
    return _serial->print(s);
}

size_t pDebug::println(String s)
{
    return _serial->println(s);
}

// size_t pDebug::printf(const char *format, ...)
// {
//     va_list vl;
//     va_start( vl, format );
//     size_t t = _serial->printf(format, va_arg(vl, char*));
//     va_end(vl);
//     return t;
// }

// size_t pDebug::printfln(const char *format, ...)
// {
//     va_list vl;
//     va_start( vl, format );
//     size_t t = _serial->printf("%s\n", format, vl);
//     va_end(vl);
//     return t;
// }

size_t pDebug::printlnInfo(String s)
{
    if(!(_debugLevel & Debug_INFO)) return -1;
    return println("==> INFO : " + s);
}

size_t pDebug::printlnWarn(String s)
{
    if(!(_debugLevel & Debug_WARN)) return -1;
    return println("==> WARN : " + s);
}

size_t pDebug::printlnError(String s)
{
    if(!(_debugLevel & Debug_ERROR)) return -1;
    return println("==> ERROR : " + s);
}

// size_t pDebug::printflnInfo(const char *format, ...)
// {
//     if(!(_debugLevel & Debug_INFO)) return -1;
//     va_list arg;
//     va_start( arg, format );
//     size_t t = _serial->printf("==> INFO : %s\n", format, arg);
//     va_end(arg);
//     return t;
// }
