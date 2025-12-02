// collision.cc
#include "collision.h"
#include "entity.h"
#include "gameState.h"

void PassThroughCollision::handle(Entity &, Entity &, GameState &) {}

void SolidCollision::handle(Entity &, Entity &, GameState &) {}

void DestroyCollision::handle(Entity &self, Entity &, GameState &) {
    self.markForRemoval();
}

void PlayerDieCollision::handle(Entity &, Entity &, GameState &state) {
    state.gameOver = true;
}
