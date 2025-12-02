// movement.cc
#include "movement.h"
#include "entity.h"
#include "gameState.h"
#include "action.h"

void StraightMovement::apply(Entity &e, GameState &) {
    auto p = e.position();
    p.row += dr;
    p.col += dc;
    e.setPosition(p);
}

void GravityMovement::apply(Entity &e, GameState &) {
    auto p = e.position();
    p.row += dr;
    p.col += dc;
    e.setPosition(p);
}

void CyclingMovement::apply(Entity &e, GameState &) {
    if (++ticks >= interval) {
        e.shape().advanceFrame();
        ticks = 0;
    }
}

void PlayerMovement::apply(Entity &e, GameState &state) {
    auto p = e.position();
    switch (state.lastInput) {
        case Action::UP:    p.row--; break;
        case Action::DOWN:  p.row++; break;
        case Action::LEFT:  p.col--; break;
        case Action::RIGHT: p.col++; break;
        default: break;
    }
    e.setPosition(p);
}
