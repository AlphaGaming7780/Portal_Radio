#pragma once

#include "Arduino.h"

// #ifndef NextionPendingData_h
// #define NextionPendingData_h

typedef enum {
	BOOL3_NULL = -1,   // Play the playlist and stop at the end of it
	BOOL3_FLASE = 0,   // Play the playlist and restart at the end of it
	BOOL3_TRUE = 1,      // Play the current track and restart it when finished.
} BOOL3;

class NextionPendingData
{
private:
    /* data */
public:
    NextionPendingData(/* args */) {};
    ~NextionPendingData() {};

    String audioSource = emptyString;

    int volume = -1;
    BOOL3 isMuted = BOOL3_NULL;

    String title = emptyString;
    String artist = emptyString;

    BOOL3 playStatus = BOOL3_NULL;

};

// #endif