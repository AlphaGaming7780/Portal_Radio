#include "I2SManager.h"

// I2SStream I2SManager::_stream;
I2SStream I2SManager::_streamIn;
I2SStream I2SManager::_streamOut;

I2SManager::I2SManager(/* args */)
{
}

I2SManager::~I2SManager()
{
}

// void I2SManager::Begin()
// {
//     auto config = _stream.defaultConfig(RXTX_MODE);
//     config.i2s_format = I2S_LSB_FORMAT;
//     config.pin_ws = 14;
//     config.pin_bck = 15;
//     config.pin_data = 22; //TX pin for RXTX mode
//     config.pin_data_rx = 21; //RX pin for RXTX mode
//     // config.pin_mck = 25;
//     // config.is_master = true;
//     config.sample_rate = 41000;
//     config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
//     config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
//     config.buffer_count = 8;
//     config.buffer_size = 64;
//     config.use_apll = false;
//     config.auto_clear = true; // avoiding noise in case of data unavailability
//     _stream.begin(config);
// }

void I2SManager::BeginIn()
{
    auto config = _streamIn.defaultConfig(RX_MODE);
    config.i2s_format = I2S_STD_FORMAT;
    config.pin_ws = 4;
    config.pin_bck = 2;
    config.pin_data = 21; //TX pin for RXTX mode
    config.pin_data_rx = 21; //TX pin for RXTX mode
    config.pin_mck = 25;
    config.is_master = false;
    config.sample_rate = 44100;
    config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
    config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    config.buffer_count = 8;
    config.buffer_size = 64;
    config.use_apll = false;
    config.auto_clear = true; // avoiding noise in case of data unavailability
    config.port_no = 1;
    _streamIn.begin(config);
}

void I2SManager::BeginOut()
{
    auto config = _streamOut.defaultConfig(TX_MODE);
    config.i2s_format = I2S_LSB_FORMAT;
    config.pin_ws = 14;
    config.pin_bck = 15;
    config.pin_data = 22; //TX pin for RXTX mode
    // config.pin_mck = 25;
    config.is_master = true;
    config.sample_rate = 44100;
    config.bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT;
    config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    config.buffer_count = 8;
    config.buffer_size = 64;
    config.use_apll = false;
    config.auto_clear = true; // avoiding noise in case of data unavailability
    config.port_no = 0;
    _streamOut.begin(config);
}
