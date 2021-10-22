#ifndef __MYCLOCK_H__
#define __MYCLOCK_H__

#include "Energia.h"
#include "enums.h"

class MyClock {
public:
    int selectedDigit;
    int digits[4];
    // String timeString;

    bool isTimeSaved;
    DayCycle dayCycle;
    
    unsigned long time;

    MyClock();

    void chooseTime(Action);
    void saveTime();
    void clock();
    String getTimeAsString();

private:
    long start_time;
};

#endif
