#include "Arduino.h"
#include "BluetoothA2DPSink.h"
#include "../../pAudioSource.h"

#ifndef dBluetoothAudioSource
#define dBluetoothAudioSource
class BluetoothAudioSource : public pAudioSource
{
private:
    /* data */
    void _begin();

public:
    BluetoothA2DPSink a2dp_sink;

    BluetoothAudioSource(/* args */);
    ~BluetoothAudioSource();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void end();

    // static void OnBluetoothConnectionChanged(esp_a2d_connection_state_t, void *);
    static void OnBluetoothMetadataCallback(uint8_t, const uint8_t *);
};

extern BluetoothAudioSource bluetoothAudioSource;
#endif