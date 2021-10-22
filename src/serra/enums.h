#ifndef __ACTION_H__
#define __ACTION_H__

typedef enum Action {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    BUP,
    BDOWN,
    NONE
} Action;

typedef enum DayCycle {
    NONSET,
    DAY,
    NIGHT
} DayCycle;

#endif
