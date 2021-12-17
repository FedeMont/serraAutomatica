// #include "Energia.h"
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

    this->timer_start = millis();
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

void Display::drawImage(tImage image, uint16_t x00, uint16_t y00)
{
    uint16_t c;
    int screenSizeX = this->getScreenSize()[0];
    int screenSizeY = this->getScreenSize()[1];

    for (uint16_t i = 0; i < image.width; i++)
    {
        for (uint16_t j = 0; j < image.height; j++)
        {
            if ((x00 + i < screenSizeX) && (y00 + j < screenSizeY))
            {
                c = image.data[i * image.height + j];
                if (c != 0x0000)
                    this->myScreen.point(x00 + i, y00 + j, c);
            }
        }
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

    this->write(this->myScreen.fontSizeX(), (this->myScreen.screenSizeY() - 2 * this->myScreen.fontSizeY()), "SEL to confirm", greenColour);
}

void Display::homeScreen(String time, bool isMinutePassed, DayCycle dayCycle, int humidity, bool shouldWatering)
{
    if (dayCycle != this->previousDayCycle)
    {
        this->drawImage((dayCycle == DAY) ? sun : moon, this->myScreen.fontSizeX(), this->myScreen.fontSizeY());
        this->previousDayCycle = dayCycle;
    }

    if (isMinutePassed)
    {
        this->write((this->myScreen.screenSizeX() - this->calculateTextSize(time) - this->myScreen.fontSizeX()), 2 * this->myScreen.fontSizeY(), time, whiteColour);
    }

    if (millis() - this->timer_start > 1000)
    {
        String humidityText = "Soil hum.: " + String(humidity) + "%";
        this->write((this->getScreenSize()[0] - this->calculateTextSize(humidityText)) / 2, (this->getScreenSize()[1] - this->myScreen.fontSizeY()) / 2, humidityText, whiteColour);

        this->timer_start = millis();
    }

    if (shouldWatering && this->wateringFlag)
    {
#ifdef DEBUG
        Serial.print("Dry: ");
        Serial.println(this->wateringFlag);
#endif
        this->write(this->myScreen.fontSizeX(), (this->getScreenSize()[1] - 2 * this->myScreen.fontSizeY()), "Watering...", blueColour);
        this->drawImage(water, (this->getScreenSize()[0] - water.width - this->myScreen.fontSizeX()), (this->getScreenSize()[1] - water.height - this->myScreen.fontSizeY()));
        this->wateringFlag = !this->wateringFlag;
    }
    else if (!shouldWatering && !this->wateringFlag)
    {
#ifdef DEBUG
        Serial.print("Wet: ");
        Serial.println(this->wateringFlag);
#endif
        this->drawRectangle(0, (this->getScreenSize()[1] - water.height - this->myScreen.fontSizeY()), this->getScreenSize()[0], (water.height + this->myScreen.fontSizeY()), blackColour, true);
        this->wateringFlag = !this->wateringFlag;
    }
}

int Display::calculateTextSize(String text)
{
    return (text.length() * this->myScreen.fontSizeX());
}

int *Display::getScreenSize()
{
    int *screenSize = new int[2];
    screenSize[0] = this->myScreen.screenSizeX();
    screenSize[1] = this->myScreen.screenSizeY();

    return screenSize;
}