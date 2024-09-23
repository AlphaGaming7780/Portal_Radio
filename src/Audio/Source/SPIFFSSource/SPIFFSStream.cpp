#include "SPIFFSStream.h"

SPIFFSStream::SPIFFSStream()
{
}

SPIFFSStream::~SPIFFSStream()
{
}

bool SPIFFSStream::begin()
{
    if(!SPIFFS.exists(_audioPath)) return false;
    _openedFile = SPIFFS.open(_audioPath);
    return true;
}

bool SPIFFSStream::begin(String audioPath)
{
    _audioPath = audioPath;
    return begin();
}

size_t SPIFFSStream::readBytes(uint8_t *data, size_t len)
{
    return _openedFile.read(data, len);
}

size_t SPIFFSStream::write(const uint8_t *data, size_t len)
{
    return _openedFile.write(data, len);
}

String SPIFFSStream::getAudioPath()
{
    return _audioPath;
}

bool SPIFFSStream::SetAudioPath(String audioPath)
{
    if(!SPIFFS.exists(audioPath)) return false;
    _audioPath = audioPath;
    if(_openedFile) _openedFile.close();
    _openedFile = SPIFFS.open(_audioPath);
}
