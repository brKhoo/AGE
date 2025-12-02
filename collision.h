// collision.h
#ifndef COLLISION_H
#define COLLISION_H

class Entity;
struct GameState;

class CollisionBehavior {
public:
    virtual ~CollisionBehavior() = default;
    virtual void handle(Entity &self, Entity &other, GameState &state) = 0;
};

class PassThroughCollision : public CollisionBehavior {
public:
    void handle(Entity &self, Entity &other, GameState &state) override;
};

class SolidCollision : public CollisionBehavior {
public:
    void handle(Entity &self, Entity &other, GameState &state) override;
};

class DestroyCollision : public CollisionBehavior {
public:
    void handle(Entity &self, Entity &other, GameState &state) override;
};

class PlayerDieCollision : public CollisionBehavior {
public:
    void handle(Entity &self, Entity &other, GameState &state) override;
};

#endif
