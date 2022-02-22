#ifndef __MYCLOCK_H__
#define __MYCLOCK_H__

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Types.h"

class MyClock
{
public:
    MyClock();
    ~MyClock();

    void begin();
    void clock();
    DayCycle getDayCycle();
private:
    WiFiUDP ntpUDP;
    NTPClient timeAndDateClient;

    long resetTimeTimer;
    long isMinutePassedTimer;
    int minutes;
    DayCycle dayCycle;

    void setStartTime();
    String getTime();
    int getMinutes();
};

#endif //__MYCLOCK_H__
