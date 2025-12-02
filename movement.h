// movement.h
#ifndef MOVEMENT_H
#define MOVEMENT_H

class Entity;
struct GameState;

class Movement {
public:
    virtual ~Movement() = default;
    virtual void apply(Entity &e, GameState &state) = 0;
};

class StraightMovement : public Movement {
    int dr, dc;
public:
    StraightMovement(int r, int c) : dr{r}, dc{c} {}
    void apply(Entity &e, GameState &state) override;
};

class GravityMovement : public Movement {
    char dir;
public:
    GravityMovement(char c): dir{c} {}
    void apply(Entity &e, GameState &state) override;
};

class CyclingMovement : public Movement {
    int ticks = 0;  // Counts ticks
    int interval;   // Number of ticks between form changes.
public:
    CyclingMovement(int t) : interval{t} {}
    void apply(Entity &e, GameState &state) override;
};

class PlayerMovement : public Movement {
public:
    void apply(Entity &e, GameState &state) override;
};

#endif
