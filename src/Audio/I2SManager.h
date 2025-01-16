#pragma once

#include "AudioTools.h"

class I2SManager
{
private:
public:
    I2SManager();
    ~I2SManager();

    // static I2SStream _stream;
    static I2SStream _streamIn;
    static I2SStream _streamOut;

    // static void Begin();
    static void BeginIn();
    static void BeginOut();
};
