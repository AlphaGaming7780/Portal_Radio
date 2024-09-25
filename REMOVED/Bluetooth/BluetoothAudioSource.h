#include "Arduino.h"
#include "BluetoothA2DPSink.h"
#include "A2DPVolumeControl.h"
#include "../../pAudioSource.h"

#ifndef dBluetoothAudioSource
#define dBluetoothAudioSource
class BluetoothAudioSource : public pAudioSource
{
private:
    /* data */
    A2DPLinearVolumeControl _volumeControl;
    // static int s_deviceVolume;

public:
    BluetoothA2DPSink a2dp_sink;

    BluetoothAudioSource(/* args */);
    ~BluetoothAudioSource();

    String getID();

    void preBegin();
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput);
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput);
    void postBegin();
    void loop();
    void end();

    void updateVolume(float volume);

    // static void OnDeviceVolumeChange(int volume);
    static void OnBluetoothConnectionChanged(esp_a2d_connection_state_t, void *);
    static void OnBluetoothMetadataCallback(uint8_t, const uint8_t *);
};

extern BluetoothAudioSource bluetoothAudioSource;
#endif