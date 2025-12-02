#include "gameBoard.h"

// AABB collision detection helper
bool pixelsOverlap(const Entity &a, const Entity &b) {
    const auto &pa = a.shape().pixels();
    const auto &pb = b.shape().pixels();

    const Position posA = a.position();
    const Position posB = b.position();

    for (const Pixel &pxA : pa) {
        int worldAr = posA.row + pxA.rowOffset;
        int worldAc = posA.col + pxA.colOffset;

        for (const Pixel &pxB : pb) {
            int worldBr = posB.row + pxB.rowOffset;
            int worldBc = posB.col + pxB.colOffset;

            if (worldAr == worldBr && worldAc == worldBc) {
                return true;
            }
        }
    }

    return false;
}

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
            if (pixelsOverlap(a, b)) {
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
