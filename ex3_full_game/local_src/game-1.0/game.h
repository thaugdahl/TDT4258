#ifndef _GAME_H
#define _GAME_H
#include <stdint.h>

#define SW1 (1 << 0)
#define SW2 (1 << 1)
#define SW3 (1 << 2)
#define SW4 (1 << 3)
#define SW5 (1 << 4)
#define SW6 (1 << 5)
#define SW7 (1 << 6)
#define SW8 (1 << 7)

typedef enum gameState {
    LOADING,
    MENU,
    GAME
} gameState;

#endif