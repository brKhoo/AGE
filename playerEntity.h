#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "entity.h"

class PlayerEntity : public Entity {
    CharShape sprite;
public:
    PlayerEntity() : sprite('@') {}
    explicit PlayerEntity(char c) : sprite(c) {}

    void updateSelf(GameState &) override {}

    Shape &shape() override { return sprite; }
    const Shape &shape() const override { return sprite; }
};

#endif
