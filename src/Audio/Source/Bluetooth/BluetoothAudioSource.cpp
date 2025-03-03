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

AudioSource &BluetoothAudioSource::getAudioSource()
{
    
}

void BluetoothAudioSource::Setup()
{
    debug.printlnInfo("Tentative de démarage du Bluetooth.");
    // a2dp_sink.set_task_core(0);
    a2dp_sink.set_on_connection_state_changed(onBluetoothConnectionChanged);
    a2dp_sink.set_on_audio_state_changed(onBluetoothAudioStateCallback);
    a2dp_sink.set_avrc_metadata_attribute_mask(127);
    a2dp_sink.set_avrc_metadata_callback(onBluetoothMetadataCallback);
    a2dp_sink.set_avrc_rn_volumechange(onBluetootVolumeChanged);

    // a2dp_sink.set_avrc_rn_volumechange(OnDeviceVolumeChange);

    a2dp_sink.set_volume_control(&_volumeControl);
}

void BluetoothAudioSource::setOutput(audio_tools::AudioOutput &output) 
{
    a2dp_sink.set_output(output);
}

void BluetoothAudioSource::setOutput(audio_tools::AudioStream &stream)
{
    a2dp_sink.set_output(stream);
}

void BluetoothAudioSource::Begin()
{
    a2dp_sink.start("Portal Radio");
    debug.printlnInfo("Bluetooth actif!");
}

void BluetoothAudioSource::End() {
    a2dp_sink.end();
}

float BluetoothAudioSource::volumeInc()
{
    return 0.0f;
}

void BluetoothAudioSource::setVolume(float volume)
{
    _volume = volume;
}

void BluetoothAudioSource::Play()
{
    a2dp_sink.play();
}

void BluetoothAudioSource::Pause()
{
    a2dp_sink.pause();
}

void BluetoothAudioSource::Next()
{
    a2dp_sink.next();
}

void BluetoothAudioSource::Previous()
{
    a2dp_sink.previous();
}

float BluetoothAudioSource::getVolume()
{
    return _volume;
}

// void BluetoothAudioSource::OnDeviceVolumeChange(int volume)
// {
//     Serial.printf("Device Volume change : %i\n.", volume);
//     s_deviceVolume = volume;
// }

void BluetoothAudioSource::onBluetoothConnectionChanged(esp_a2d_connection_state_t state, void *)
{
    switch (state)
    {
    case ESP_A2D_CONNECTION_STATE_CONNECTING:
        Serial.printf("ESP32 connecting.\n");
        nextion.setBtConnectionStatus("Connecting...");
        nextion.setBtPeerName("");
        break;
    case ESP_A2D_CONNECTION_STATE_CONNECTED:
        Serial.printf("ESP32 connected to %s.\n", bluetoothAudioSource.a2dp_sink.get_peer_name());
        nextion.setBtConnectionStatus("Connected");
        nextion.setBtPeerName(bluetoothAudioSource.a2dp_sink.get_peer_name());
        break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
        Serial.printf("ESP32 disconecting of %s.\n", bluetoothAudioSource.a2dp_sink.get_peer_name());
        nextion.setBtConnectionStatus("Disconecting...");
        nextion.setBtPeerName(bluetoothAudioSource.a2dp_sink.get_peer_name());
        break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
        Serial.printf("ESP32 disconnected.\n");
        nextion.setBtConnectionStatus("Disconnected");
        nextion.setBtPeerName("");
        break;
    default:
        Serial.println("Unknow `esp_a2d_connection_state_t` for `OnBluetoothConnectionChanged`.");
        break;
    }
}

void BluetoothAudioSource::onBluetoothAudioStateCallback(esp_a2d_audio_state_t state, void *)
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

void BluetoothAudioSource::onBluetoothMetadataCallback(uint8_t data1, const uint8_t *data2) {

    debug.printlnInfo("Metadat has been updated:");

    switch ((esp_avrc_md_attr_mask_t)data1)
    {
    case ESP_AVRC_MD_ATTR_TITLE:
        Serial.printf("==> Title: %s.\n", data2);
        UpdateMetaData(Title, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_ALBUM:
        Serial.printf("==> Album: %s.\n", data2);
        UpdateMetaData(Album, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_ARTIST:
        Serial.printf("==> Artist: %s.\n", data2);
        UpdateMetaData(Artist, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_GENRE:
        Serial.printf("==> Genre: %s.\n", data2);
        UpdateMetaData(Genre, (const char*)data2, -1);
        break;
    case ESP_AVRC_MD_ATTR_TRACK_NUM:
        Serial.printf("==> Track num: %s.\n", data2);
        nextion.setTrackNum(String((char*)data2).toInt());
        break;
    case ESP_AVRC_MD_ATTR_NUM_TRACKS:
        Serial.printf("==> Num of tracks: %s.\n", data2);
        nextion.setNumTracks(String((char*)data2).toInt());
        break;
    case ESP_AVRC_MD_ATTR_PLAYING_TIME:
    {
        uint32_t playtime = String((char*)data2).toInt();
        Serial.printf("==> Playing time is %d ms (%d seconds)\n", playtime, (int)round(playtime/1000.0));
        nextion.setPlayTime((int)round(playtime/1000.0));
    } break;
    default:
        break;
    }
}

void BluetoothAudioSource::onBluetootVolumeChanged(int volume)
{
    nextion.setVolume(volume, audioManager.isMuted());
}
