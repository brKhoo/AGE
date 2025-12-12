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

CollisionResult DestroyCollision::handle(Entity &self, Entity &, GameState &) {
    self.markForRemoval();
    return CollisionResult::Destroy;
}

CollisionResult PlayerDieCollision::handle(Entity &, Entity &, GameState &state) {
    state.gameOver = true;
    return CollisionResult::Destroy;
}
