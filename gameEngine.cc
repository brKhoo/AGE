#include "gameEngine.h"
#include "input.h"
#include "view.h"

GameEngine::GameEngine(int rows, int cols): board(rows, cols) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    timeout(0);

    state.bindStatus("Status1", 1);
    state.bindStatus("Status2", 2);
    state.bindStatus("Status3", 3);

    // Default values
    state.setStatus("Status1", "");
    state.setStatus("Status2", "");
    state.setStatus("Status3", "");
}

GameEngine::~GameEngine(){
    endwin();
}

void GameEngine::addEntity(std::unique_ptr<Entity> e) {
    board.addEntity(std::move(e));
}

void GameEngine::defineStatus(int line, const std::string &name) {
    if (line < 1 || line > 3) return;

    state.bindStatus(name, line);
    state.setStatus(name, "");  // empty by default
}

void GameEngine::run(){
    using namespace std::chrono;
    using namespace std::this_thread;

    while (true) {
        // Always render
        view.draw(board, state);

        // Stop updating once the game is over
        if (state.gameOver)
            break;

        Action a = input.readAction();
        if (a == Action::INVALID) {
            state.gameOver = true;
            state.win = false;
            break;
        }

        state.lastInput = a;

        board.tick(state);

        sleep_for(milliseconds(50));
    }

    // One last draw to ensure final state is visible
    view.draw(board, state);

    // Wait for user before exiting (important!)
    input.waitForAnyKey();
}
