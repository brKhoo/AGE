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
    Position prevPos;
    int heightLevel = 0;
    int health = 1; // Default 1 health point
    int damageCooldown = 0; // number of ticks before entity can take damage again
    bool removeFlag = false;
    int offScreenTicks = 0;
    int maxOffScreenTicks = 10;
    bool isPlayer = false;
    std::string _tag;

    std::unique_ptr<Shape> shapePtr;
    std::vector<std::unique_ptr<Movement>> movements;
    std::unique_ptr<CollisionBehavior> collision;

public:
    Entity(
        std::unique_ptr<Shape> s = nullptr,
        std::unique_ptr<CollisionBehavior> c = nullptr,
        int h = 0,
        int hp = 1
    );
    virtual ~Entity();

    void markPlayer(bool b = true) { isPlayer = b; }
    bool isPlayerControlled() { return isPlayer; }
    int getOffScreenTicks() { return offScreenTicks; }
    void setOffScreenTicks(int t) { offScreenTicks = t; }
    int getMaxOffScreenTicks() { return maxOffScreenTicks; }
    void setMaxOffScreenTicks(int t) { maxOffScreenTicks = t; }

    void setTag(const std::string &name) { _tag = name; }
    const std::string &tag() const { return _tag; }

    void setHealth(int h){ health = h; }
    int getHealth() const { return health; }
    virtual void tickCooldown(){
        if(damageCooldown > 0)
            --damageCooldown;   
    }
    virtual void takeDamage(int dmg) {
        if(damageCooldown > 0) return;
        health -= dmg;
        damageCooldown = 10;
        if(health <= 0){
            markForRemoval();
        }
    }

    void addMovement(std::unique_ptr<Movement> m);
    void clearMovements();
    void setCollision(std::unique_ptr<CollisionBehavior> c);

    void updateMovements(GameState &state);
    virtual void updateSelf(GameState &) {}

    virtual void onCollide(Entity &other, GameState &state);

    CollisionBehavior &collisionRule();

    Position position() const { return pos; }
    void setPosition(Position p) { pos = p; }
    Position prevPosition() const { return prevPos; }
    void revertPosition(){ pos = prevPos; }

    std::vector<std::unique_ptr<Movement>> &movementComponents() {
        return movements;
    }


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
