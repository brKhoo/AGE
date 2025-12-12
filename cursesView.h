#ifndef CURSES_VIEW_H
#define CURSES_VIEW_H

#include "view.h"
#include <ncurses.h>

class CursesView: public View{
public:
    void draw(const GameBoard &board, const GameState &state) override;
};

#endif
