#include <memory>
#include <vector>

#include "gameEngine.h"
#include "movement.h"
#include "collision.h"
#include "shape.h"
#include "entity.h"

int main() {
    // Create engine + board
    GameEngine engine(25, 80);
    engine.setBorder("solid");

    // Status lines
    engine.defineStatus(1, "Player", 0);
    engine.defineStatus(2, "CPU", 0);
    engine.defineStatus(3, "Gameover", false);

    // Player
    auto player = std::make_unique<Entity>(
        std::make_unique<RectShape>(4, 1, '|'),
        std::make_unique<SolidCollision>(),
        0
    );
    player->setPosition({10, 3});
    player->addMovement(std::make_unique<PlayerMovement>());
    engine.addEntity(std::move(player));

    // CPU
    auto cpu = std::make_unique<Entity>(
        std::make_unique<RectShape>(4, 1, '|'),
        std::make_unique<BounceCollision>(),
        0
    );
    cpu->setPosition({10, 76});
    cpu->addMovement(std::make_unique<StraightMovement>(1, 0)); // move down
    engine.addEntity(std::move(cpu));

    // Ball
    Entity *ballPtr = nullptr;
    auto ball = std::make_unique<Entity>(
        std::make_unique<CharShape>('O'),
        std::make_unique<BounceCollision>(),
        0
    );
    ball->setPosition({12, 40});
    ball->addMovement(std::make_unique<StraightMovement>(1, 1));
    
    ballPtr = ball.get();
    engine.addEntity(std::move(ball));

    auto resetBall = [&](int dir) {
        ballPtr->setPosition({12, 40});
        ballPtr->clearMovements();
        ballPtr->addMovement(std::make_unique<StraightMovement>(1, dir));
    };

    // Custom game loop
    int playerScore = 0;
    int cpuScore = 0;
    while (!engine.isGameOver()) {
        engine.draw();
        engine.step();

        Position p = ballPtr->position();

        if (p.col <= 1) {
            engine.setStatus("CPU", ++cpuScore);   // or increment yourself
            resetBall(+1);
        }
        else if (p.col >= engine.cols() - 2) {
            engine.setStatus("Player", ++playerScore);
            resetBall(-1);
        }

        if(playerScore >= 5){
            engine.setStatus("Gameover", true);
            engine.endGame(true);
        }else if(cpuScore >= 5){
            engine.setStatus("Gameover", true);
            engine.endGame(false);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
