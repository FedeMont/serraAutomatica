#ifndef __TYPES_H__
#define __TYPES_H__

#define DEBUG

#include <Arduino.h>

typedef struct {
    bool isValid;
    char commandType;
    String commandText;
} Command;

enum State
{
    State_MANUAL,
    State_AUTOMATIC,
    State_NONE
};

#endif
