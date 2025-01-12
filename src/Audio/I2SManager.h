#pragma once

#include "AudioTools.h"

class I2SManager
{
private:
public:
    I2SManager();
    ~I2SManager();

    static I2SStream _stream;

    static void Begin();
};
