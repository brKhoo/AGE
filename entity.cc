#include "entity.h"
#include "movement.h"
#include "collision.h"

Entity::Entity() {
    movements = new std::vector<std::unique_ptr<Movement>>();
    collision  = new std::unique_ptr<CollisionBehavior>();
}

Entity::~Entity() {
    delete movements;
    delete collision;
}

void Entity::addMovement(std::unique_ptr<Movement> m) {
    movements->push_back(std::move(m));
}

void Entity::setCollision(std::unique_ptr<CollisionBehavior> c) {
    *collision = std::move(c);
}

CollisionBehavior &Entity::collisionRule() {
    return **collision;
}

void Entity::updateMovements(GameState &state) {
    for (auto &m : *movements)
        m->apply(*this, state);
}

void Entity::onCollide(Entity &other, GameState &state) {
    collisionRule().handle(*this, other, state);
}
