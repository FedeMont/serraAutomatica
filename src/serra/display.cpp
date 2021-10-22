#include "Energia.h"
#include "Screen_HX8353E.h"
#include "display.h"

Display::Display()
{
}

void Display::begin()
{
    this->myScreen.begin();
    this->myScreen.setFontSolid(true);
    this->myScreen.setPenSolid(false);
    this->myScreen.setOrientation(0);
    this->myScreen.setFontSize(this->myScreen.fontMax());
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

void Display::write(int x, int y, String text, int fontSize, uint16_t colour)
{
    this->myScreen.setFontSize(fontSize);
    this->myScreen.gText(x, y, text, colour);
    this->myScreen.setFontSize(this->myScreen.fontMax());
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

void Display::chooseTime(int selectedDigit, int digits[])
{
    int leftBound = (this->myScreen.screenSizeX() - (this->myScreen.fontSizeX() * 5)) / 2;
    int upperBound = (this->myScreen.screenSizeY() - this->myScreen.fontSizeY()) / 2; // hh:mm

    int column = 0;

    this->write(this->myScreen.fontSizeX(), this->myScreen.fontSizeY(), "Set time: (24h)", greenColour);

    for (int i = 0; i <= 4; i++)
    {
        if (i == 2)
        {
            column++;
            this->write(leftBound + ((i) * this->myScreen.fontSizeX()), upperBound, ":", whiteColour);
        }
        else
        {
            if (selectedDigit == (i - column))
            {
                this->myScreen.setFontSolid(false);
                this->drawRectangle(leftBound + ((selectedDigit + column) * this->myScreen.fontSizeX()), upperBound, this->myScreen.fontSizeX(), this->myScreen.fontSizeY(), whiteColour, true);
                this->write(leftBound + ((selectedDigit + column) * this->myScreen.fontSizeX()), upperBound, String(digits[selectedDigit]), redColour);
                this->myScreen.setFontSolid(true);
            }
            else
            {
                this->write(leftBound + ((i) * this->myScreen.fontSizeX()), upperBound, String(digits[i - column]), whiteColour);
            }
        }
    }

    this->write(this->myScreen.fontSizeX(), (this->myScreen.screenSizeY() - 2*this->myScreen.fontSizeY()), "SEL to confirm", greenColour);

}

void Display::homeScreen(String time) {
    this->write((this->myScreen.screenSizeX() - this->calculateTextSize(time) - this->myScreen.fontSizeX()), this->myScreen.fontSizeY(), time, whiteColour);
}

int Display::calculateTextSize(String text) {
    return (text.length() * this->myScreen.fontSizeX());
}

int* Display::getScreenSize() {
    int screenSize[2] = {this->myScreen.screenSizeX(), this->myScreen.screenSizeY()};
    return screenSize;
}