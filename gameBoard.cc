#include "gameBoard.h"

void GameBoard::tick(GameState &state) {
    // 1) Apply movements + per-entity logic
    for (auto &e : ents) {
        if (!e->isMarkedForRemoval()) {
            e->updateMovements(state);
            e->updateSelf(state);
        }
    }

    // 2) Handle collisions: same position and same height
    const std::size_t n = ents.size();
    for (std::size_t i = 0; i < n; ++i) {
        Entity &a = *ents[i];
        if (a.isMarkedForRemoval()) continue;
        for (std::size_t j = i + 1; j < n; ++j) {
            Entity &b = *ents[j];
            if (b.isMarkedForRemoval()) continue;
            if (a.height() != b.height()) continue;
            if (a.position().row == b.position().row &&
                a.position().col == b.position().col) {
                a.onCollide(b, state);
                b.onCollide(a, state);
            }
        }
    }

    // 3) Remove entities marked for removal
    ents.erase(
        std::remove_if(
            ents.begin(), ents.end(),
            [](const std::unique_ptr<Entity> &p) {
                return p->isMarkedForRemoval();
            }),
        ents.end()
    );
}
