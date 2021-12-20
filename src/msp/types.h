#ifndef __ACTION_H__
#define __ACTION_H__

#define DEBUG

enum class Action
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    BUP,
    BDOWN,
    NONE
};

enum class State
{
    MANUAL,
    AUTOMATIC,
    NONE
};

enum class DayCycle
{
    DAY,
    NIGHT,
    NONSET
};

typedef struct {
    String commandType;
    String commandText;
} Command;

typedef struct
{
    const uint16_t *data;
    uint16_t width;
    uint16_t height;
    uint8_t dataSize;
} tImage;

#endif
