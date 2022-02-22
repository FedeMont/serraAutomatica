#include "MyClock.h"

MyClock::MyClock() : timeAndDateClient(this->ntpUDP, "pool.ntp.org", 3600, 60000) // https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/
{
    this->dayCycle = DayCycle_NONSET;
}

MyClock::~MyClock()
{
}

// public
void MyClock::begin()
{
    this->timeAndDateClient.begin();

    this->resetTimeTimer = millis();
    this->isMinutePassedTimer = millis();
    this->minutes = this->getMinutes();
}

// private
String MyClock::getTime()
{
    this->timeAndDateClient.update();
    return this->timeAndDateClient.getFormattedTime().substring(0, 5);
}

int MyClock::getMinutes()
{
    String time = this->getTime();

#ifdef DEBUG
    Serial.println(time);
#endif

    return 60 * (time.substring(0,2)).toInt() + (time.substring(3, 5)).toInt();
}

void MyClock::clock()
{
    if (millis() - this->resetTimeTimer > 10800000) // 3 hours = 3 * 3600s * 10000 ms = 10 800 000 
    {
        this->minutes = this->getMinutes();
        this->resetTimeTimer = millis();
        this->isMinutePassedTimer = millis();
    }
    
    if (this->dayCycle == DayCycle_NONSET || millis() - this->isMinutePassedTimer > 60000) // 1 minute = 60s * 1000ms
    {
        if (this->minutes + 1 == 1440) // 23:59 + 1m
        {
            this->minutes = 0;
        }
        else
        {
            this->minutes++;
        }

        if (this->minutes >= (7 * 60 + 30) && this->minutes <= (18 * 60 + 30))
        {
            this->dayCycle = DayCycle_DAY;
        }
        else
        {
            this->dayCycle = DayCycle_NIGHT;
        }


#ifdef DEBUG
        Serial.println(this->minutes);
#endif
        this->isMinutePassedTimer = millis();
    }
}

DayCycle MyClock::getDayCycle()
{
    return this->dayCycle;
}
