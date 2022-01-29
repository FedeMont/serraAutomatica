#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Energia.h"
#include "Screen_HX8353E.h"

#include "day.h"
#include "night.h"
#include "water.h"

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
    int stringLength(String);
    void write(int, int, String, uint16_t);
    void write(int, int, String, int, uint16_t);
    void drawRectangle(int, int, int, int, uint16_t, bool);
    void drawImage(tImage, uint16_t, uint16_t);
    void chooseState(State);
    void chooseTime(int, int[]);
    void resetHomeScreenFlags();
    void homeScreen(String, bool, DayCycle, float, float, bool);
    
    int calculateTextSize(String);
    int *getScreenSize();

private:
    DayCycle previousDayCycle;
    Screen_HX8353E myScreen;
    bool wateringFlag;

    Action previousSelectedAction;

    long timer_start;
};

#endif
