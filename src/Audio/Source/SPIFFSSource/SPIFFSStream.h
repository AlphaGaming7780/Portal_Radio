#include "AudioTools.h"


class SPIFFSStream : public AudioStream
{
private:
    String _audioPath;
    File _openedFile;
public:
    SPIFFSStream(/* args */);
    ~SPIFFSStream();

    bool begin();
    bool begin(String audioPath);

    size_t readBytes(uint8_t *data, size_t len);
    size_t write(const uint8_t *data, size_t len);

    String getAudioPath();
    bool SetAudioPath(String audioPath);

};


#pragma once

#include "AudioBasic/StrExt.h"
#include "AudioTools/AudioSource.h"
#include "SPIFFS.h"

/**
 * @brief ESP32 AudioSource for AudioPlayer using an SD card as data source.
 * This class is based on the Arduino SD implementation
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 *
 *  On the AI Thinker boards the pin settings should be On, On, On, On, On,
 * @ingroup player
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class SPIFFSAudioSource : public AudioSource {
public:
    /// Default constructor
    SPIFFSAudioSource(const char *startFilePath = "/", const char *ext = ".ogg", bool setupIndex=true) {
        start_path = startFilePath;
        exension = ext;
        setup_index = setupIndex;
    }

    virtual void begin() override {
        TRACED();
        if (!is_SPIFFS_setup) {
            while (!SPIFFS.begin(true)) {
                Serial.println("SPIFFS.begin failed.");
                delay(1000);
            }
            is_SPIFFS_setup = true;
        }
        MakeIdx();
        idx_pos = 0;
    }

    void MakeIdx() {
        // root = SPIFFS.open(start_path);
        // File file = root.openNextFile();

        // while (file)
        // {   
        //     if(!file.isDirectory()) {
        //         String fileName = String(file.name());
        //         Serial.printf("[IDX] File: %s.\n", fileName.c_str());
        //         if(fileName.endsWith(exension)) idx.push_back(file.path());
        //     }
        //     file.close();
        //     file = root.openNextFile();
        // }

        // Serial.println("IDX Made.");
        // for(int i = 0; i < idx.size(); i++) {
        //     Serial.printf("%s\n", idx[i]);
        // }

        idx.push_back("/Portal Radio.ogg");

    }

    void end(bool endSPIFFS = true) {
        if(endSPIFFS) 
        {
            SPIFFS.end();
            is_SPIFFS_setup = false;
        }
    }

    // void end() {
    //     SD.end();
    //     is_SPIFFS_setup = false;
    // }

    virtual Stream *nextStream(int offset = 1) override {
        LOGI("nextStream: %d", offset);
        return selectStream(idx_pos + offset);
    }

    virtual Stream *selectStream(int index) override {
        LOGI("selectStream: %d", index);
        idx_pos = index;
        file_path = idx[index];
        if (file_path==nullptr) return nullptr;
        LOGI("Using file %s", file_path);
        file = SPIFFS.open(file_path);
        return file ? &file : nullptr;
    }

    virtual Stream *selectStream(const char *path) override {
        file.close();
        file = SPIFFS.open(path);
        file_path = file.path();
        LOGI("-> selectStream: %s", path);
        return file ? &file : nullptr;
    }

    /// Defines the regex filter criteria for selecting files. E.g. ".*Bob
    /// Dylan.*"
    // void setFileFilter(const char *filter) { file_name_pattern = filter; }

    /// Provides the current index position
    int index() { return idx_pos; }

    /// provides the actual file path
    const char *toStr() { return file_path; }

    // provides default setting go to the next
    virtual bool isAutoNext() { return true; }

    /// Allows to "correct" the start path if not defined in the constructor
    virtual void setPath(const char *p) { start_path = p; }

    /// Provides the number of files (The max index is size()-1): WARNING this is very slow if you have a lot of files in many subdirectories
    long size() { return idx.size();}

protected:
    File file;
    File root;
    List<const char*> idx;
    size_t idx_pos = 0;
    const char *file_path;
    const char *exension = nullptr;
    const char *start_path = nullptr;
    // const char *file_name_pattern = "*";
    bool setup_index = true;
    bool is_SPIFFS_setup = false;
    int cs;


};