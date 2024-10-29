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
    float _potVolume = 0;
    // static int s_deviceVolume;

public:
    BluetoothA2DPSink a2dp_sink;

    BluetoothAudioSource(/* args */);
    ~BluetoothAudioSource();

    String getID() override;

    void preBegin() override;
    void begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) override;
    void begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput) override;
    void postBegin() override;
    void loop() override;
    void end() override;

    void updateVolume(float volume) override;
    void play() override;
    void pause() override;
    void next() override;
    void previous() override;

    float getVolume() override;

    // static void OnDeviceVolumeChange(int volume);
    static void OnBluetoothConnectionChanged(esp_a2d_connection_state_t, void *);
    static void OnBluetoothAudioStateCallback(esp_a2d_audio_state_t state, void*);
    static void OnBluetoothMetadataCallback(uint8_t, const uint8_t *);
    static void OnBluetootVolumeChanged(int volume);
};

extern BluetoothAudioSource bluetoothAudioSource;
#endif