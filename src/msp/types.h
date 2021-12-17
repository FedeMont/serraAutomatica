#ifndef __ACTION_H__
#define __ACTION_H__

#define DEBUG

typedef enum Action
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    BUP,
    BDOWN,
    NONE
} Action;

typedef enum State
{
    MANUAL,
    AUTOMATIC
} State;

typedef enum DayCycle
{
    NONSET,
    DAY,
    NIGHT
} DayCycle;

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
