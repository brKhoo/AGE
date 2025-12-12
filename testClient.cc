#include <memory>
#include <vector>

#include "movement.h"
#include "collision.h"
#include "playerEntity.h"
#include "enemyEntity.h"
#include "gameEngine.h"
#include "shape.h"

int main() {
    // Game spec: 80x25 total, 3 status lines at bottom
    GameEngine engine(25, 80);
    //engine.setBorder("View");
    // Set up status variables
    engine.defineStatus(1, "Lives", 3);
    engine.defineStatus(2, "Name", "Brock");
    engine.defineStatus(3, "Gameover?", false);

    engine.setStatus("Lives", 2);
    engine.setStatus("Name", "Khoo");
    engine.setStatus("Gameover?", false);

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
    smile->setHeight(0);
    smile->setCollision(std::make_unique<SolidCollision>());
    engine.addEntity(std::move(smile));

    // Player entity in the middle
    auto player = std::make_unique<PlayerEntity>(std::make_unique<RectShape>(4, 2, 'P'));
    player->setPosition({10, 10});
    player->setHeight(0);
    player->setHealth(3);
    player->addMovement(std::make_unique<PlayerMovement>());
    player->setCollision(std::make_unique<PassThroughCollision>());
    engine.addEntity(std::move(player));

    // Straight-moving enemy
    auto enemy = std::make_unique<EnemyEntity>('E');
    enemy->setPosition({5,6});
    enemy->setHeight(0);
    enemy->setHealth(3);
    //enemy->addMovement(std::make_unique<StraightMovement>(1, 0));
    enemy->setCollision(std::make_unique<DestroySelfCollision>());
    engine.addEntity(std::move(enemy));

    // Gravity object that falls down
    auto falling = std::make_unique<EnemyEntity>('*');
    falling->setPosition({1, 30});
    falling->setHeight(0);
    falling->setCollision(std::make_unique<DestroySelfCollision>());
    engine.addEntity(std::move(falling));

    // Animated blinking object that cycles
    std::vector<std::vector<Pixel>> frames = {
        { Pixel{0,0,'o'}, Pixel{0,1,'o'} },
        { Pixel{0,0,'O'}, Pixel{0,1,'O'} },
        { Pixel{0,0,'X'}, Pixel{0,1,'X'} }
    };
    auto animShape = std::make_unique<AnimatedShape>(frames);
    auto cycler = std::make_unique<EnemyEntity>(' ');  // placeholder
    cycler->setPosition({1, 10});
    cycler->setShape(std::move(animShape));
    cycler->addMovement(std::make_unique<CyclingMovement>(5));
    cycler->setCollision(std::make_unique<DestroyBothCollision>());
    engine.addEntity(std::move(cycler));

    engine.run();
}
