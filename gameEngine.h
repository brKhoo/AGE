#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <chrono>
#include <thread>
#include "gameBoard.h"
#include "gameState.h"

class Input;
class View;

class GameEngine {
    GameBoard &board;
    GameState &state;
    Input &input;
    View &view;
public:
    GameEngine(GameBoard &b, GameState &s, Input &i, View &v)
        : board{b}, state{s}, input{i}, view{v} {}

    void run();
};

#endif
