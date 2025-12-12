#include "entity.h"
#include "gameState.h"
#include "gameBoard.h"
#include "movement.h"
#include "collision.h"

Entity::Entity(
    std::unique_ptr<Shape> s,
    std::unique_ptr<CollisionBehavior> c,
    int height,
    int hp): 
    heightLevel{height},
    health{hp},
    shapePtr{s ? std::move(s) : std::make_unique<CharShape>('E')},
    movements{},
    collision{c ? std::move(c) : std::make_unique<PassThroughCollision>()}
{}

Entity::~Entity() = default;

void Entity::addMovement(std::unique_ptr<Movement> m){
    movements.push_back(std::move(m));
}

void Entity::setCollision(std::unique_ptr<CollisionBehavior> c){
    collision = std::move(c);
}

CollisionBehavior &Entity::collisionRule(){
    return *collision;
}

void Entity::updateMovements(GameState &state){
    prevPos = pos;
    for(auto &m:movements){
        m->apply(*this, state);
    }
}

void Entity::onCollide(Entity &other, GameState &state){
    collisionRule().handle(*this, other, state);
}

void Entity::clearMovements(){
    movements.clear();
}

void Entity::tickCooldown(){
        if(damageCooldown > 0)
            --damageCooldown;   
    }

void Entity::takeDamage(int dmg){
    if(damageCooldown > 0) return;
    health -= dmg;
    damageCooldown = 10;
    if(health <= 0){
        markForRemoval();
    }
}
