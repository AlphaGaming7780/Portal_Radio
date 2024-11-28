#include "Alarm.h"

DayOfWeek dayOfWeekFromTimeInfoFormat(int wday)
{
    switch (wday)
    {
    case 0:
        return DayOfWeek_Sunday;
        break;

    case 1:
        return DayOfWeek_Monday;
        break;

    case 2:
        return DayOfWeek_Tuesday;
        break;

    case 3:
        return DayOfWeek_Wednesday;
        break;

    case 4:
        return DayOfWeek_Thursday;
        break;

    case 5:
        return DayOfWeek_Friday;
        break;

    case 6:
        return DayOfWeek_Saturday;
        break;

    default:
        return DayOfWeek_NONE;
        break;
    }
}

int dayOfWeekToTimeInfoFormat(DayOfWeek wday)
{
    switch (wday)
    {
    case DayOfWeek_Sunday:
        return 0;
        break;

    case DayOfWeek_Monday:
        return 1;
        break;

    case DayOfWeek_Tuesday:
        return 2;
        break;

    case DayOfWeek_Wednesday:
        return 3;
        break;

    case DayOfWeek_Thursday:
        return 4;
        break;

    case DayOfWeek_Friday:
        return 5;
        break;

    case DayOfWeek_Saturday:
        return 6;
        break;

    default:
        return -1;
        break;
    }
}

String Alarm::getName()
{
    String s = "";
    if(dayOfWeek & DayOfWeek_Monday)    s += ",Mon";
    if(dayOfWeek & DayOfWeek_Tuesday)   s += ",Tue";
    if(dayOfWeek & DayOfWeek_Wednesday) s += ",Wed";
    if(dayOfWeek & DayOfWeek_Thursday)  s += ",Thu";
    if(dayOfWeek & DayOfWeek_Friday)    s += ",Fri";
    if(dayOfWeek & DayOfWeek_Saturday)  s += ",Sat";
    if(dayOfWeek & DayOfWeek_Sunday)    s += ",Sun";

    return String(hour) + ":" + String(minute) + s;

}
