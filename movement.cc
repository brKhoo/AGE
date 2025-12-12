#include "movement.h"
#include "entity.h"
#include "gameState.h"
#include "action.h"
#include "input.h"

void StraightMovement::apply(Entity &e, GameState &){
    auto p = e.position();
    p.row += dr;
    p.col += dc;
    e.setPosition(p);
}

void GravityMovement::apply(Entity &e, GameState &){
    auto p = e.position();
    char effectiveDir = dir;

    if(bounceTicks > 0){
        // flip direction temporarily
        if(dir == 'u') effectiveDir = 'd';
        else if(dir == 'd') effectiveDir = 'u';
        else if(dir == 'l') effectiveDir = 'r';
        else if(dir == 'r') effectiveDir = 'l';

        --bounceTicks;  // revert after bounceTicks ticks
    }

    switch(effectiveDir){
        case 'd': p.row++; break;
        case 'u': p.row--; break;
        case 'l': p.col--; break;
        case 'r': p.col++; break;
    }
    e.setPosition(p);
}

void CyclingMovement::apply(Entity &e, GameState &){
    if(++ticks >= interval){
        e.shape().advanceFrame();
        ticks = 0;
    }
}

void PlayerMovement::apply(Entity &e, GameState &state){
    auto p = e.position();
    switch(state.lastInput){
        case Action::UP:    p.row--; break;
        case Action::DOWN:  p.row++; break;
        case Action::LEFT:  p.col--; break;
        case Action::RIGHT: p.col++; break;
        default: break;
    }
    e.setPosition(p);
}

// For snake
SnakeMovement::SnakeMovement(Position start, int length): dr(0), dc(1) {
    for(int i = 0; i < length; ++i)
        body.push_back({ start.row, start.col - i });
}

void SnakeMovement::apply(Entity &head, GameState &state){
    // Prevent 180 deg turns
    int newDr = dr;
    int newDc = dc;

    switch(state.lastInput){
        case Action::UP:    newDr = -1; newDc = 0; break;
        case Action::DOWN:  newDr =  1; newDc = 0; break;
        case Action::LEFT:  newDr =  0; newDc = -1; break;
        case Action::RIGHT: newDr =  0; newDc =  1; break;
        default: break;
    }

    // Stops reversing
    if(!(newDr == -dr && newDc == -dc)){
        dr = newDr;
        dc = newDc;
    }

    // Next head pos
    Position next{
        body.front().row + dr,
        body.front().col + dc
    };

    // Self collide check
    for(const auto &seg : body){
        if(seg.row == next.row && seg.col == next.col){
            state.gameOver = true;
            state.win = false;
            return;
        }
    }

    // Move snake
    body.push_front(next);

    if(!pendingGrow){
        body.pop_back();
    }else{
        pendingGrow = false;
    }

    head.setPosition(body.front());
}

const std::deque<Position>& SnakeMovement::segments() const {
    return body;
}

void SnakeMovement::grow(){
    pendingGrow = true;
}
