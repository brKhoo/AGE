#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <ncurses.h>
#include <chrono>
#include <thread>
#include "gameBoard.h"
#include "gameState.h"
#include "cursesInput.h"
#include "cursesView.h"

class GameEngine {
    GameBoard board;
    GameState state;
    CursesInput input;
    CursesView view;
public:
    GameEngine(int rows, int cols);
    ~GameEngine();
    void run();

    GameState &getState() { return state; }

    void setBorder(const std::string &border);

    // Name a status line
    void defineStatus(int line, const std::string &name);

    // Name and initial value
    template<typename T> void defineStatus(int line, const std::string &name, T initial) {
        defineStatus(line, name);
        state.setStatus(name, initial);
    }

    template<typename T> void setStatus(const std::string &name, T value) {
        state.setStatus(name, value);
    }

    void addEntity(std::unique_ptr<Entity> e);

    void step();
    void draw();
    bool isGameOver() const;
    int cols() const;
    int rows() const;
    void endGame(bool didWin);
};

#endif
