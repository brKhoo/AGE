#ifndef ENEMY_ENTITY_H
#define ENEMY_ENTITY_H

#include "entity.h"

// Simple enemy that can be given movement + collision policies.
class EnemyEntity : public Entity {
    CharShape sprite;
public:
    explicit EnemyEntity(char c) : sprite(c) {}

    void updateSelf(GameState &) override {}

    Shape &shape() override { return sprite; }
    const Shape &shape() const override { return sprite; }
};

#endif
