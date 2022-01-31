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

    Display();

    void begin();
    void clear();
    void write(int, int, String, uint16_t);
    void write(int, int, String, int, uint16_t);
    void drawRectangle(int, int, int, int, uint16_t, bool);
    void drawImage(tImage, uint16_t, uint16_t);
    void drawImage(tImage, uint16_t, uint16_t, bool);
    void connecting();
    void chooseState(State);
    void chooseTime(int, int[]);
    void resetHomeScreenFlags();
    void homeScreen(bool, String, bool, DayCycle, float, float, bool, bool, bool, bool, bool);
    
    int calculateTextSize(String);
    int *getScreenSize();

private:
    uint8_t screenSizeX;
    uint8_t screenSizeY;
    uint8_t halfScreenY;
    uint8_t halfScreenX;

    Screen_HX8353E myScreen;

    bool previousConnectionState;
    DayCycle previousDayCycle;

    bool fanFlag;
    bool wateringFlag;

    Action previousSelectedAction;

    long timer_start;
    long timer_dayCycle;
    long timer_watering;
    long timer_state;
};

#endif
