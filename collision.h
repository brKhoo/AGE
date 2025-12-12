// collision.h
#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <memory>

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

class DamageCollision : public CollisionBehavior {
    int damage;
public:
    explicit DamageCollision(int d);
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class DestroySelfCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class DestroyBothCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class WinCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

class LossCollision : public CollisionBehavior {
public:
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};

// Allows collisions to be combined
class CompositeCollision : public CollisionBehavior {
    std::vector<std::unique_ptr<CollisionBehavior>> rules;
public:
    void add(std::unique_ptr<CollisionBehavior> r);
    CollisionResult handle(Entity &self, Entity &other, GameState &state) override;
};


#endif
