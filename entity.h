#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include "shape.h"
#include "gameState.h"

struct Position {
    int row = 0;
    int col = 0;
};

class Movement;
class CollisionBehavior;

class Entity {
protected:
    Position pos;
    int heightLevel = 0;
    bool removeFlag = false;
    std::unique_ptr<Shape> shapePtr;

    std::vector<std::unique_ptr<Movement>> *movements;
    std::unique_ptr<CollisionBehavior> *collision;

public:
    Entity();
    virtual ~Entity();

    void addMovement(std::unique_ptr<Movement> m);
    void setCollision(std::unique_ptr<CollisionBehavior> c);

    void updateMovements(GameState &state);
    virtual void updateSelf(GameState &) {}

    // DECLARATION ONLY
    virtual void onCollide(Entity &other, GameState &state);

    CollisionBehavior &collisionRule();

    Position position() const { return pos; }
    void setPosition(Position p) { pos = p; }

    int height() const { return heightLevel; }
    void setHeight(int h) { heightLevel = h; }

    void markForRemoval() { removeFlag = true; }
    bool isMarkedForRemoval() const { return removeFlag; }

    virtual Shape &shape() { return *shapePtr; }
    virtual const Shape &shape() const { return *shapePtr; }
    void setShape(std::unique_ptr<Shape> s){
        shapePtr = std::move(s);
    }
};

#endif
