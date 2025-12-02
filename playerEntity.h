#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "entity.h"

class PlayerEntity : public Entity {
public:
    PlayerEntity(){
        setShape(std::make_unique<CharShape>('P'));
    }

    explicit PlayerEntity(char c){
        setShape(std::make_unique<CharShape>(c));
    }

    explicit PlayerEntity(std::unique_ptr<Shape> s){
        setShape(std::move(s));
    }

    void updateSelf(GameState &) override {}
};

#endif
