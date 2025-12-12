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

static bool touchesBorderAt(
    const Entity &e,
    Position base,
    int top, int left, int bottom, int right
) {
    for (const Pixel &px : e.shape().pixels()) {
        int r = base.row + px.rowOffset;
        int c = base.col + px.colOffset;

        if (r < top || r > bottom || c < left || c > right)
            return true;
    }
    return false;
}

static bool touchesBorder(const Entity &e, int top, int left, int bottom, int right) {
    return touchesBorderAt(e, e.position(), top, left, bottom, right);
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
                        Position otherPrev, Position otherCurr)
{
    // 1) Primary: geometry axis resolution (works for stationary + moving)
    Position undoRow{ selfPrev.row, selfCurr.col }; // undo row movement only
    Position undoCol{ selfCurr.row, selfPrev.col }; // undo col movement only

    bool stillHitIfUndoRow = pixelsOverlapAt(self, undoRow, other, otherCurr);
    bool stillHitIfUndoCol = pixelsOverlapAt(self, undoCol, other, otherCurr);

    bool bounceRow = !stillHitIfUndoRow; // if undoing row fixes overlap -> row axis caused collision
    bool bounceCol = !stillHitIfUndoCol; // if undoing col fixes overlap -> col axis caused collision

    // 2) If geometry is ambiguous, fall back to RELATIVE motion
    // Ambiguous cases:
    // - both undo tests still collide (weird shapes / deep overlap)
    // - both undo tests resolve (true corner hit)
    if (bounceRow == bounceCol) {
        int selfDr  = selfCurr.row  - selfPrev.row;
        int selfDc  = selfCurr.col  - selfPrev.col;
        int otherDr = otherCurr.row - otherPrev.row;
        int otherDc = otherCurr.col - otherPrev.col;

        int relDr = selfDr - otherDr;
        int relDc = selfDc - otherDc;

        // Prefer single-axis when possible
        if (std::abs(relDr) > std::abs(relDc)) { bounceRow = true;  bounceCol = false; }
        else if (std::abs(relDc) > std::abs(relDr)) { bounceRow = false; bounceCol = true; }
        else { bounceRow = true; bounceCol = true; } // true corner tie
    }

    // 3) Apply to movement components
    for (auto &m : self.movementComponents()) {
        if (auto sm = dynamic_cast<StraightMovement*>(m.get())) {
            if (bounceRow) sm->reverseRow();
            if (bounceCol) sm->reverseCol();
        }
        else if (auto gm = dynamic_cast<GravityMovement*>(m.get())) {
            if ((bounceRow && gm->affectsRow()) || (bounceCol && gm->affectsCol()))
                gm->bounce(3);
        }
    }
}

void applyBorderBounce(
    Entity &e,
    Position prev,
    Position curr,
    int top, int left, int bottom, int right
) {
    bool bounceRow = false;
    bool bounceCol = false;

    bool wasOutsideRow = false;
    bool isOutsideRow  = false;
    bool wasOutsideCol = false;
    bool isOutsideCol  = false;

    for (const Pixel &px : e.shape().pixels()) {
        int pr = prev.row + px.rowOffset;
        int pc = prev.col + px.colOffset;
        int cr = curr.row + px.rowOffset;
        int cc = curr.col + px.colOffset;

        if (pr < top || pr > bottom) wasOutsideRow = true;
        if (cr < top || cr > bottom) isOutsideRow  = true;

        if (pc < left || pc > right) wasOutsideCol = true;
        if (cc < left || cc > right) isOutsideCol  = true;
    }

    bounceRow = !wasOutsideRow && isOutsideRow;
    bounceCol = !wasOutsideCol && isOutsideCol;

    for (auto &m : e.movementComponents()) {
        if (auto sm = dynamic_cast<StraightMovement*>(m.get())) {
            if (bounceRow) sm->reverseRow();
            if (bounceCol) sm->reverseCol();
        }
        else if (auto gm = dynamic_cast<GravityMovement*>(m.get())) {
            if (bounceRow || bounceCol)
                gm->bounce(3);
        }
    }
}

void GameBoard::tick(GameState &state) {
    state.appleEatenThisTick = false;
    // 1) Apply movements + per-entity logic
    for (auto &e : ents) {
        if (!e->isMarkedForRemoval()) {
            e->updateMovements(state);
            e->updateSelf(state);
            e->tickCooldown();
        }
    }

    // 2) Border handling
    // Board bounds
    int top = 1;
    int left = 1;
    int bottom = rows() - 5;   // matches your view
    int right = cols() - 2;

    if (borderType == BorderType::Solid) {
        for (auto &up : ents) {
            Entity &e = *up;
            if (e.isMarkedForRemoval()) continue;

            bool wasOutside = touchesBorderAt(
                e, e.prevPosition(),
                top, left, bottom, right
            );

            bool isOutside = touchesBorder(
                e, top, left, bottom, right
            );

            // Only block if entity CROSSED the border this tick
            if (touchesBorderAt(e, e.position(), top, left, bottom, right) &&
                !touchesBorderAt(e, e.prevPosition(), top, left, bottom, right))  {
                Position prev = e.prevPosition();
                Position curr = e.position();

                CollisionResult r = e.collisionRule().handle(e, e, state);
                e.revertPosition();

                // Apply bounce if entity has bounce collision
                if (r == CollisionResult::Bounce) {
                    applyBorderBounce(e, prev, curr, top, left, bottom, right);
                }
                if (r == CollisionResult::Destroy) {
                    e.markForRemoval();
                }
            }
        }
    }else if (borderType == BorderType::View) {
        for (auto &up : ents) {
            Entity &e = *up;
            if (e.isMarkedForRemoval()) continue;
            if (touchesBorder(e, top, left, bottom, right)) {
                if (e.isPlayerControlled()) {
                    e.revertPosition(); // player cannot leave
                } else {
                    e.setOffScreenTicks(e.getOffScreenTicks()+1);
                    if (e.getOffScreenTicks() > e.getMaxOffScreenTicks())
                        e.markForRemoval();
                }
            } else {
                e.setOffScreenTicks(0);
            }
        }
    }

    // 3) Handle collisions
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
                    applyBounce(a, b, aPrev, aCurr, bPrev, bCurr);

                if (rb == CollisionResult::Bounce)
                    applyBounce(b, a, bPrev, bCurr, aPrev, aCurr);
            }
        }
    }

    // 4) Remove entities marked for removal
    ents.erase(
        std::remove_if(
            ents.begin(), ents.end(),
            [](const std::unique_ptr<Entity> &p) {
                return p->isMarkedForRemoval();
            }),
        ents.end()
    );
}
