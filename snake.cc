#include "gameEngine.h"
#include "movement.h"
#include "collision.h"
#include "shape.h"
#include <chrono>
#include <thread>
#include <random>

// Hacky helper func to spawn the apple in a new free locatoin
Position randomFreePosition(GameEngine &engine){
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> rowDist(1, engine.rows() - 6);
    std::uniform_int_distribution<int> colDist(1, engine.cols() - 2);
    return { rowDist(rng), colDist(rng) };
}

void spawnApple(GameEngine &engine, SnakeMovement &snake){
    Position pos;
    while(true){
        pos = randomFreePosition(engine);
        bool collision = false;
        for(const auto &seg:snake.segments()){
            if(seg.row == pos.row && seg.col == pos.col){
                collision = true;
                break;
            }
        }
        if(!collision) break;
    }

    auto apple = std::make_unique<Entity>(
        std::make_unique<CharShape>('a'),
        std::make_unique<EatAppleCollision>(snake),
        0
    );
    apple->setTag("apple");
    apple->setPosition(pos);
    engine.addEntity(std::move(apple));
}

int main(){
    GameEngine engine(25, 80);
    engine.setBorder("solid");

    engine.defineStatus(1, "Score", 0);
    engine.defineStatus(2, "Gameover", false);
    engine.defineStatus(3, "q to quit", "");

    // Snake entity
    auto head = std::make_unique<Entity>(
        std::make_unique<CharShape>('O'),
        std::make_unique<SnakeHeadCollision>(),
        0
    );
    head->setTag("snake");
    head->setPosition({12, 40});
    auto snakeMove = std::make_unique<SnakeMovement>(Position{12, 40});
    SnakeMovement *snakePtr = snakeMove.get();
    head->addMovement(std::move(snakeMove));
    engine.addEntity(std::move(head));

    // Apple entity
    spawnApple(engine, *snakePtr);

    // Game loop
    int score = 0;
    while(!engine.isGameOver()){
        engine.draw();
        engine.step();

        // React after physics
        if(engine.getState().appleEatenThisTick){
            snakePtr->grow();
            engine.setStatus("Score", ++score);
            engine.getState().appleEatenThisTick = false;
            spawnApple(engine, *snakePtr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
    // Final frame
    engine.draw();
}
