#include "gameBoard.h"
#include "collision.h"
#include "movement.h"

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

// Bounce collision helper
bool overlapAt(const Entity &a, const Entity &b, Position testPos) {
    Position saved = a.position();
    const_cast<Entity&>(a).setPosition(testPos);
    bool hit = pixelsOverlap(a, b);
    const_cast<Entity&>(a).setPosition(saved);
    return hit;
}

// Bounce collision helper
void applyBounce(Entity &self, Entity &other) {
    Position prev = self.prevPosition();
    Position curr = self.position();

    bool hitRow = overlapAt(self, other, { curr.row, prev.col });
    bool hitCol = overlapAt(self, other, { prev.row, curr.col });

    self.revertPosition();

    for (auto &m : self.movementComponents()) {
        // Straight movement: permanent reflection
        if (auto sm = dynamic_cast<StraightMovement*>(m.get())) {
            if (hitRow) sm->reverseRow();
            if (hitCol) sm->reverseCol();
        }

        // Gravity: temporary bounce
        else if (auto gm = dynamic_cast<GravityMovement*>(m.get())) {
            if ((hitRow && gm->affectsRow()) ||
                (hitCol && gm->affectsCol())) {
                gm->bounce(3); // 3 ticks per bounce
            }
        }
    }

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
                auto ra = a.collisionRule().handle(a, b, state);
                auto rb = b.collisionRule().handle(b, a, state);

                if (ra == CollisionResult::Block)
                    a.revertPosition();

                if (rb == CollisionResult::Block)
                    b.revertPosition();

                if (ra == CollisionResult::Bounce)
                    applyBounce(a, b);

                if (rb == CollisionResult::Bounce)
                    applyBounce(b, a);
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
