#ifndef __MYCLOCK_H__
#define __MYCLOCK_H__

#include "Energia.h"
#include "Types.h"

class MyClock
{
public:
    int selectedDigit;
    int digits[4];
    bool isTimeSaved;
    DayCycle dayCycle;

    MyClock();
    ~MyClock();

    void chooseTime(Action);
    void saveTime();
    void saveTime(const String&);
    String getTimeAsString();

    void clock(bool, bool);
    bool isMinutePassed();

private:
    unsigned long time;

    long start_time;

    void setStartTime();
};

#endif
