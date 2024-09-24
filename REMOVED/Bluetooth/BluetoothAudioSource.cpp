#include "BluetoothAudioSource.h"
// AnalogAudioStream out;
// BluetoothA2DPSink a2dp_sink(out);

BluetoothAudioSource bluetoothAudioSource;

BluetoothAudioSource::BluetoothAudioSource()
{
}

BluetoothAudioSource::~BluetoothAudioSource()
{
}

void BluetoothAudioSource::begin(audio_tools::AudioOutput &output, pAudioOutput *pAudioOutput) 
{
    a2dp_sink.set_output(output);
    pAudioOutput->begin();
    _begin();
}

void BluetoothAudioSource::begin(audio_tools::AudioStream &stream, pAudioOutput *pAudioOutput)
{
    a2dp_sink.set_output(stream);
    pAudioOutput->begin();
    _begin();
}

void BluetoothAudioSource::_begin()
{
    Serial.println("Tentative de démarage du Bluetooth.");
    // a2dp_sink->set_on_connection_state_changed(OnBluetoothConnectionChanged);
    a2dp_sink.set_avrc_metadata_attribute_mask(127);
    a2dp_sink.set_avrc_metadata_callback(OnBluetoothMetadataCallback);
    a2dp_sink.set_volume_control(&volume);

    a2dp_sink.start("Portal Radio");
    Serial.println("Bluetooth actif!");
}

void BluetoothAudioSource::end() {
    a2dp_sink.end();
    // free(a2dp_sink);
    // a2dp_sink = nullptr;
}

void BluetoothAudioSource::OnBluetoothConnectionChanged(esp_a2d_connection_state_t state, void * )
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

void BluetoothAudioSource::OnBluetoothMetadataCallback(uint8_t data1, const uint8_t *data2) {

    Serial.println("Metadat has been updated:");

    switch ((esp_avrc_md_attr_mask_t)data1)
    {
    case ESP_AVRC_MD_ATTR_TITLE:
        Serial.printf("==> Title: %s.\n", data2);
        break;
    case ESP_AVRC_MD_ATTR_ALBUM:
        Serial.printf("==> Album: %s.\n", data2);
        break;
    case ESP_AVRC_MD_ATTR_ARTIST:
        Serial.printf("==> Artist: %s.\n", data2);
        break;
    case ESP_AVRC_MD_ATTR_GENRE:
        Serial.printf("==> Genre: %s.\n", data2);
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