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

    // Set up status variables
    state.bindStatus("Lives", 1);
    state.bindStatus("Name", 2);
    state.bindStatus("GameOver?", 3);

    state.setStatus("Lives", 3);
    state.setStatus("Name", "Brock");
    state.setStatus("GameOver?", false);

    // Bitmap enemy
    std::vector<Pixel> smiley = {
        {0, 0, 'I'},
        {0, 1, 'L'},
        {0, 2, 'O'},
        {1, 1, 'V'}
    };
    auto shape = std::make_unique<BitmapShape>(smiley);
    auto smile = std::make_unique<EnemyEntity>(std::move(shape));
    smile->setPosition({12, 12});
    smile->setHeight(1);
    smile->setCollision(std::make_unique<SolidCollision>());
    board.addEntity(std::move(smile));

    // Player entity in the middle
    auto player = std::make_unique<PlayerEntity>(std::make_unique<RectShape>(4, 2, 'P'));
    player->setPosition({10, 10});
    player->setHeight(0);
    player->addMovement(std::make_unique<PlayerMovement>());
    player->addMovement(std::make_unique<GravityMovement>('r'));
    player->setCollision(std::make_unique<SolidCollision>());
    board.addEntity(std::move(player));

    // Straight-moving enemy
    auto enemy = std::make_unique<EnemyEntity>('E');
    enemy->setPosition({5,6});
    enemy->setHeight(0);
    enemy->addMovement(std::make_unique<StraightMovement>(1, 1)); // move right
    enemy->setCollision(std::make_unique<BounceCollision>());
    board.addEntity(std::move(enemy));

    // Gravity object that falls down
    auto falling = std::make_unique<EnemyEntity>('*');
    falling->setPosition({1, 30});
    falling->setHeight(0);
    falling->addMovement(std::make_unique<GravityMovement>('d')); // falling down
    falling->setCollision(std::make_unique<DestroyCollision>());
    board.addEntity(std::move(falling));

    // Animated blinking object that cycles
    std::vector<std::vector<Pixel>> frames = {
        { Pixel{0,0,'o'}, Pixel{0,1,'o'} },
        { Pixel{0,0,'O'}, Pixel{0,1,'O'} },
        { Pixel{0,0,'X'}, Pixel{0,1,'X'} }
    };
    auto animShape = std::make_unique<AnimatedShape>(frames);
    auto cycler = std::make_unique<EnemyEntity>(' ');  // placeholder
    cycler->setPosition({1, 10});
    // give it the animated shape
    cycler->setShape(std::move(animShape));
    // add the cycling movement every 3 ticks
    cycler->addMovement(std::make_unique<CyclingMovement>(5));
    cycler->addMovement(std::make_unique<GravityMovement>('d')); // combine with gravity movement
    // no blocking collisions
    cycler->setCollision(std::make_unique<SolidCollision>());
    // add to board
    board.addEntity(std::move(cycler));


    CursesInput input;
    CursesView view;
    GameEngine engine(board, state, input, view);
    engine.run();

    endwin();
    return 0;
}
