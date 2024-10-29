#include "BluetoothAudioSource.h"
#include "Nextion/Nextion.h"

// AnalogAudioStream out;
// BluetoothA2DPSink a2dp_sink(out);

BluetoothAudioSource bluetoothAudioSource;

// int BluetoothAudioSource::s_deviceVolume = 0;

BluetoothAudioSource::BluetoothAudioSource()
{
}

BluetoothAudioSource::~BluetoothAudioSource()
{
}

String BluetoothAudioSource::getID()
{
    return "Bluetooth";
}

void BluetoothAudioSource::preBegin()
{
    Serial.println("Tentative de démarage du Bluetooth.");
    a2dp_sink.set_task_core(1);
    a2dp_sink.set_on_connection_state_changed(OnBluetoothConnectionChanged);
    a2dp_sink.set_on_audio_state_changed(OnBluetoothAudioStateCallback);
    a2dp_sink.set_avrc_metadata_attribute_mask(127);
    a2dp_sink.set_avrc_metadata_callback(OnBluetoothMetadataCallback);
    a2dp_sink.set_avrc_rn_volumechange(OnBluetootVolumeChanged);

    // a2dp_sink.set_avrc_rn_volumechange(OnDeviceVolumeChange);

    a2dp_sink.set_volume_control(&_volumeControl);
}

void BluetoothAudioSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) 
{
    a2dp_sink.set_output(output);
}

void BluetoothAudioSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    a2dp_sink.set_output(stream);
}

void BluetoothAudioSource::postBegin()
{
    a2dp_sink.start("Portal Radio");
    Serial.println("Bluetooth actif!");
}

void BluetoothAudioSource::loop()
{
}

void BluetoothAudioSource::end() {
    a2dp_sink.end();
}

void BluetoothAudioSource::updateVolume(float volume)
{
    _potVolume = volume;
}

void BluetoothAudioSource::play()
{
    a2dp_sink.play();
}

void BluetoothAudioSource::pause()
{
    a2dp_sink.pause();
}

void BluetoothAudioSource::next()
{
    a2dp_sink.next();
}

void BluetoothAudioSource::previous()
{
    a2dp_sink.previous();
}

float BluetoothAudioSource::getVolume()
{
    return _potVolume;
}

// void BluetoothAudioSource::OnDeviceVolumeChange(int volume)
// {
//     Serial.printf("Device Volume change : %i\n.", volume);
//     s_deviceVolume = volume;
// }

void BluetoothAudioSource::OnBluetoothConnectionChanged(esp_a2d_connection_state_t state, void *)
{
    switch (state)
    {
    case ESP_A2D_CONNECTION_STATE_CONNECTING:
        Serial.printf("ESP32 connecting.\n");
        break;
    case ESP_A2D_CONNECTION_STATE_CONNECTED:
        Serial.printf("ESP32 connected to %s.\n", bluetoothAudioSource.a2dp_sink.get_peer_name());
        break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
        Serial.printf("ESP32 disconecting of %s.\n", bluetoothAudioSource.a2dp_sink.get_peer_name());
        break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
        Serial.printf("ESP32 disconected.\n");
        break;
    default:
        Serial.println("Unknow `esp_a2d_connection_state_t` for `OnBluetoothConnectionChanged`.");
        break;
    }
}

void BluetoothAudioSource::OnBluetoothAudioStateCallback(esp_a2d_audio_state_t state, void *)
{
    Serial.printf("Audio State changed : %i.\n", state);
    switch (state)
    {
    case ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND:
        nextion.setPlayStatus(0);
        break;
    
    case ESP_A2D_AUDIO_STATE_STOPPED:
        nextion.setPlayStatus(0);
        break;

    case ESP_A2D_AUDIO_STATE_STARTED:
        nextion.setPlayStatus(1);
        break;

    default:
        break;
    }
}

void BluetoothAudioSource::OnBluetoothMetadataCallback(uint8_t data1, const uint8_t *data2) {

    Serial.println("Metadat has been updated:");

    switch ((esp_avrc_md_attr_mask_t)data1)
    {
    case ESP_AVRC_MD_ATTR_TITLE:
        // Serial.printf("==> Title: %s.\n", data2);
        updateMetaData(Title, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_ALBUM:
        // Serial.printf("==> Album: %s.\n", data2);
        updateMetaData(Album, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_ARTIST:
        // Serial.printf("==> Artist: %s.\n", data2);
        updateMetaData(Artist, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_GENRE:
        // Serial.printf("==> Genre: %s.\n", data2);
        updateMetaData(Genre, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_TRACK_NUM:
        Serial.printf("==> Track num: %s.\n", data2);
        break;
    case ESP_AVRC_MD_ATTR_NUM_TRACKS:
        Serial.printf("==> Num of tracks: %s.\n", data2);
        break;
    case ESP_AVRC_MD_ATTR_PLAYING_TIME:
    {
        uint32_t playtime = String((char*)data2).toInt();
        Serial.printf("==> Playing time is %d ms (%d seconds)\n", playtime, (int)round(playtime/1000.0));
    } break;
    default:
        break;
    }
}

void BluetoothAudioSource::OnBluetootVolumeChanged(int volume)
{
    nextion.setVolume(volume, audioManager.isMuted());
}
