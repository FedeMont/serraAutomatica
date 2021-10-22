#include "Energia.h"
#include "controller.h"

Controller::Controller() {

}

void Controller::begin(Display* display, Navigator navigator, MyClock* myClock) {
    this->display = display;
    this->navigator = navigator;
    this->myClock = myClock;
}

void Controller::chooseTime(Action action) {
    this->display->chooseTime(this->myClock->selectedDigit, this->myClock->digits);
    this->myClock->chooseTime(action);    
}

void Controller::start(){
    Action action = this->navigator.getAction();

    if (!this->myClock->isTimeSaved)
    {
        if (action == SELECT)
        {
            this->myClock->saveTime();
            this->display->clear();
        }
        else {
            this->chooseTime(action);
        }
    } else {
        this->myClock->clock();
        this->display->homeScreen(this->myClock->getTimeAsString());
    }
    

    
}