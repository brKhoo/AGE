#ifndef TEST_BOARD_H
#define TEST_BOARD_H

#include "gameBoard.h"

// Thin wrapper around GameBoard, in case you want custom behaviour later.
class TestBoard : public GameBoard {
public:
    TestBoard(int r, int c) : GameBoard(r, c) {}
};

#endif
