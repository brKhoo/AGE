#include <memory>
#include <vector>
#include "gameEngine.h"
#include "movement.h"
#include "collision.h"
#include "shape.h"
#include "entity.h"

int main() {
    // Create engine and board
    GameEngine engine(25, 80);
    engine.setBorder("view");

    // Status lines
    engine.defineStatus(1, "Player", 0);
    engine.defineStatus(2, "CPU", 0);
    engine.defineStatus(3, "Gameover", false);
    
    // Player
    auto player = std::make_unique<Entity>(
        std::make_unique<RectShape>(4, 2, 'X'),
        std::make_unique<PassThroughCollision>(),
        0
    );
    player->setPosition({10, 3});
    player->addMovement(std::make_unique<PlayerMovement>());
    engine.addEntity(std::move(player));

    // Animated blinking object that cycles
    std::vector<std::vector<Pixel>> frames = {
        { Pixel{0,0,'o'}, Pixel{0,1,'o'} },
        { Pixel{0,0,'O'}, Pixel{0,1,'O'} },
        { Pixel{0,0,'X'}, Pixel{0,1,'X'} }
    };
    auto animShape = std::make_unique<AnimatedShape>(frames);
    auto cycler = std::make_unique<Entity>(
        std::make_unique<RectShape>(4, 1, '|'),
        std::make_unique<SolidCollision>(),
        0
    ); // Placeholder
    cycler->setPosition({5, 10});
    cycler->setShape(std::move(animShape));
    cycler->addMovement(std::make_unique<CyclingMovement>(5));
    cycler->addMovement(std::make_unique<GravityMovement>('r'));
    cycler->setCollision(std::make_unique<PassThroughCollision>());
    cycler->setHeight(1);
    engine.addEntity(std::move(cycler));

    // Destroy Entity
    auto dstry = std::make_unique<Entity>(
        std::make_unique<CharShape>('D'),
        std::make_unique<DestroySelfCollision>(),
        0
    );
    dstry->setPosition({12, 40});
    engine.addEntity(std::move(dstry));

    // Solid entity
    auto s = std::make_unique<Entity>(
        std::make_unique<CharShape>('S'),
        std::make_unique<SolidCollision>(),
        0
    );
    s->setPosition({12, 20});
    s->addMovement(std::make_unique<StraightMovement>(0, 1)); // Move right
    engine.addEntity(std::move(s));

    // Solid Entity
    auto s1 = std::make_unique<Entity>(
        std::make_unique<CharShape>('I'),
        std::make_unique<SolidCollision>(),
        0
    );
    s1->setPosition({12, 60});
    engine.addEntity(std::move(s1));

    // Win Entity
    auto s2 = std::make_unique<Entity>(
        std::make_unique<CharShape>('W'),
        std::make_unique<WinCollision>(),
        0
    );
    s2->setPosition({20, 60});
    engine.addEntity(std::move(s2));

    // Bounce Entity
    auto s3 = std::make_unique<Entity>(
        std::make_unique<CharShape>('B'),
        std::make_unique<BounceCollision>(),
        0
    );
    s3->setPosition({12, 20});
    s3->addMovement(std::make_unique<StraightMovement>(0, 1)); // Move right
    engine.addEntity(std::move(s3));

    // Ball
    Entity *ballPtr = nullptr;
    auto ball = std::make_unique<Entity>(
        std::make_unique<CharShape>('O'),
        std::make_unique<BounceCollision>(),
        0
    );
    ball->setPosition({12, 40});
    ball->addMovement(std::make_unique<StraightMovement>(1, 1)); // Move diagonally
    engine.addEntity(std::move(ball));

    // Game loop
    engine.run();
}
