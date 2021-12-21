// #include "Energia.h"
#include "MyClock.h"

MyClock::MyClock()
{
    this->selectedDigit = 0;
    this->digits[4] = {0};
    this->isTimeSaved = false;
    this->dayCycle = DayCycle::NONSET;
    this->time = 0;
}

void MyClock::chooseTime(Action action)
{
    switch (action)
    {
    case Action::UP:
        int module;
        switch (this->selectedDigit)
        {
        case 0:
            if (this->digits[1] > 3)
                module = 2;
            else
                module = 3;
            break;
        case 1:
            if (this->digits[0] == 2)
                module = 4;
            else
                module = 10;
            break;
        case 2:
            module = 6;
            break;
        case 3:
            module = 10;
            break;
        default:
            break;
        }

        this->digits[this->selectedDigit] = (this->digits[this->selectedDigit] + 1) % module;

        break;
    case Action::DOWN:
        if (this->digits[this->selectedDigit] > 0)
        {
            this->digits[this->selectedDigit]--;
        }
        else
        {
            switch (this->selectedDigit)
            {
            case 0:
                if (this->digits[1] > 3)
                    this->digits[this->selectedDigit] = 1;
                else
                    this->digits[this->selectedDigit] = 2;
                break;
            case 1:
                if (this->digits[0] == 2)
                    this->digits[this->selectedDigit] = 3;
                else
                    this->digits[this->selectedDigit] = 9;
                break;
            case 2:
                this->digits[this->selectedDigit] = 5;
                break;
            case 3:
                this->digits[this->selectedDigit] = 9;
                break;
            default:
                break;
            }
        }

        break;
    case Action::RIGHT:
        this->selectedDigit = (this->selectedDigit + 1) % 4;
        break;
    case Action::LEFT:
        if (this->selectedDigit > 0)
            this->selectedDigit--;
        else
            this->selectedDigit = 3;
        break;
    default:
        break;
    }

#ifdef DEBUG
    Serial.print(this->digits[1]);
    Serial.print(this->digits[0]);
    Serial.print(":");
    Serial.print(this->digits[2]);
    Serial.print(this->digits[3]);
    Serial.print(" sel: ");
    Serial.println(this->selectedDigit);
    Serial.println(this->time);
#endif
}

void MyClock::saveTime()
{
    this->time = this->digits[3] + (this->digits[2] * 10) + (this->digits[1] * 60) + (this->digits[0] * 60 * 10);
    this->start_time = millis();
    this->isTimeSaved = true;
}

void MyClock::clock(bool isMinutePassed)
{
    if (this->dayCycle == DayCycle::NONSET)
    {
        if (this->time >= (7 * 60 + 30) && this->time <= (18 * 60 + 30))
        {
            this->dayCycle = DayCycle::DAY;
        }
        else
        {
            this->dayCycle = DayCycle::NIGHT;
        }
    }

    if (isMinutePassed)
    {
        if (this->time + 1 == (24 * 60)) // 23:59 + 1 min
        {
            this->time = 0;
        }
        else
        {
            this->time++;
        }

        if (this->time >= (7 * 60 + 30) && this->time <= (18 * 60 + 30))
        {
            this->dayCycle = DayCycle::DAY;
        }
        else
        {
            this->dayCycle = DayCycle::NIGHT;
        }

#ifdef DEBUG
        Serial.print("Saved time: ");
        Serial.print(this->time);
        Serial.print(", DayCycle: ");
        Serial.println(int(this->dayCycle));
#endif
    }
}

void MyClock::setStartTime()
{
    this->start_time = millis();
}

bool MyClock::isMinutePassed()
{
    if ((this->isTimeSaved) && (millis() - this->start_time >= 60000)) // 1 every minute
    {
        this->setStartTime();
        return true;
    }
    else
        return false;
}

String MyClock::getTimeAsString()
{
    int hours = this->time / 60;
    String hoursString = String(hours);
    if (hours < 10)
    {
        hoursString = "0" + hoursString;
    }

    int minutes = this->time - (hours * 60);
    String minutesString = String(minutes);
    if (minutes < 10)
    {
        minutesString = "0" + minutesString;
    }

    return hoursString + ":" + minutesString;
}
