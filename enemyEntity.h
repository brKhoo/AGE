#ifndef ENEMY_ENTITY_H
#define ENEMY_ENTITY_H

#include "entity.h"

class EnemyEntity : public Entity {
public:
    EnemyEntity(){
        setShape(std::make_unique<CharShape>('E'));
    }

    explicit EnemyEntity(char c){
        setShape(std::make_unique<CharShape>(c));
    }

    explicit EnemyEntity(std::unique_ptr<Shape> s){
        setShape(std::move(s));
    }
};


#endif
