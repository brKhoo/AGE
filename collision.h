// collision.h
#ifndef COLLISION_H
#define COLLISION_H

class Entity;
struct GameState;

enum class CollisionResult{
    Pass,
    Block,
    Bounce,
    Destroy
};

class CollisionBehavior {
public:
    virtual ~CollisionBehavior() = default;
    virtual CollisionResult handle(Entity &self, Entity &other, GameState &state) = 0;
};

class PassThroughCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class BounceCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class SolidCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class DestroyCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class PlayerDieCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

#endif
