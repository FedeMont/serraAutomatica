// #include "Energia.h"
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

    this->previousDayCycle = DayCycle_NONSET;
    this->previousSelectedAction = Action_NONE;

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

void Display::chooseState(State selectedState)
{
    String state, otherState;
    int stateIndex, otherStateIndex;

    if (selectedState == State_MANUAL) {
        state = "MANUAL";
        otherState = "AUTOMATIC";
        stateIndex = 0;
        otherStateIndex = (2 * this->myScreen.fontSizeY());
    } else {
        state = "AUTOMATIC";
        otherState = "MANUAL";
        stateIndex = (2 * this->myScreen.fontSizeY());
        otherStateIndex = 0;
    }

    this->write(0, 0, "Select mode:", greenColour);

    int leftStateBound = (this->myScreen.screenSizeX() - this->stringLength(state)) / 2;
    int leftOtherStateBound = (this->myScreen.screenSizeX() - this->stringLength(otherState)) / 2;
    int upperBound = (this->myScreen.screenSizeY() - this->myScreen.fontSizeY()) / 2;

    this->drawRectangle(0, upperBound, this->myScreen.screenSizeX(), 3 * this->myScreen.fontSizeY(), blackColour, true);

    this->myScreen.setFontSolid(false);
    this->drawRectangle(leftStateBound, upperBound + stateIndex, this->stringLength(state), this->myScreen.fontSizeY(), violetColour, true);
    this->write(leftStateBound, upperBound + stateIndex, state, whiteColour);
    this->myScreen.setFontSolid(true);

    this->write(leftOtherStateBound, upperBound + otherStateIndex, otherState, violetColour);
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

void Display::resetHomeScreenFlags() {
    this->previousDayCycle = DayCycle_NONSET;
    this->previousSelectedAction = Action_NONE;

    this->wateringFlag = true;
}

void Display::homeScreen(String time, bool isMinutePassed, DayCycle dayCycle, float humidity, float temperature, bool shouldWatering)
{
    if (dayCycle != this->previousDayCycle)
    {
        this->drawRectangle(this->myScreen.fontSizeX(), this->myScreen.fontSizeY(), 25, 25, blackColour, true);
        this->drawImage((dayCycle == DayCycle_DAY) ? sun : moon, this->myScreen.fontSizeX(), this->myScreen.fontSizeY());
        this->previousDayCycle = dayCycle;
    }

    if (isMinutePassed)
    {
        this->write((this->myScreen.screenSizeX() - this->calculateTextSize(time) - this->myScreen.fontSizeX()), 2 * this->myScreen.fontSizeY(), time, whiteColour);
    }

    if (millis() - this->timer_start > 1000)
    {
        uint8_t halfScreenY = (this->getScreenSize()[1] - this->myScreen.fontSizeY()) / 2;

        String tempText = "Temperature: " + String(temperature, 2) + "C";
        this->drawRectangle(0, (halfScreenY - this->myScreen.fontSizeY()), this->getScreenSize()[0], this->myScreen.fontSizeY(), blackColour, true);
        this->write((this->getScreenSize()[0] - this->calculateTextSize(tempText)) / 2, halfScreenY - this->myScreen.fontSizeY(), tempText, redColour);

        String humidityText = "Soil hum.: " + String(humidity, 2) + "%";
        this->drawRectangle(0, (halfScreenY + this->myScreen.fontSizeY()), this->getScreenSize()[0], this->myScreen.fontSizeY(), blackColour, true);
        this->write((this->getScreenSize()[0] - this->calculateTextSize(humidityText)) / 2, halfScreenY + this->myScreen.fontSizeY(), humidityText, whiteColour);

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