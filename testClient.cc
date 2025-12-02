#include <memory>
#include <vector>
#include <ncurses.h>

#include "gameBoard.h"
#include "testBoard.h"
#include "gameState.h"
#include "movement.h"
#include "collision.h"
#include "playerEntity.h"
#include "enemyEntity.h"
#include "cursesView.h"
#include "cursesInput.h"
#include "gameEngine.h"
#include "shape.h"

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);  // These last two lines for no input delay
    timeout(0);
    keypad(stdscr, TRUE);


    // Game spec: 80x25 total, 3 status lines at bottom
    int rows = 25;
    int cols = 80;
    TestBoard board(rows, cols);
    GameState state;

    // Player entity in the middle
    auto player = std::make_unique<PlayerEntity>('@');
    player->setPosition({10, 10});
    player->setHeight(0);
    player->addMovement(std::make_unique<PlayerMovement>());
    player->setCollision(std::make_unique<PlayerDieCollision>());
    board.addEntity(std::move(player));

    // Straight-moving enemy
    auto enemy = std::make_unique<EnemyEntity>('E');
    enemy->setPosition({5, 5});
    enemy->setHeight(0);
    enemy->addMovement(std::make_unique<StraightMovement>(0, 1)); // move right
    enemy->setCollision(std::make_unique<DestroyCollision>());
    board.addEntity(std::move(enemy));

    // Gravity object that falls down
    auto falling = std::make_unique<EnemyEntity>('*');
    falling->setPosition({1, 30});
    falling->setHeight(0);
    falling->addMovement(std::make_unique<GravityMovement>(1, 0)); // falling down
    falling->setCollision(std::make_unique<DestroyCollision>());
    board.addEntity(std::move(falling));

    // Animated blinking object that cycles
    std::vector<std::vector<Pixel>> frames = {
        { Pixel{0,0,'o'}, Pixel{0,1,'o'} },
        { Pixel{0,0,'O'}, Pixel{0,1,'O'} }
    };
    AnimatedShape anim(frames);
    auto cycler = std::make_unique<EnemyEntity>(' '); // placeholder char, unused
    // We can't store AnimatedShape directly in EnemyEntity, but this at least
    // demonstrates CyclingMovement; the sprite itself is static here.
    cycler->setPosition({8, 40});
    cycler->setHeight(0);
    cycler->addMovement(std::make_unique<CyclingMovement>(5));
    cycler->setCollision(std::make_unique<PassThroughCollision>());
    board.addEntity(std::move(cycler));

    CursesInput input;
    CursesView view;
    GameEngine engine(board, state, input, view);
    engine.run();

    endwin();
    return 0;
}
