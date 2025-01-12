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
    AudioSource &getAudioSource() override;

    void Setup() override;
    void setOutput(audio_tools::AudioOutput &output) override;
    void setOutput(audio_tools::AudioStream &stream) override;
    void Begin() override;
    void End() override;

    void Play() override;
    void Pause() override;
    void Next() override;
    void Previous() override;

    void setVolume(float volume) override;
    float getVolume() override;

    // static void OnDeviceVolumeChange(int volume);
    static void onBluetoothConnectionChanged(esp_a2d_connection_state_t, void *);
    static void onBluetoothAudioStateCallback(esp_a2d_audio_state_t state, void*);
    static void onBluetoothMetadataCallback(uint8_t, const uint8_t *);
    static void onBluetootVolumeChanged(int volume);
};

extern BluetoothAudioSource bluetoothAudioSource;
#endif