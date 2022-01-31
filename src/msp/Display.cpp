#include <Energia.h>
#include "Display.h"

Display::Display()
{
    this->blackColour = 0b0000000000000000;
    this->whiteColour = 0b1111111111111111;
    this->redColour = 0b1111100000000000;
    this->greenColour = 0b0000011111100000;
    this->blueColour = 0b0000000000011111;
    this->yellowColour = 0b1111111111100000;
    this->cyanColour = 0b0000011111111111;
    this->orangeColour = 0b1111101111100000;
    this->magentaColour = 0b1111100000001111;
    this->violetColour = 0b1111100000011111;
    this->grayColour = 0b0111101111101111;
    this->darkGrayColour = 0b0011100111100111;

    this->previousConnectionState = false;
    this->previousDayCycle = DayCycle_NONSET;
    this->previousSelectedAction = Action_NONE;
    this->halfScreenY = 0;
    this->halfScreenX = 0;

    this->fanFlag = true;
    this->wateringFlag = true;
}

void Display::begin()
{
    this->myScreen.begin();
    this->myScreen.setFontSolid(true);
    this->myScreen.setPenSolid(false);
    this->myScreen.setOrientation(0);
    this->myScreen.setFontSize(this->myScreen.fontMax());
    this->myScreen.clear(blackColour);

    this->screenSizeX = this->myScreen.screenSizeX();
    this->screenSizeY = this->myScreen.screenSizeY();
    this->halfScreenY = (this->screenSizeY - this->myScreen.fontSizeY()) / 2;
    this->halfScreenX = (this->screenSizeX - this->myScreen.fontSizeX()) / 2;

    this->timer_start = millis();
    this->timer_dayCycle = millis();
    this->timer_watering = millis();
    this->timer_state = millis();
}

void Display::clear()
{
    this->myScreen.clear(blackColour);
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
    this->drawRectangle(x00, y00, image.width, image.height, blackColour, true);

    uint16_t c;
    for (uint16_t i = 0; i < image.width; i++)
    {
        for (uint16_t j = 0; j < image.height; j++)
        {
            if ((x00 + i < this->screenSizeX) && (y00 + j < this->screenSizeY))
            {
                c = image.data[i * image.height + j];
                if (c != 0x0000)
                    this->myScreen.point(x00 + i, y00 + j, c);
            }
        }
    }
}

void Display::drawImage(tImage image, uint16_t x00, uint16_t y00, bool isAuto)
{
    this->drawRectangle(x00, y00, image.width, image.height, blackColour, true);
    uint16_t c;
    for (uint16_t i = 0; i < image.width; i++)
    {
        for (uint16_t j = 0; j < image.height; j++)
        {
            if ((x00 + i < this->screenSizeX) && (y00 + j < this->screenSizeY))
            {
                c = image.data[i * image.height + j];
                if (c != 0x0000)
                    this->myScreen.point(x00 + i, y00 + j, c);
            }
        }
    }

    this->write(x00 + image.width - this->myScreen.fontSizeX(), y00 + image.height - this->myScreen.fontSizeY(), (isAuto)? "A" : "M", whiteColour);
}

void Display::connecting()
{
    String text = "Connecting...";
    int textSize = this->calculateTextSize(text);
    this->write(this->halfScreenX - textSize/2, this->halfScreenY, text, orangeColour);
}

// void Display::chooseState(State selectedState)
// {
//     String state, otherState;
//     int stateIndex, otherStateIndex;
// 
//     if (selectedState == State_MANUAL) {
//         state = "MANUAL";
//         otherState = "AUTOMATIC";
//         stateIndex = 0;
//         otherStateIndex = (2 * this->myScreen.fontSizeY());
//     } else {
//         state = "AUTOMATIC";
//         otherState = "MANUAL";
//         stateIndex = (2 * this->myScreen.fontSizeY());
//         otherStateIndex = 0;
//     }
// 
//     this->write(0, 0, "Select mode:", greenColour);
// 
//     int leftStateBound = (this->myScreen.screenSizeX() - this->calculateTextSize(state)) / 2;
//     int leftOtherStateBound = (this->myScreen.screenSizeX() - this->calculateTextSize(otherState)) / 2;
//     int upperBound = (this->myScreen.screenSizeY() - this->myScreen.fontSizeY()) / 2;
// 
//     this->drawRectangle(0, upperBound, this->myScreen.screenSizeX(), 3 * this->myScreen.fontSizeY(), blackColour, true);
// 
//     this->myScreen.setFontSolid(false);
//     this->drawRectangle(leftStateBound, upperBound + stateIndex, this->calculateTextSize(state), this->myScreen.fontSizeY(), violetColour, true);
//     this->write(leftStateBound, upperBound + stateIndex, state, whiteColour);
//     this->myScreen.setFontSolid(true);
// 
//     this->write(leftOtherStateBound, upperBound + otherStateIndex, otherState, violetColour);
// }

