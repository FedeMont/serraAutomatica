#ifndef __MYCLOCK_H__
#define __MYCLOCK_H__

#include "Energia.h"
#include "types.h"

class MyClock
{
public:
    int selectedDigit;
    int digits[4];

    bool isTimeSaved;
    DayCycle dayCycle;

    unsigned long time;

    MyClock();

    void chooseTime(Action);
    void saveTime();
    void saveTime(const String&);
    void clock(bool);
    void setStartTime();
    bool isMinutePassed();
    String getTimeAsString();

private:
    long start_time;
};

#endif
