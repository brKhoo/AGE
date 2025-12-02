#ifndef ENEMY_ENTITY_H
#define ENEMY_ENTITY_H

#include "entity.h"

// Simple enemy that can be given movement + collision policies.
class EnemyEntity : public Entity {
    std::unique_ptr<Shape> shp;
public:
    explicit EnemyEntity(char c) : shp{std::make_unique<CharShape>(c)} {}

    Shape &shape() override { return *shp; }
    const Shape &shape() const override { return *shp; }

    void setShape(std::unique_ptr<Shape> s) {
        shp = std::move(s);
    }
};


#endif
