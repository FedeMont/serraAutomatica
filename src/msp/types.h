#ifndef __ACTION_H__
#define __ACTION_H__

#define DEBUG

enum Action
{
    Action_UP,
    Action_DOWN,
    Action_LEFT,
    Action_RIGHT,
    Action_SELECT,
    Action_BUP,
    Action_BDOWN,
    Action_NONE
};

enum State
{
    State_MANUAL,
    State_AUTOMATIC,
    State_NONE
};

enum DayCycle
{
    DayCycle_DAY,
    DayCycle_NIGHT,
    DayCycle_NONSET
};

typedef struct {
    bool isValid;
    char commandType;
    String commandText;
    String chatId;
} Command;

typedef struct
{
    const uint16_t *data;
    uint16_t width;
    uint16_t height;
    uint8_t dataSize;
} tImage;

#endif
