#include "Energia.h"
#include "controller.h"

Controller::Controller()
{
}

void Controller::begin(Display *display, Navigator navigator, MyClock *myClock)
{
    this->display = display;
    this->navigator = navigator;
    this->myClock = myClock;
}

void Controller::chooseTime(Action action)
{
    this->display->chooseTime(this->myClock->selectedDigit, this->myClock->digits);
    this->myClock->chooseTime(action);
}

void Controller::start()
{
    this->action = this->navigator.getAction();

    if (!this->myClock->isTimeSaved)
    {
        if (this->action == SELECT)
        {
            this->myClock->saveTime();
            this->display->clear();
        }
        else
        {
            this->chooseTime(this->action);
        }
    }
    else
    {
        this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, this->myClock->dayCycle); // first write

        this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
        this->myClock->clock(this->isMinutePassed);
    }
}