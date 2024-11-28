#pragma once
#include "ArduinoJson.h"

enum DayOfWeek : int
{   
    DayOfWeek_NONE      = 0,
    DayOfWeek_Monday    = 1,
    DayOfWeek_Tuesday   = 2,
    DayOfWeek_Wednesday = 4,
    DayOfWeek_Thursday  = 8,
    DayOfWeek_Friday    = 16,
    DayOfWeek_Saturday  = 32,
    DayOfWeek_Sunday    = 64,
};

class Alarm {

public:
    bool enable = false;
    int hour, minute = 0;
    DayOfWeek dayOfWeek = DayOfWeek_NONE;

    String getName();
};

DayOfWeek dayOfWeekFromTimeInfoFormat(int wday);
int dayOfWeekToTimeInfoFormat(DayOfWeek wday);

// inline void convertToJson(const Alarm& t, JsonVariant variant) {

//     JsonObject AlarmData;
//     AlarmData["enable"] = t.enable;
//     AlarmData["hour"] = t.hour;
//     AlarmData["minute"] = t.minute;
//     AlarmData["dayOfWeek"] = t.dayOfWeek;

//     variant.set(AlarmData);

// }

// inline void convertFromJson(JsonVariantConst src, Alarm& dst) {
//     dst.enable = src["enable"];
//     dst.hour = src["hour"];
//     dst.minute = src["minute"];  
//     dst.dayOfWeek = src["dayOfWeek"];
// }

inline DayOfWeek operator|(DayOfWeek a, DayOfWeek b)
{
    return static_cast<DayOfWeek>(static_cast<int>(a) | static_cast<int>(b));
}

inline DayOfWeek operator&(DayOfWeek a, DayOfWeek b)
{
    return static_cast<DayOfWeek>(static_cast<int>(a) & static_cast<int>(b));
}

inline DayOfWeek operator^(DayOfWeek a, DayOfWeek b)
{
    return static_cast<DayOfWeek>(static_cast<int>(a) ^ static_cast<int>(b));
}