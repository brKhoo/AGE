#include "gameEngine.h"
#include "input.h"
#include "view.h"

void GameEngine::run() {
    using namespace std::chrono;
    using namespace std::this_thread;

    while (!state.gameOver) {
        // Render before reading input so the frame rate feels steady
        view.draw(board, state);

        Action a = input.readAction();
        if (a == Action::INVALID) {
            state.gameOver = true;
            break;
        }
        
        state.lastInput = a;

        board.tick(state);

        sleep_for(milliseconds(50));
    }

    // Final frame
    view.draw(board, state);
}
