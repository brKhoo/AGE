// movement.h
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <deque>

class Entity;
struct GameState;
struct Position;
enum class Action;

class Movement {
public:
    virtual ~Movement() = default;
    virtual void apply(Entity &e, GameState &state) = 0;

    virtual bool affectsRow() const { return false; }
    virtual bool affectsCol() const { return false; }

    virtual void reverseRow() {}
    virtual void reverseCol() {}
};

class StraightMovement : public Movement {
    int dr, dc;
public:
    StraightMovement(int r, int c) : dr{r}, dc{c} {}
    void apply(Entity &e, GameState &state) override;
    void reverseRow() { dr = -dr; }
    void reverseCol() { dc = -dc; }

    bool affectsRow() const override { return dr != 0; }
    bool affectsCol() const override { return dc != 0; }
};

class GravityMovement : public Movement {
    char dir;
    int bounceTicks = 0;
public:
    GravityMovement(char c) : dir(c) {}
    void apply(Entity &e, GameState &state) override;

    bool affectsRow() const override {
        return dir == 'u' || dir == 'd';
    }
    bool affectsCol() const override {
        return dir == 'l' || dir == 'r';
    }
    void bounce(int ticks = 1) {
        bounceTicks = ticks;
    }
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

class SnakeMovement : public Movement {
    std::deque<Position> body;
    int dr = 0, dc = 1;
    bool pendingGrow;
public:
    SnakeMovement(Position start, int length = 3);
    void apply(Entity &head, GameState &state) override;

    const std::deque<Position>& segments() const;
    void grow();
};

#endif
