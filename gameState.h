#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "action.h"

struct GameState {
    bool gameOver = false;
    int score = 0;
    Action lastInput = Action::NONE;
};

#endif
