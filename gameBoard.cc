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

static bool pixelsOverlapAt(const Entity &a, Position posA,
                            const Entity &b, Position posB) {
    const auto &pa = a.shape().pixels();
    const auto &pb = b.shape().pixels();

    for (const Pixel &pxA : pa) {
        int worldAr = posA.row + pxA.rowOffset;
        int worldAc = posA.col + pxA.colOffset;

        for (const Pixel &pxB : pb) {
            int worldBr = posB.row + pxB.rowOffset;
            int worldBc = posB.col + pxB.colOffset;

            if (worldAr == worldBr && worldAc == worldBc)
                return true;
        }
    }
    return false;
}

static void applyBounce(Entity &self,
                        const Entity &other,
                        Position selfPrev, Position selfCurr,
                        Position otherCurr) {
    // We know selfCurr vs otherCurr overlaps (collision detected).

    // Test: if we undo ONLY row movement, do we stop overlapping?
    Position undoRow = { selfPrev.row, selfCurr.col };
    bool stillHitIfUndoRow = pixelsOverlapAt(self, undoRow, other, otherCurr);

    // Test: if we undo ONLY col movement, do we stop overlapping?
    Position undoCol = { selfCurr.row, selfPrev.col };
    bool stillHitIfUndoCol = pixelsOverlapAt(self, undoCol, other, otherCurr);

    // If undoing row fixes collision => row caused it => bounce row.
    bool bounceRow = !stillHitIfUndoRow;
    // If undoing col fixes collision => col caused it => bounce col.
    bool bounceCol = !stillHitIfUndoCol;

    // Corner case: both tests still overlap (or both resolve).
    // If both resolve, reflect both (true corner hit).
    // If neither resolves (rare with weird shapes), fall back to delta.
    if (!bounceRow && !bounceCol) {
        int dRow = selfCurr.row - selfPrev.row;
        int dCol = selfCurr.col - selfPrev.col;
        bounceRow = (dRow != 0);
        bounceCol = (dCol != 0);
    }

    for (auto &m : self.movementComponents()) {
        if (auto sm = dynamic_cast<StraightMovement*>(m.get())) {
            if (bounceRow) sm->reverseRow();
            if (bounceCol) sm->reverseCol();
        } else if (auto gm = dynamic_cast<GravityMovement*>(m.get())) {
            // gravity bounce: only if it was moving along the bounced axis
            if ((bounceRow && gm->affectsRow()) || (bounceCol && gm->affectsCol()))
                gm->bounce(3);
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

    // 2) Handle collisions
    const std::size_t n = ents.size();
    for (std::size_t i = 0; i < n; ++i) {
        Entity &a = *ents[i];
        if (a.isMarkedForRemoval()) continue;

        for (std::size_t j = i + 1; j < n; ++j) {
            Entity &b = *ents[j];
            if (b.isMarkedForRemoval()) continue;
            if (a.height() != b.height()) continue;

            if (pixelsOverlap(a, b)) {

                // Save movement intent BEFORE correction
                Position aPrev = a.prevPosition();
                Position aCurr = a.position();
                Position bPrev = b.prevPosition();
                Position bCurr = b.position();

                CollisionResult ra = a.collisionRule().handle(a, b, state);
                CollisionResult rb = b.collisionRule().handle(b, a, state);

                // Correct position FIRST
                if (ra == CollisionResult::Block || ra == CollisionResult::Bounce)
                    a.revertPosition();

                if (rb == CollisionResult::Block || rb == CollisionResult::Bounce)
                    b.revertPosition();

                // Correct velocity SECOND
                if (ra == CollisionResult::Bounce)
                    applyBounce(a, b, aPrev, aCurr, bCurr);

                if (rb == CollisionResult::Bounce)
                    applyBounce(b, a, bPrev, bCurr, aCurr);

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
