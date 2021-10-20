#include "Energia.h"
#include "Screen_HX8353E.h"
#include "display.h"
#include "action.h"

Display::Display()
{
}

void Display::begin()
{
    this->myScreen.begin();
    this->myScreen.setFontSolid(true);
    this->myScreen.setPenSolid(false);
    this->myScreen.setOrientation(0);
    this->myScreen.setFontSize(this->myScreen.fontMax() + 15);
    this->myScreen.clear(blackColour);
}

void Display::clear()
{
    this->myScreen.clear(blackColour);
}

int Display::stringLength(String text)
{
    return (text.length() * this->myScreen.fontSizeX());
}

void Display::write(int x, int y, String text, uint16_t colour)
{
    this->myScreen.gText(x, y, text, colour);
}

void Display::drawRectangle(int x, int y, int lengthX, int lengthY, uint16_t colour, bool isFilled)
{
    if (isFilled)
    {
        this->myScreen.setPenSolid(true);
    }
    this->myScreen.dRectangle(x, y, lengthX, lengthY, colour);
    if (isFilled)
    {
        this->myScreen.setPenSolid(false);
    }
}

void Display::chooseTime(Navigator navigator, MyClock *myClock)
{

    int leftBound = (this->myScreen.screenSizeX() - (this->myScreen.fontSizeX() * 5)) / 2;
    int upperBound = (this->myScreen.screenSizeY() - this->myScreen.fontSizeY()) / 2; // hh:mm

    this->myScreen.setFontSolid(false);
    this->drawRectangle(leftBound + (myClock->selectedDigit * this->myScreen.fontSizeX()), upperBound, this->myScreen.fontSizeX(), this->myScreen.fontSizeY(), whiteColour, true);
    this->write(leftBound + (myClock->selectedDigit * this->myScreen.fontSizeX()), upperBound, String(myClock->digits[myClock->selectedDigit]), redColour);
    this->myScreen.setFontSolid(true);

    this->write(leftBound + this->myScreen.fontSizeX(), upperBound, "0:00", whiteColour);

    switch (navigator.getAction())
    {
    case UP:
        int module;
        switch (myClock->selectedDigit)
        {
        case 0:
            if (myClock->digits[1] > 3) module = 2;
            else module = 3;
            break;
        case 1:
            if (myClock->digits[0] == 2) module = 4;
            else module = 10;
            break;
        case 3:
            module = 10;
            break;
        case 2:
            module = 6;
            break;
        default:
            break;
        }

        myClock->digits[myClock->selectedDigit] = (myClock->digits[myClock->selectedDigit] + 1) % module;

        break;
    case DOWN:
        if (myClock->digits[myClock->selectedDigit] > 0)
            myClock->digits[myClock->selectedDigit]--;
        break;
    case RIGHT:
        myClock->selectedDigit = (myClock->selectedDigit + 1) % 4;
        break;
    case LEFT:
        if (myClock->selectedDigit > 0) myClock->selectedDigit--;
        break;
    default:
        break;
    }

    Serial.print(myClock->digits[0]);
    Serial.print(myClock->digits[1]);
    Serial.print(":");
    Serial.print(myClock->digits[2]);
    Serial.print(myClock->digits[3]);
    Serial.print(" sel: ");
    Serial.println(myClock->selectedDigit);
}