void Display::chooseTime(int selectedDigit, int digits[])
{
    int leftBound = (this->screenSizeX - (this->myScreen.fontSizeX() * 5)) / 2;
    int upperBound = (this->screenSizeY - this->myScreen.fontSizeY()) / 2; // hh:mm

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

    this->write(this->myScreen.fontSizeX(), (this->screenSizeY - 2 * this->myScreen.fontSizeY()), "SEL to confirm", greenColour);
}

void Display::resetHomeScreenFlags() {
    this->previousDayCycle = DayCycle_NONSET;
    this->previousSelectedAction = Action_NONE;

    this->wateringFlag = true;
    this->fanFlag = true;
}

void Display::homeScreen(bool connectionState, String time, bool isMinutePassed, DayCycle dayCycle, float humidity, float temperature, bool shouldWatering, bool shouldFan, bool isLightAuto, bool isWaterAuto, bool isFanAuto)
{
    if (connectionState != this->previousConnectionState || isMinutePassed)
    {
        this->previousConnectionState = connectionState;
        tImage image = (this->previousConnectionState == true)? connection : notconnection;
        this->drawImage(image, this->myScreen.fontSizeX(), this->myScreen.fontSizeY());
    }
    // else
    // {
    //     if (isMinutePassed) // 1 minute
    //     {
    //         tImage image = (this->previousConnectionState == true)? connection : notconnection;
    //         this->drawImage(image, this->myScreen.fontSizeX(), this->myScreen.fontSizeY());
    //     }
    // }

    if (isMinutePassed)
    {
        this->write((this->screenSizeX - this->calculateTextSize(time) - this->myScreen.fontSizeX()), 2 * this->myScreen.fontSizeY(), time, whiteColour);
    }

    if (millis() - this->timer_start > 1000)
    {
        String tempText = "Temperature: " + String(temperature, 2) + "C";
        this->drawRectangle(0, (this->halfScreenY - this->myScreen.fontSizeY()), this->screenSizeX, this->myScreen.fontSizeY(), blackColour, true);
        this->write((this->screenSizeX - this->calculateTextSize(tempText)) / 2, this->halfScreenY - this->myScreen.fontSizeY(), tempText, redColour);

        String humidityText = "Soil hum.: " + String(humidity, 2) + "%";
        this->drawRectangle(0, (this->halfScreenY + this->myScreen.fontSizeY()), this->screenSizeX, this->myScreen.fontSizeY(), blackColour, true);
        this->write((this->screenSizeX - this->calculateTextSize(humidityText)) / 2, this->halfScreenY + this->myScreen.fontSizeY(), humidityText, whiteColour);

        this->timer_start = millis();
    }

    if (dayCycle != this->previousDayCycle || isMinutePassed)
    {
        this->previousDayCycle = dayCycle;
        tImage image = (this->previousDayCycle == DayCycle_DAY)? sun : moon;
        this->drawImage(image, 0 + this->myScreen.fontSizeX(), this->screenSizeY - this->myScreen.fontSizeY() - image.height, isLightAuto);
    }
    // else
    // {
    //     if (isMinutePassed) // 1 minute
    //     {
    //         tImage image = (this->previousDayCycle == DayCycle_DAY)? sun : moon;
    //         this->drawImage(image, 0 + this->myScreen.fontSizeX(), this->screenSizeY - this->myScreen.fontSizeY() - image.height, true);
    //     }
    // }

    if (shouldFan && this->fanFlag)
    {
        this->drawImage(fan, halfScreenX - fan.width/2, this->screenSizeY - this->myScreen.fontSizeY()- water.height, isFanAuto);
        this->fanFlag = !this->fanFlag;
    }
    else if (!shouldFan && !this->fanFlag)
    {
        this->drawRectangle(halfScreenX - fan.width/2, this->screenSizeY - this->myScreen.fontSizeY()- water.height, fan.width, fan.height, blackColour, true);
        this->fanFlag = !this->fanFlag;
    }
    else
    {
        if (isMinutePassed)
        {
            this->drawImage(fan, halfScreenX - fan.width/2, this->screenSizeY - this->myScreen.fontSizeY()- water.height, isFanAuto);
        }
    }

    if (shouldWatering && this->wateringFlag)
    {
        this->drawImage(water, this->screenSizeX - this->myScreen.fontSizeX() - water.width, this->screenSizeY - this->myScreen.fontSizeY()- water.height, isWaterAuto);
        this->wateringFlag = !this->wateringFlag;
    }
    else if (!shouldWatering && !this->wateringFlag)
    {
        this->drawRectangle(this->screenSizeX - this->myScreen.fontSizeX() - water.width, this->screenSizeY - this->myScreen.fontSizeY()- water.height, water.width, water.height, blackColour, true);
        this->wateringFlag = !this->wateringFlag;
    }
    else
    {
        if (isMinutePassed) // 1 minute
        {
            this->drawImage(water, this->screenSizeX - this->myScreen.fontSizeX() - water.width, this->screenSizeY - this->myScreen.fontSizeY()- water.height, isWaterAuto);
        }
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