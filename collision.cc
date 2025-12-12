// collision.cc
#include "collision.h"
#include "entity.h"
#include "gameState.h"

CollisionResult PassThroughCollision::handle(Entity &, Entity &, GameState &){
    return CollisionResult::Pass;
}

CollisionResult BounceCollision::handle(Entity &, Entity &, GameState &){
    return CollisionResult::Bounce;
}

CollisionResult SolidCollision::handle(Entity &, Entity &, GameState &){
    return CollisionResult::Block;
}

DamageCollision::DamageCollision(int d) : damage(d) {}

CollisionResult DamageCollision::handle(Entity &, Entity &other, GameState &) {
    other.takeDamage(damage);
    return CollisionResult::Pass;
}

CollisionResult DestroySelfCollision::handle(Entity &self, Entity &, GameState &) {
    self.markForRemoval();
    return CollisionResult::Destroy;
}

CollisionResult DestroyBothCollision::handle(Entity &self, Entity &other, GameState &) {
    self.markForRemoval();
    other.markForRemoval();
    return CollisionResult::Destroy;
}

CollisionResult WinCollision::handle(Entity &self, Entity &, GameState &state) {
    state.win = true;
    state.gameOver = true;
    return CollisionResult::Pass;
}

CollisionResult LossCollision::handle(Entity &, Entity &, GameState &state) {
    state.win = false;
    state.gameOver = true;
    return CollisionResult::Pass;
}

// Allows collisions to be combined
void CompositeCollision::add(std::unique_ptr<CollisionBehavior> r) {
    rules.push_back(std::move(r));
}

CollisionResult CompositeCollision::handle(Entity &self, Entity &other, GameState &state) {
    CollisionResult phys = CollisionResult::Pass;
    for (auto &r : rules) {
        CollisionResult res = r->handle(self, other, state);
        if (res != CollisionResult::Pass)
            phys = res;
    }
    return phys;
}
