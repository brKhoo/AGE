#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <memory>
#include <algorithm>
#include "entity.h"
#include "gameState.h"

class GameBoard {
    int rows_;
    int cols_;
    std::vector<std::unique_ptr<Entity>> ents;
public:
    GameBoard(int r, int c) : rows_{r}, cols_{c} {}

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    void addEntity(std::unique_ptr<Entity> e) {
        ents.push_back(std::move(e));
    }

    std::vector<std::unique_ptr<Entity>> &entities() { return ents; }
    const std::vector<std::unique_ptr<Entity>> &entities() const { return ents; }

    // Simple tick: update movements, then resolve collisions by position+height
    void tick(GameState &state);
};

#endif
