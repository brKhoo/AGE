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
    Position next = p;

    switch(dir){
        case 'd':
            next.row += 1;
            break;
        case 'u':
            next.row -= 1;
            break;
        case 'l':
            next.col -= 1;
            break;
        case 'r':
            next.col += 1;
            break;
        default:
            next.row += 1;
            break;
    }

    e.setPosition(next);
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
