#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Energia.h"
#include "Screen_HX8353E.h"

#include "day.h"
#include "night.h"
#include "water.h"

#include "types.h"

class Display
{
public:
    const uint16_t blackColour = 0b0000000000000000;
    const uint16_t whiteColour = 0b1111111111111111;
    const uint16_t redColour = 0b1111100000000000;
    const uint16_t greenColour = 0b0000011111100000;
    const uint16_t blueColour = 0b0000000000011111;
    const uint16_t yellowColour = 0b1111111111100000;
    const uint16_t cyanColour = 0b0000011111111111;
    const uint16_t orangeColour = 0b1111101111100000;
    const uint16_t magentaColour = 0b1111100000001111;
    const uint16_t violetColour = 0b1111100000011111;
    const uint16_t grayColour = 0b0111101111101111;
    const uint16_t darkGrayColour = 0b0011100111100111;

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
    void homeScreen(String, bool, DayCycle, int, bool);

    int calculateTextSize(String);
    int *getScreenSize();

private:
    DayCycle previousDayCycle = DayCycle::NONSET;
    Screen_HX8353E myScreen;
    bool wateringFlag = true;

    Action previousSelectedAction = Action::NONE;

    long timer_start;
};

#endif
