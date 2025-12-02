#ifndef VIEW_H
#define VIEW_H

#include "gameBoard.h"
#include "gameState.h"

class View {
public:
    virtual ~View() = default;
    virtual void draw(const GameBoard &board, const GameState &state) = 0;
};

#endif
