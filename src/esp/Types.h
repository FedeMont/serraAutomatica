#ifndef __TYPES_H__
#define __TYPES_H__

#define DEBUG

#include <Arduino.h>

typedef struct {
    bool isValid;
    char commandType;
    String commandText;
} Command;

#endif
