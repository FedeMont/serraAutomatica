#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Energia.h"
#include "Screen_HX8353E.h"

#include "day.h"
#include "night.h"
#include "fan.h"
#include "water.h"
#include "connection.h"
#include "notconnection.h"

#include "Types.h"

class Display
{
public:
    Display();
    ~Display();

    void begin();

    void clear();

    void connecting();
    void chooseTime(int, int[]);
    void resetHomeScreenFlags();
    void homeScreen(bool, String, bool, DayCycle, float, float, bool, bool, bool, bool, bool);

private:
    uint16_t blackColour;
    uint16_t whiteColour;
    uint16_t redColour;
    uint16_t greenColour;
    uint16_t blueColour;
    uint16_t yellowColour;
    uint16_t cyanColour;
    uint16_t orangeColour;
    uint16_t magentaColour;
    uint16_t violetColour;
    uint16_t grayColour;
    uint16_t darkGrayColour;

    uint8_t screenSizeX;
    uint8_t screenSizeY;
    uint8_t halfScreenY;
    uint8_t halfScreenX;

    Screen_HX8353E myScreen;

    bool previousConnectionState;
    DayCycle previousDayCycle;

    bool fanFlag;
    bool wateringFlag;

    bool connectingFlag;
    long timer_start;

    int calculateTextSize(String);

    void write(int, int, String, uint16_t);
    void write(int, int, String, int, uint16_t);
    void drawRectangle(int, int, int, int, uint16_t, bool);
    void drawImage(tImage, uint16_t, uint16_t);
    void drawImage(tImage, uint16_t, uint16_t, bool);
};

#endif
